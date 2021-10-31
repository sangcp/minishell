/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_echo.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sangcpar <sangcpar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/25 03:38:16 by sangcpar          #+#    #+#             */
/*   Updated: 2021/10/25 03:38:17 by sangcpar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	print_echo1(char **str, int i)
{
	int	j;

	j = -1;
	while (str[i][++j])
		ft_putchar_fd(str[i][j], 1);
}

int	cmd_echo(t_list *list, char **args)
{
	int	n_flag;
	int	i;

	i = 1;
	if (!args[1])
	{
		ft_putchar_fd('\n', 1);
		return (0);
	}
	n_flag = 0;
	if (args[1][0] == '-' && args[1][1] == 'n' && args[1][2] == '\0')
		n_flag = 1;
	if (n_flag)
		i++;
	while (args[i])
	{
		print_echo1(args, i);
		if (args[i + 1] && ((t_ops *)(list->content))->q_chk[i] != '0')
			ft_putchar_fd(' ', 1);
		i++;
	}
	if (!n_flag)
		ft_putchar_fd('\n', 1);
	return (0);
}
