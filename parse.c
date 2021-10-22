/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sangcpar <sangcpar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/02 03:15:45 by sangcpar          #+#    #+#             */
/*   Updated: 2021/10/16 19:40:08 by sangcpar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	quote_skip(char *cmd, int i, char q)
{
	while (1)
	{
		i++;
		if (cmd[i] == q)
			return (i);
	}
}

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

char	**parse_args(char *line, t_ops *ops)
{
	t_list	*list;
	//char	*tmp;
	int		i;

	ops->in_quotes = 0;
	i = 0;
	list = NULL;
	while (line[i])
	{
		/*while (*line == ' ')
			line++;*/
		while (line[i] == ' ')
			i++;
		while (*line == ' ')
			line++;
		if (!line[0])
			break ;
		if (((line[0] == '\"' || line[0] == '\'' || !line[i + 1] || \
		(line[i] != ' ' && line[i + 1] == ' ')) && i > 0) || (i == 0 && line[0] && !line[1]))
		{
			if (line[0] == '\"' || line[0] == '\'')
			{
				i = quote_skip(line, i, line[0]);
				ops->in_quotes = 1;
			}
			//if (ops->in_quotes == 0)
			ft_lstadd_back(&list, ft_lstnew(ft_substr(line, 0, i + 1)));
			/*else
				ft_lstadd_back(&list, ft_lstnew(ft_substr(line, 1, i - 1)));*/
			line += i + 1;
			i = -1;
		}
		i++;
		ops->in_quotes = 0;
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

int		cmd_chk(char *cmd)
{
	int i;

	i = 0;
	if (!ft_strcmp(cmd, ""))
		return (1);
	while (cmd[i] == ' ')
		i++;
	if (!cmd[i])
		return (1);
	i = 0;
	if (ft_strchr("<>|;", cmd[i]))
	{
		i++;
		if (cmd[0] == '|' || cmd[0] == ';')
		{
			ft_putstr_fd("minishell: syntax error near unexpected token ", 2);
			printf("\'%c\'\n", cmd[0]);
			return (1);
		}
		while (cmd[i] == '<' || cmd[i] == '>')
			i++;
		while (cmd[i] && cmd[i] == ' ')
			i++;
		if (!cmd[i])
		{
			ft_putstr_fd("minishell: syntax error near unexpected token ", 2);
			if (cmd[0] == '<' || cmd[0] == '>')
				ft_putstr_fd("\'newline\'\n", 2);
			else
				printf("\'%c\'\n", cmd[0]);
			return (1);
		}
	}
	return (0);
}

t_list	*parse_option(char *cmd)
{
	t_list	*list;
	t_ops	*ops;
	int		i;

	i = 0;
	if (cmd_chk(cmd))
		return (NULL);
	list = NULL;
	while (1)
	{
		if (cmd[i] == '\'' || cmd[i] == '\"')
			i = quote_skip(cmd, i, cmd[i]);
		else if (!cmd[i] || ft_strchr(";|<>", cmd[i]))
		{
			ops = set_ops(cmd, i);
			ft_lstadd_back(&list, ft_lstnew(ops));
			if (!cmd[i])
				return (list);
			if (cmd[i] && (cmd[i + 1] == '>' || cmd[i + 1] == '<'))
				cmd += 1;
			cmd += i + 1;
			i = 0;
		}
		i++;
	}
	return (list);
}
