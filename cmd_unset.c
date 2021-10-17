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
	char	**ret;

	i = 0;
	while (evs[i])
		i++;
	ret = malloc((sizeof(char *)) * i);
	if (!ret)
		return (NULL);
	i = 0;
	while (*evs)
	{
		j = 0;
		while ((*evs)[j] != '=')
			j++;
		if (ft_strncmp(*evs, arg, j + 1) != '=')
		{
			ret[i] = ft_strdup(*evs);
			i++;
		}
		evs++;
	}
	ret[i] = NULL;
	/*i = 0;
	if (evs)
	{
		while (evs[i] != NULL)
		{
			free(evs[i]);
			i++;
		}
		free(evs);
	}*/
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
