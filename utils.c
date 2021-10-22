/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sangcpar <sangcpar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/15 16:21:04 by sangcpar          #+#    #+#             */
/*   Updated: 2021/10/15 16:21:06 by sangcpar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	is_quotes(char c)
{
	if (c == '"' || c == '\'')
		return (1);
	return (0);
}

void	path_free(char **str)
{
	int	i;

	i = 0;
	while (str[i])
		free(str[i++]);
	free(str);
}

void	free_all(t_shell *mini, t_list *list)
{
	t_list	*tlist;
	int	i;

	(void)mini;
	i = 0;
	if (list == NULL)
		return ;
	tlist = list;
	while (tlist->next)
	{
		if (((t_ops *)(tlist->content))->args)
		{
			while (((t_ops *)(tlist->content))->args[i])
			{
				free(((t_ops *)(tlist->content))->args[i]);
				i++;
			}
			free(((t_ops *)(tlist->content))->args);
		}
		free(((t_ops *)(tlist->content))->operation);
		free(((t_ops *)(tlist->content)));
		free(tlist);
		tlist = tlist->next;
		i = 0;
	}
	while (((t_ops *)(tlist->content))->args[i])
	{
		free(((t_ops *)(tlist->content))->args[i]);
		i++;
	}
	if (((t_ops *)(tlist->content))->operation)
		free(((t_ops *)(tlist->content))->operation);
	if (((t_ops *)(tlist->content))->args)
		free(((t_ops *)(tlist->content))->args);
	if (((t_ops *)(tlist->content)))
		free(((t_ops *)(tlist->content)));
	if (tlist)
		free(tlist);
	if (mini->q_c)
		free(mini->q_c);
}
