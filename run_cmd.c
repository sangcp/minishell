/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   run_cmd.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sangcpar <sangcpar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/03 10:03:38 by sangcpar          #+#    #+#             */
/*   Updated: 2021/10/14 14:42:40 by sangcpar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
/*
int append_input(t_list *list, t_shell *mini, char **envp)
{
	char **cmd;
	char *cm;
	int i;
	char *exit_c;
	(void)envp;
	(void)mini;
	int fd;

	
	cmd = (char**)malloc(sizeof(char*) * 99);
	cmd[0] = ft_strdup(((t_ops *)(list->content))->args[0]);
	exit_c = ((t_ops *)(list->next->content))->args[0];
	i = 1;
	while (1)
	{
		cm = readline("heredoc> ");
		if (!(ft_strcmp(exit_c, cm)))
			break ;
		cmd[i] = ft_strdup(cm);
		printf("%s| %s| %s|\n", cm, cmd[i], exit_c);
		i++;
	}
	((t_ops *)(list->content))->args = cmd;
	i = 0;
	fd = open(((t_ops *)(list->content))->args[0], O_RDONLY);
	while (((t_ops *)(list->content))->args[i])
		printf("%s\n", ((t_ops *)(list->content))->args[i++]);
	((t_ops *)(list->content))->type = ' ';
	mini->count--;
	return (0);
}*/
/*
int	append_here(t_list *list, t_shell *mini, char **envp)
{
	int		fd;
	char	*filename;
	(void)list;

	mini->fds[0] = dup(STDIN_FILENO);
	filename = ((t_ops *)(list->next->content))->args[0];
	printf("na = %s\n", filename);
	fd = open(filename, O_RDONLY);
	if (fd == -1)
	{
		printf("%s: no such file or directory\n", filename);
		return (-1);
	}
	dup2(mini->fds[0], STDIN_FILENO);
	mini->rv = exec_cmp(mini, ((t_ops *)(list->content))->args, envp);
	close(mini->fds[0]);
	dup2(mini->fds[0], STDIN_FILENO);
	return (0);
}

static void	write_heredoc(t_shell *mini)
{
	char	*str;
	(void)mini;
	int		fd;
	//char *line;
	//char buf[42];

	fd = open(mini->args[0], O_WRONLY | O_TRUNC | O_CREAT, 0644);
	printf("%s %d\n", mini->args[0], fd);
	if (fd < 0)
		exit(1);
	str = readline("heredoc> ");
	while (str != NULL && ft_strcmp(str, mini->args[0]))
	{
		write(fd, str, ft_strlen(str));
		write(fd, "\n", 1);
		free(str);
		str = readline("heredoc> ");
	}
	close(fd);
	fd = open(mini->args[0], O_RDONLY);
	//get_next_line(fd, &line);
	read(fd, buf, 100);
	printf("a = %s b = %s\n", mini->args[0], buf);
	free(str);
	close(fd);
}*/

int	operator_exec(t_list *list, t_shell *mini, char **envp)
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

int run_cmd2(t_shell *mini, char **envp)
{
	char **path;
	//char **strs;
	char *tmp;
	pid_t pid;
	int i;

	pid = fork();
	signal(SIGINT, sighandler2);
	signal(SIGQUIT, pipe_sighandler2);
	i = 0;
	if (pid == 0)
	{
		path = ft_split2(get_env(envp, "PATH"), ':');
		/*if (!ft_strncmp(mini->args, "/bin/", 5))
			*strs = *strs + 5;*/
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
		//path_free(strs);
		return (-1);
	}
	wait(&pid);
	return (0);
}

int cmd_pwd(char **args, char **envp)
{
	char buf[1000];

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
		return (cmd_echo(args, envp));
    if (!(ft_strncmp(args[0], "export", 5)))
    {
        envp = cmd_export(args, envp);
        return (0);
    }
    if (!(ft_strncmp(args[0], "env", 3)))
        return (cmd_env(args, envp));
	if (!(ft_strncmp(args[0], "pwd", 3)))
        return (cmd_pwd(args, envp));
	return (run_cmd2(mini, envp));
}

int run_cmd1(t_shell *mini, t_list *list, char **envp)
{
	/*if (((t_ops *)(list->content))->type == '{')
	{
		append_input(mini, list, envp);
		list = list->next;
		mini->count = ft_lstsize(list);
	}*/
	while (mini->count > 1)
	{
		mini->args = ((t_ops *)(list->content))->args;
		if (((t_ops *)(list->content))->type > ';')
		{
			dup2(mini->prev_pipe, STDIN_FILENO);
			operator_exec(list, mini, envp);
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
	if (list)
	{
		dup2(mini->prev_pipe, STDIN_FILENO);
		mini->args = ((t_ops *)(list->content))->args;
		mini->rv = exec_cmp(mini, mini->args, envp);
		if (mini->rv == -1)
			return (-1);
	}
	return (0);
}
