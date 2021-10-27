/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   multi_oper_utils.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sangcpar <sangcpar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/27 05:06:15 by sangcpar          #+#    #+#             */
/*   Updated: 2021/10/27 05:06:16 by sangcpar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	multi_chk1(t_list *list)
{
	t_list	*tlist;

	tlist = list;
	while (tlist)
	{
		if ((((t_ops *)(tlist->content))->type == '<' || \
		((t_ops *)(tlist->content))->type == '{') && \
		((t_ops *)(tlist->next->content))->type == '>')
			return (1);
		tlist = tlist->next;
	}
	return (0);
}

int	multi_chk2(t_list *list)
{
	t_list	*tlist;

	tlist = list;
	while (tlist)
	{
		if ((((t_ops *)(tlist->content))->type == '<' || \
		((t_ops *)(tlist->content))->type == '{') && \
		(((t_ops *)(tlist->next->content))->type == '|'))
			return (1);
		tlist = tlist->next;
	}
	return (0);
}
