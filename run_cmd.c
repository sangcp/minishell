/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   run_cmd.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sangcpar <sangcpar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/03 10:03:38 by sangcpar          #+#    #+#             */
/*   Updated: 2021/10/16 18:23:23 by sangcpar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	operator_exec1(t_list *list, t_shell *mini, char **envp)
{
	if (((t_ops *)(list->content))->type == '|')
		return (operator_pipe(list, mini, envp));
	else if (((t_ops *)(list->content))->type == '>')
		return (redirect_output(list, mini, envp));
	else if (((t_ops *)(list->content))->type == '}')
		return (append_output(list, mini, envp));
	else if (((t_ops *)(list->content))->type == '<')
		return (redirect_input(list, mini, envp));
	else if (((t_ops *)(list->content))->type == '{')
		return (append_input(mini, list, envp));
	else
		return (-1);
}

int	test_input(t_list *list, t_shell *mini, char **envp)
{
	pid_t	pid;

	mini->args = ((t_ops *)(list->content))->args;
	pipe(((t_ops *)(list->content))->fds);
	pid = fork();
	if (pid == 0)
	{
		close(((t_ops *)(list->content))->fds[0]);
		dup2(mini->prev_pipe, STDIN_FILENO);
		dup2(((t_ops *)(list->content))->fds[1], 1);
		operator_exec1(list, mini, envp);
		exit(0);
	}
	wait(&pid);
	close(((t_ops *)(list->content))->fds[1]);
	mini->prev_pipe = ((t_ops *)(list->content))->fds[0];
	return (0);
}
// -----------
int	operator_exec(t_list *list, t_shell *mini, char **envp)
{
	if ((((t_ops *)(list->content))->type == '<' ||\
	((t_ops *)(list->content))->type == '{') && \
	(((t_ops *)(list->next->content))->type == '|' ||\
	((t_ops *)(list->next->content))->type == '>'))
		return (test_input(list, mini, envp));
	if (((t_ops *)(list->content))->type == '|')
		return (operator_pipe(list, mini, envp));
	else if (((t_ops *)(list->content))->type == '>')
		return (redirect_output(list, mini, envp));
	else if (((t_ops *)(list->content))->type == '}')
		return (append_output(list, mini, envp));
	else if (((t_ops *)(list->content))->type == '<')
		return (redirect_input(list, mini, envp));
	else if (((t_ops *)(list->content))->type == '{')
		return (append_input(mini, list, envp));
	else
		return (-1);
}

int	run_cmd2(t_shell *mini, char **envp)
{
	char	**path;
	char	*tmp;
	pid_t	pid;
	int		i;

	pid = fork();
	signal(SIGINT, sighandler2);
	signal(SIGQUIT, pipe_sighandler2);
	i = 0;
	if (pid == 0)
	{
		path = ft_split2(get_env(envp, "PATH"), ':');
		if (!ft_strncmp(mini->args[0], "/bin/", 5))
		{
			tmp = ft_strdup(*mini->args + 5);
			free(mini->args[i]);
			mini->args[i] = ft_strdup(tmp);
			free(tmp);
		}
		while (path[i])
		{
			tmp = ft_strjoin(path[i], "/");
			path[i] = tmp;
			tmp = ft_strjoin(path[i], *mini->args);
			execve(tmp, mini->args, envp);
			if (errno != ENOENT)
				perror("minishell:");
			free(tmp);
			i++;
		}
		ft_putstr_fd("minishell: command not found : ", STDERR_FILENO);
		ft_putstr_fd(mini->args[0], STDERR_FILENO);
		write(STDERR_FILENO, "\n", 1);
		path_free(path);
		return (-1);
	}
	wait(&pid);
	return (0);
}

int	cmd_pwd(char **args, char **envp)
{
	char	buf[1000];

	(void)args;
	(void)envp;
	getcwd(buf, 1000);
	printf("%s\n", buf);
	return (0);
}

int	exec_cmp(t_shell *mini, char **args, char **envp)
{
	unsigned long	i;

	i = 0;
	//args = transl_env(mini, args);
	if (mini->args == NULL)
		return (0);
	if (ft_strcmp(args[0], "exit") == 0)
	{
		printf("%s\n", "exit");
		return (-1);
	}
	if (!(ft_strncmp(args[0], "cd", 2)))
		return (cmd_cd(args, envp));
	if (!(ft_strncmp(args[0], "echo", 4)))
		return (cmd_echo(mini, args, envp));
	if (!(ft_strncmp(args[0], "export", 7)))
	{
		cmd_export(mini, args);
		return (0);
	}
	if (!(ft_strncmp(args[0], "env", 3)))
		return (cmd_env(args, envp));
	if (!(ft_strncmp(args[0], "pwd", 3)))
		return (cmd_pwd(args, envp));
	if (!(ft_strncmp(args[0], "unset", 5)))
		return (cmd_unset(mini, envp));
	return (run_cmd2(mini, envp));
}

int	run_cmd1(t_shell *mini, t_list *list, char **envp)
{
	while (mini->count > 1)
	{
		mini->args = ((t_ops *)(list->content))->args;
		if (((t_ops *)(list->content))->type > ';')
		{
			dup2(mini->prev_pipe, STDIN_FILENO);
			operator_exec(list, mini, envp);
			//test_input(list, mini, envp);
			if (((t_ops *)(list->content))->type != '|')
				list = list->next;
		}
		else
			mini->rv = exec_cmp(mini, mini->args, envp);
		if (mini->rv == -1)
			return (-1);
		list = list->next;
		mini->count = ft_lstsize(list);
	}
	if (list && mini->count)
	{
		dup2(mini->prev_pipe, STDIN_FILENO);
		mini->args = ((t_ops *)(list->content))->args;
		mini->rv = exec_cmp(mini, mini->args, envp);
		if (mini->rv == -1)
			return (-1);
	}
	return (0);
}
