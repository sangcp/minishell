/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   multi_oper.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sangcpar <sangcpar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/26 02:44:00 by sangcpar          #+#    #+#             */
/*   Updated: 2021/10/26 02:44:02 by sangcpar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	operator_exec1(t_list *list, t_shell *mini)
{
	if (((t_ops *)(list->content))->type == '|')
		return (operator_pipe(list, mini));
	else if (((t_ops *)(list->content))->type == '>')
		return (redirect_output(list, mini));
	else if (((t_ops *)(list->content))->type == '}')
		return (append_output(list, mini));
	else if (((t_ops *)(list->content))->type == '<')
		return (redirect_input(list, mini));
	else if (((t_ops *)(list->content))->type == '{')
		return (append_input(mini, list));
	else
		return (-1);
}

int	multi_pipe(t_list *list, t_shell *mini)
{
	pid_t	pid;

	if (((t_ops *)(list->content))->type == '{')
		make_heredoc(mini, list);
	mini->args = ((t_ops *)(list->content))->args;
	pipe(((t_ops *)(list->content))->fds);
	pid = fork();
	if (pid == 0)
	{
		close(((t_ops *)(list->content))->fds[0]);
		dup2(mini->prev_pipe, STDIN_FILENO);
		dup2(((t_ops *)(list->content))->fds[1], 1);
		redirect_input(list, mini);
		exit(0);
	}
	wait(&pid);
	close(((t_ops *)(list->content))->fds[1]);
	mini->prev_pipe = ((t_ops *)(list->content))->fds[0];
	if (((t_ops *)(list->content))->type == '{')
		unlink(((t_ops *)(list->next->content))->args[0]);
	return (0);
}

int	multi_input(t_list *list, t_shell *mini)
{
	int		fd;
	char	*filename;

	if (((t_ops *)(list->content))->type == '{')
		make_heredoc(mini, list);
	mini->fds[0] = dup(STDIN_FILENO);
	filename = ((t_ops *)(list->next->content))->args[0];
	fd = open(filename, O_RDONLY);
	while (((t_ops *)(list->next->content))->type == '<')
	{
		mini->i++;
		list = list->next;
		filename = ((t_ops *)(list->next->content))->args[0];
		fd = open(filename, O_RDONLY);
	}
	if (fd == -1)
	{
		printf("%s: no such file or directory\n", filename);
		return (-1);
	}
	dup2(fd, STDIN_FILENO);
	mini->rv = exec_cmp(mini, mini->args, list);
	dup2(mini->fds[0], STDIN_FILENO);
	close(fd);
	return (0);
}

int	multi_red(t_shell *mini, t_list *list)
{
	int		fd;
	char	*filename;
	t_list	*tlist;

	tlist = list;
	if (((t_ops *)(list->content))->type == '{')
		make_heredoc(mini, tlist);
	while (((t_ops *)(list->content))->type != '>')
		list = list->next;
	mini->fds[0] = dup(STDOUT_FILENO);
	filename = ((t_ops *)(list->next->content))->args[0];
	fd = open(filename, O_WRONLY | O_CREAT | O_TRUNC, 0666);
	while (((t_ops *)(list->next->content))->type == '>' || \
	((t_ops *)(list->next->content))->type == '}')
	{
		mini->i++;
		list = list->next;
		filename = ((t_ops *)(list->next->content))->args[0];
		if (((t_ops *)(list->next->content))->type == '>')
			fd = open(filename, O_WRONLY | O_CREAT | O_TRUNC, 0666);
		else
			fd = open(filename, O_WRONLY | O_CREAT | O_APPEND, 0666);
	}
	if (fd == -1)
		return (-1);
	dup2(fd, STDOUT_FILENO);
	redirect_input(tlist, mini);
	dup2(mini->fds[0], STDOUT_FILENO);
	close(fd);
	if (((t_ops *)(tlist->content))->type == '{')
		unlink(((t_ops *)(tlist->next->content))->args[0]);
	return (0);
}

int	operator_exec(t_list *list, t_shell *mini)
{
	if (multi_chk1(list))
	{
		mini->i++;
		return (multi_red(mini, list));
	}
	else if (multi_chk2(list))
		return (multi_pipe(list, mini));
	else if (((t_ops *)(list->content))->type == '|')
		return (operator_pipe(list, mini));
	else if (((t_ops *)(list->content))->type == '>')
		return (redirect_output(list, mini));
	else if (((t_ops *)(list->content))->type == '}')
		return (append_output(list, mini));
	else if (((t_ops *)(list->content))->type == '<')
		return (redirect_input(list, mini));
	else if (((t_ops *)(list->content))->type == '{')
		return (append_input(mini, list));
	else
		return (-1);
}
