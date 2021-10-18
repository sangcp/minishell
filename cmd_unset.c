/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_unset.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sangcpar <sangcpar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/17 08:21:17 by sangcpar          #+#    #+#             */
/*   Updated: 2021/10/17 13:48:56 by sangcpar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	**rm_env(char **evs, char *arg)
{
	int		i;
	int		j;
	int 	k;
	char	**ret;
	char	**tmp;

	tmp = evs;
	i = 0;
	k = 0;
	while (evs[i])
		i++;
	ret = malloc((sizeof(char *)) * i);
	if (!ret)
		return (NULL);
	i = 0;
	while (evs[k])
	{
		j = 0;
		while (evs[k][j] && evs[k][j] != '=')
			j++;
		if (ft_strncmp(evs[k], arg, j + 1) != '=')
		{
			ret[i] = ft_strdup(evs[k]);
			printf("%s\n", evs[k]);
			i++;
		}
		k++;
	}
	ret[i] = NULL;
	i = 0;
	if (tmp)
	{
		while (tmp[i] != NULL)
		{
			free(tmp[i]);
			i++;
		}
		free(tmp);
	}
	return (ret);
}

int cmd_unset(t_shell *mini, char **envp)
{
	int i;

	i = 1;
	(void)envp;
	if (mini->args[1] == NULL)
		return (0);
	while (mini->args[i] != NULL)
		mini->c_evs = rm_env(mini->c_evs, mini->args[i++]);
	return (0);
}
