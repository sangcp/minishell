/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils2.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sangcpar <sangcpar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/25 03:58:15 by sangcpar          #+#    #+#             */
/*   Updated: 2021/10/27 18:15:26 by sangcpar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	reset_fds(t_shell *mini)
{
	dup2(mini->fds[0], 0);
	dup2(mini->fds[1], 1);
	dup2(mini->stdinp, 0);
	dup2(mini->stdout, 1);
}

void	mini_c_p(t_shell *mini, t_list *list)
{
	mini->prev_pipe = STDIN_FILENO;
	mini->count = ft_lstsize(list);
	mini->i = 0;
}

void	list_jmp(t_shell *mini, t_list **list)
{
	int	i;

	i = -1;
	*list = (*list)->next;
	while (mini->i > 0)
	{
		*list = (*list)->next;
		mini->i--;
	}
	while (mini->heredoc_name[++i])
	{
		unlink(mini->heredoc_name[i]);
		free(mini->heredoc_name[i]);
	}
	if (mini->heredoc_name)
		free(mini->heredoc_name);
}

void	free_list(t_list **list)
{
	t_list	*tlist;
	int		i;

	tlist = *list;
	i = 0;
	while (tlist)
	{
		while (((t_ops *)(tlist->content))->args[i])
			free(((t_ops *)(tlist->content))->args[i++]);
		free(((t_ops *)(tlist->content))->args);
		free(((t_ops *)(tlist->content))->operation);
		free(((t_ops *)(tlist->content))->q_chk);
		free(((t_ops *)(tlist->content)));
		free(tlist);
		tlist = tlist->next;
		i = 0;
	}
}
