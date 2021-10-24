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

void	print_echo(char **str, int i)
{
	int	starts_qu;
	int	ends_qu;
	int	len;
	int	j;

	j = -1;
	if (!str[i])
		return ;
	starts_qu = is_quotes(str[i][0]);
	len = (int)ft_strlen(str[i]);
	ends_qu = is_quotes(str[i][len - 1]);
	if (ends_qu && starts_qu)
		ft_putnstr(str[i] + 1, -1);
	else if (ends_qu)
		ft_putnstr(str[i], -1);
	else if (starts_qu)
		ft_putstr_fd(str[i] + 1, 1);
	else
	{
		while (str[i][++j])
			if (str[i][j] != '\'' && str[i][j] != '\"')
				ft_putchar_fd(str[i][j], 1);
	}
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
		print_echo(args, i);
		if (args[i + 1] && ((t_ops *)(list->content))->q_chk[i] != '0')
			ft_putchar_fd(' ', 1);
		i++;
	}
	if (!n_flag)
		ft_putchar_fd('\n', 1);
	return (0);
}
