/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   multi_oper.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sangcpar <sangcpar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/26 02:44:00 by sangcpar          #+#    #+#             */
/*   Updated: 2021/10/27 19:46:54 by sangcpar         ###   ########.fr       */
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

void	m_chk(t_list *list, t_shell *mini)
{
	t_list	*tlist;
	int		i;

	i = 0;
	tlist = list;
	while (tlist)
	{
		if (((t_ops *)(tlist->content))->type == '{' || \
		((t_ops *)(tlist->content))->type == '<')
			i++;
		tlist = tlist->next;
	}
	if (i > 1)
		mini->i++;
}

int	multi_pipe(t_list *list, t_shell *mini)
{
	pid_t	pid;

	/*if (((t_ops *)(list->content))->type == '{')
		make_heredoc(mini, list);*/
	m_chk(list, mini);
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
/*
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
}*/

int	multi_red(t_shell *mini, t_list *list)
{
	int		fd;
	char	*filename;
	t_list	*tlist;

	tlist = list;
	/*if (tlist && (((t_ops *)(tlist->content))->type != '{' || \
	((t_ops *)(tlist->content))->type != '<'))
	{
		ft_putstr_fd("aaaa\n", 1);
		tlist = tlist->next;
	}*/
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
		if (((t_ops *)(list->content))->type == '>')
			fd = open(filename, O_WRONLY | O_CREAT | O_TRUNC, 0666);
		else if (((t_ops *)(list->content))->type == '}')
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

void	heredoc_chk(t_list *list, t_shell *mini)
{
	t_list	*tlist;
	int		here;

	tlist = list;
	here = 0;
	while (tlist)
	{
		if (((t_ops *)(tlist->content))->type == '{')
			here++;
		tlist = tlist->next;
	}
	if (here > 0)
	{
		mini->heredoc_name = (char **)malloc(sizeof(char *) * (here + 1));
		mini->here_ck = 1;
	}
	tlist = list;
	here = 0;
	while (tlist)
	{
		if (((t_ops *)(tlist->content))->type == '{')
		{
			make_heredoc(mini, tlist);
			mini->heredoc_name[here++] = ft_strdup(((t_ops *)(tlist->next->content))->args[0]);
			mini->heredoc_name[here] = NULL;
		}
		tlist = tlist->next;
	}
	/*while (tlist && ((t_ops *)(tlist->content))->type != '{')
		tlist = tlist->next;
	if (tlist && ((t_ops *)(tlist->content))->type == '{')
	{
		make_heredoc(mini, tlist);
		mini->heredoc_name = ((t_ops *)(tlist->next->content))->args[0]
	}*/
}

int	operator_exec(t_list *list, t_shell *mini)
{
	heredoc_chk(list, mini);
	if (multi_chk1(mini, list))
	{
		mini->i++;
		return (multi_red(mini, list));
	}
	else if (multi_chk2(mini, list))
		return (multi_pipe(list, mini));
	else if (((t_ops *)(list->content))->type == '|')
		return (operator_pipe(list, mini));
	else if (((t_ops *)(list->content))->type == '>')
		return (redirect_output(list, mini));
	else if (((t_ops *)(list->content))->type == '}')
		return (append_output(list, mini));
	else if (((t_ops *)(list->content))->type == '<' || \
	((t_ops *)(list->content))->type == '{')
		return (redirect_input(list, mini));
	else
		return (-1);
}
