/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   multi_oper_utils.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sangcpar <sangcpar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/27 05:06:15 by sangcpar          #+#    #+#             */
/*   Updated: 2021/10/27 16:41:40 by sangcpar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	multi_chk1(t_shell *mini, t_list *list)
{
	t_list	*tlist;
	int		i;
	int		re_in;
	int		re_out;

	i = 0;
	re_in = 0;
	re_out = 0;
	tlist = list;
	mini->i = 0;
	while (tlist)
	{
		if (((t_ops *)(tlist->content))->type == '<')
			re_in = 1;
		if (((t_ops *)(tlist->content))->type == '>')
			re_out = 1;
		if ((((t_ops *)(tlist->content))->type == '<' || \
		((t_ops *)(tlist->content))->type == '{') && \
		((t_ops *)(tlist->next->content))->type == '>')
			i = 1;
		tlist = tlist->next;
	}
	tlist = list;
	while (tlist && ((t_ops *)(tlist->content))->type != '{')
		tlist = tlist->next;
	/*if (i && tlist && ((t_ops *)(tlist->content))->type == '{')
		if (re_in && re_out)
			mini->i++;*/
	return (i);
}

int	multi_chk2(t_shell *mini, t_list *list)
{
	t_list	*tlist;
	int		i;
	int		re_in;
	int		re_pipe;

	i = 0;
	re_in = 0;
	re_pipe = 0;
	tlist = list;
	(void)mini;
	while (tlist)
	{
		if (((t_ops *)(tlist->content))->type == '<')
			re_in = 1;
		if (((t_ops *)(tlist->content))->type == '|')
			re_pipe = 1;
		if ((((t_ops *)(tlist->content))->type == '<' || \
		((t_ops *)(tlist->content))->type == '{') && \
		(((t_ops *)(tlist->next->content))->type == '|'))
			i = 1;
		tlist = tlist->next;
	}
	tlist = list;
	while (tlist && ((t_ops *)(tlist->content))->type != '{')
		tlist = tlist->next;
	if (i && tlist && ((t_ops *)(tlist->content))->type == '{')
		if (re_in && re_pipe)
			mini->i++;
	return (i);
}
