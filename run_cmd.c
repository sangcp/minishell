/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   run_cmd.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sangcpar <sangcpar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/03 10:03:38 by sangcpar          #+#    #+#             */
/*   Updated: 2021/10/27 19:40:43 by sangcpar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	cmd_exec(char *path, t_shell *mini)
{
	char	*tmp;

	tmp = ft_strjoin(path, "/");
	path = tmp;
	tmp = ft_strjoin(path, *mini->args);
	execve(tmp, mini->args, mini->c_evs);
	if (errno != ENOENT)
		perror("minishell:");
	free(tmp);
}

void	bin_chk(t_shell *mini)
{
	char	*tmp;

	if (!ft_strncmp(mini->args[0], "/bin/", 5))
	{
		tmp = ft_strdup(*mini->args + 5);
		free(mini->args[0]);
		mini->args[0] = ft_strdup(tmp);
		free(tmp);
	}
}

int	run_cmd2(t_shell *mini, char **envp)
{
	char	**path;
	pid_t	pid;
	int		i;

	pid = fork();
	signal(SIGINT, sighandler2);
	signal(SIGQUIT, pipe_sighandler2);
	i = 0;
	if (pid == 0)
	{
		path = ft_split2(get_env(envp, "PATH"), ':');
		bin_chk(mini);
		while (path[i])
			cmd_exec(path[i++], mini);
		printf("minishell: command not found : %s\n", mini->args[0]);
		path_free(path);
		return (-1);
	}
	wait(&pid);
	return (0);
}

int	exec_cmp(t_shell *mini, char **args, t_list *list)
{
	mini->args = args;
	if (mini->args == NULL)
		return (0);
	if (ft_strcmp(args[0], "exit") == 0)
	{
		printf("%s\n", "exit");
		return (-1);
	}
	if (!(ft_strncmp(args[0], "cd", 2)))
		return (cmd_cd(args, mini->c_evs));
	if (!(ft_strncmp(args[0], "echo", 4)))
		return (cmd_echo(list, args));
	if (!(ft_strncmp(args[0], "export", 7)))
	{
		cmd_export(mini, args);
		return (0);
	}
	if (!(ft_strncmp(args[0], "env", 3)))
		return (cmd_env(args, mini));
	if (!(ft_strncmp(args[0], "pwd", 3)))
		return (cmd_pwd(args));
	if (!(ft_strncmp(args[0], "unset", 5)))
		return (cmd_unset(mini));
	return (run_cmd2(mini, mini->c_evs));
}

int	run_cmd1(t_shell *mini, t_list *list)
{
	while (mini->count > 1)
	{
		mini->here_ck = 0;
		mini->args = ((t_ops *)(list->content))->args;
		if (((t_ops *)(list->content))->type > ';')
		{
			dup2(mini->prev_pipe, STDIN_FILENO);
			operator_exec(list, mini);
			if (((t_ops *)(list->content))->type != '|')
				list = list->next;
		}
		else
		{
			dup2(mini->prev_pipe, STDIN_FILENO);
			mini->rv = exec_cmp(mini, mini->args, list);
		}
		if (mini->rv == -1)
			return (-1);
		list_jmp(mini, &list);
		mini->count = ft_lstsize(list);
	}
	if (list && mini->count)
	{
		dup2(mini->prev_pipe, STDIN_FILENO);
		mini->rv = exec_cmp(mini, ((t_ops *)(list->content))->args, list);
		if (mini->rv == -1)
			return (-1);
	}
	return (0);
}
