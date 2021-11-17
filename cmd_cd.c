/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_cd.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sangcpar <sangcpar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/25 03:35:34 by sangcpar          #+#    #+#             */
/*   Updated: 2021/10/25 03:35:35 by sangcpar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*get_path(char **strs)
{
	if (!strs[1][1])
		return ("");
	else
		return (strs[1] + 1);
}

int	cmd_cd(char **args, t_shell *mini)
{
	if (args[1][0] == '~')
	{
		if ((chdir(ft_strjoin(get_env(mini->c_evs, "HOME"), get_path(args), 4))) == -1)
			ft_putstr_fd("cd fail\n", 2);
		return (0);
	}
	if ((chdir(args[1])) == -1)
		ft_putstr_fd("cd fail\n", 2);
	return (0);
}
