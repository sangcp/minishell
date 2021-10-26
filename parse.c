/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sangcpar <sangcpar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/02 03:15:45 by sangcpar          #+#    #+#             */
/*   Updated: 2021/10/26 15:01:49 by sangcpar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static char	**list_to_arr(t_list *list)
{
	int		size;
	int		i;
	t_list	*lst;
	char	**arr;

	if (!list)
		return (NULL);
	lst = list;
	size = ft_lstsize(list);
	arr = (char **)malloc(sizeof(char *) * (size + 1));
	i = 0;
	while (i < size)
	{
		arr[i] = ft_strdup(lst->content);
		i++;
		lst = lst->next;
	}
	arr[size] = NULL;
	ft_lstclear(&list, &free);
	return (arr);
}

int	normi(char *line, int i)
{
	if (i == 1 && line[i] == ' ' && line[i + 1] != ' ')
		i--;
	return (i);
}

char	**parse_args(char *line, t_ops *ops)
{
	t_list	*list;
	int		i;

	i = -1;
	list = NULL;
	while (line[++i])
	{
		ops->in_quotes = 0;
		while (line[i] == ' ')
			i++;
		while (*line == ' ')
			line++;
		if (!line[0])
			break ;
		if (line_chk(line, i))
		{
			i = normi(line, i);
			if (line[0] == '\"' || line[0] == '\'')
				i = quote_skip(line, i, line[0], ops);
			ft_lstadd_back(&list, ft_lstnew(ft_substr(line, 0, i + 1)));
			line += i + 1;
			i = -1;
		}
	}
	return (list_to_arr(list));
}

t_ops	*set_ops(char *cmd, int i)
{
	t_ops	*ops;
	int		j;

	j = 1;
	ops = (t_ops *)malloc(sizeof(t_ops));
	if (cmd[i] != '\0' && cmd[i - 1] == ' ')
	{
		while (cmd[i - j] == ' ')
			j++;
		ops->operation = ft_substr(cmd, 0, i - j + 1);
	}
	else
		ops->operation = ft_substr(cmd, 0, i);
	ops->args = parse_args(ops->operation, ops);
	if (cmd[i] && cmd[i + 1] == '>')
		ops->type = '}';
	else if (cmd[i] && cmd[i + 1] == '<')
		ops->type = '{';
	else
		ops->type = cmd[i];
	ops->next = NULL;
	return (ops);
}

t_list	*parse_option(char *cmd)
{
	t_list	*list;
	t_ops	ops;
	int		i;

	i = 0;
	list = NULL;
	if (cmd_chk(cmd))
		return (NULL);
	while (1)
	{
		if (cmd[++i] == '\'' || cmd[i] == '\"')
			i = quote_skip(cmd, i, cmd[i], &ops);
		else if (!cmd[i] || ft_strchr(";|<>", cmd[i]))
		{
			if (add_list(&list, cmd, i))
				return (NULL);
			if (!cmd[i])
				return (list);
			if (cmd[i] && (cmd[i + 1] == '>' || cmd[i + 1] == '<'))
				cmd += 1;
			cmd += i + 1;
			i = 0;
		}
	}
	return (list);
}
