/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   run_cmd.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sangcpar <sangcpar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/03 10:03:38 by sangcpar          #+#    #+#             */
/*   Updated: 2021/12/01 14:45:15 by sangcpar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	cmd_exec(char *path, t_shell *mini)
{
	char	*tmp;

	tmp = ft_strjoin(path, "/", 4);
	path = tmp;
	tmp = ft_strjoin(path, *mini->args, 4);
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

void	put_err_msg(char *msg, int chk, char **path)
{
	if (chk == 1)
	{
		ft_putstr_fd("minishell: ", 2);
		ft_putstr_fd(msg, 2);
		ft_putstr_fd(": No such file or directory\n", 2);
		exit(2);
	}
	else
	{
		ft_putstr_fd("minishell: ", 2);
		ft_putstr_fd(msg, 2);
		ft_putstr_fd(": command not found \n", 2);
		path_free(path);
		exit(2);
	}
}

int	run_cmd2(t_shell *mini, char **envp)
{
	char	**path;
	pid_t	pid;
	int		i;
	int		status;

	pid = fork();
	signal(SIGINT, sighandler2);
	signal(SIGQUIT, pipe_sighandler2);
	i = 0;
	if (pid == 0)
	{
		path = ft_split2(get_env(envp, "PATH"), ':');
		if (!path)
			put_err_msg(mini->args[0], 1, NULL);
		bin_chk(mini);
		while (path[i] && mini->args[0][0] != '\0')
			cmd_exec(path[i++], mini);
		put_err_msg(mini->args[0], 2, path);
	}
	waitpid(pid, &status, WUNTRACED);
	if (status != 0)
		mini->rv = 127;
	else
		mini->rv = 0;
	return (0);
}

int	exec_cmp_norm(t_shell *mini, char **args, t_list *list)
{
	if (!(ft_strncmp(args[0], "cd", 2)))
		return (cmd_cd(args, mini));
	if (!(ft_strncmp(args[0], "echo", 4)))
		return (cmd_echo(list, args));
	if (!(ft_strncmp(args[0], "export", 7)))
		return (cmd_export(mini, args, list));
	if (!(ft_strncmp(args[0], "env", 3)))
		return (cmd_env(args, mini, list));
	if (!(ft_strncmp(args[0], "pwd", 3)))
		return (cmd_pwd(args));
	if (!(ft_strncmp(args[0], "unset", 5)))
		return (cmd_unset(mini));
	return (100);
}

int	exec_cmp(t_shell *mini, char **args, t_list *list)
{
	mini->args = args;
	if (mini->args == NULL)
		return (0);
	if (ft_strcmp(args[0], "exit") == 0)
	{
		if (mini->pipe_chk == 1)
			return (0);
		ft_putstr_fd("exit\n", 2);
		return (-1);
	}
	if (exec_cmp_norm(mini, args, list) != 100)
		return (0);
	run_cmd2(mini, mini->c_evs);
	ft_close(((t_ops *)(list->content))->fds[0]);
	ft_close(mini->pipe[1]);
	ft_close(mini->pipe[0]);
	ft_close(mini->prev_pipe);
	mini->pipe[0] = -1;
	mini->pipe[1] = -1;
	mini->prev_pipe = -1;
	return (0);
}

int	run_cmd1(t_shell *mini, t_list *list)
{
	int		i;

	i = 0;
	while (mini->count > 1)
	{
		mini->here_ck = 0;
		mini->args = ((t_ops *)(list->content))->args;
		dup2(mini->prev_pipe, STDIN_FILENO);
		if (ft_strchr("|<>{}", ((t_ops *)(list->content))->type))
		{
			operator_exec(list, mini);
			if (((t_ops *)(list->content))->type != '|')
				list = list->next;
		}
		list_jmp(mini, &list);
		mini->count = ft_lstsize(list);
	}
	if (list && mini->count)
	{
		dup2(mini->prev_pipe, STDIN_FILENO);
		i = exec_cmp(mini, ((t_ops *)(list->content))->args, list);
		if (i == -1)
			return (-1);
	}
	return (0);
}
