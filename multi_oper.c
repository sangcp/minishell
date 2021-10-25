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

int	multi_oper(t_list *list, t_shell *mini)
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
		operator_exec1(list, mini);
		exit(0);
	}
	wait(&pid);
	close(((t_ops *)(list->content))->fds[1]);
	mini->prev_pipe = ((t_ops *)(list->content))->fds[0];
	return (0);
}

int	operator_exec(t_list *list, t_shell *mini)
{
	if ((((t_ops *)(list->content))->type == '<' || \
	((t_ops *)(list->content))->type == '{') && \
	(((t_ops *)(list->next->content))->type == '|' || \
	((t_ops *)(list->next->content))->type == '>'))
		return (multi_oper(list, mini));
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
