/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sangcpar <sangcpar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/09/29 14:36:52 by sangcpar          #+#    #+#             */
/*   Updated: 2021/09/29 14:36:53 by sangcpar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"


char		*get_path1(char **envp)
{
	int		i;
	int		j;
	char	find[5];

	i = 0;
	ft_strlcpy(find, "PATH", 5);
	while (envp[i])
	{
		j = 0;
		while (envp[i][j] != '=')
			j++;
		if (ft_strncmp(find, envp[i], j) == 0)
			return (envp[i] + j + 1);
		++i;
	}
	return (NULL);
}

void		get_cmd_pipe(char *av, char **envp)
{
	char	*tmp;
	char	**path;
	char	**strs;
	int		i;

	path = ft_split(get_env(envp, "PATH"), ':');
	strs = ft_split(av, ' ');
	i = 0;
	while (path[i])
	{
		tmp = ft_strjoin(path[i], "/");
		free(path[i]);
		path[i] = tmp;
		tmp = ft_strjoin(path[i], *strs);
		execve(tmp, strs, envp);
		if (errno != ENOENT)
			perror("pipex:");
		free(tmp);
		i++;
	}
	ft_putstr_fd("pipex: command not found : ", STDERR_FILENO);
	ft_putstr_fd(*strs, STDERR_FILENO);
	write(STDERR_FILENO, "\n", 1);
	//free_strtab(path);
	//free_strtab(strs);
}

int cmd_pipe(char *cmd, char **envp)
{
	int		pipefd[2];
	int		status;
	char	**command;
	pid_t	pid;

	pipe(pipefd);
	command = ft_split(cmd, ' ');
	if ((pid = fork()) == 0)
	{
		close(pipefd[0]);
		dup2(pipefd[1], STDOUT_FILENO);
		get_cmd_pipe(command[0], envp);
	}
	else if (pid > 0)
	{
		waitpid(pid, &status, 0);
		close(pipefd[1]);
		dup2(pipefd[0], STDIN_FILENO);
		get_cmd_pipe(command[2], envp);
	}
	return (0);
}