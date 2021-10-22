/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_env.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sangcpar <sangcpar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/15 16:40:42 by sangcpar          #+#    #+#             */
/*   Updated: 2021/10/15 16:40:43 by sangcpar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	find_equal(char *cmd)
{
	int	i;

	i = 0;
	while (cmd[i])
	{
		if (cmd[i] == '=')
			return (1);
		i++;
	}
	return (0);
}

int	cmd_env(char **args, t_shell *mini)
{
	int	i;

	i = 0;
	if (!(ft_strcmp(args[0], "env")))
	{
		while (mini->c_evs[i])
		{
			printf("%s\n", mini->c_evs[i]);
			i++;
		}
	}
	else
	{
		while (mini->c_evs[i])
		{
			printf("%s\n", mini->c_evs[i]);
			i++;
		}
		if (find_equal(mini->c_evs[1]))
			printf("%s\n", mini->c_evs[1]);
		else
			printf("env : %s: No such file or directory\n", mini->c_evs[1]);
	}
	return (1);
}
