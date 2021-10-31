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

void	put_oneenv(char *evs, char *line, int *chk)
{
	int	j;

	j = 0;
	if (!ft_strncmp(line, evs, ft_strlen(line)))
	{
		*chk = 1;
		while (evs[j] != '=')
			j++;
		printf("%s\n", &evs[++j]);
	}
}

int	cmd_env(char **args, t_shell *mini, t_list *list)
{
	int		i;
	int		chk;

	i = 0;
	chk = 0;
	(void)list;
	if (!(ft_strcmp(args[0], "env")) && !args[1])
		while (mini->c_evs[i])
			printf("%s\n", mini->c_evs[i++]);
	else if (!(ft_strcmp(args[0], "env")) && args[1])
	{
		while (mini->c_evs[i])
		{
			put_oneenv(mini->c_evs[i], args[1], &chk);
			i++;
		}
		if (!chk)
			printf("env : %s: No such file or directory\n", args[1]);
	}
	return (1);
}
