/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sangcpar <sangcpar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/02 03:15:45 by sangcpar          #+#    #+#             */
/*   Updated: 2021/10/27 18:25:35 by sangcpar         ###   ########.fr       */
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

void	add_back(t_list **list, t_ops *ops, char **line, int i)
{
	if ((*line)[0] == '\"' || (*line)[0] == '\'')
	{
		if ((*line)[0] == (*line)[1])
		{
			(*line) += 2;
			i = i_jump(*line);
		}
		else
			i = quote_skip((*line), i, (*line)[0], ops);
	}
	else if ((*line)[0] == '$')
		i = i_jump(*line);
	else
		i = i_jump(*line);
	ft_lstadd_back(list, ft_lstnew(ft_substr(*line, 0, i + 1)));
	(*line) += i + 1;
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
			add_back(&list, ops, &line, i);
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

char	*cmd_change(char **cmd)
{
	char	*tmp;
	int		i;
	int		j;

	i = 0;
	j = 0;
	tmp = ft_strdup("");
	while ((*cmd)[j])
	{
		if ((*cmd)[j] != '\'' && (*cmd)[j] != '\"')
		{
			tmp = fstr_join(&tmp, &(*cmd)[j]);
		}
		j++;
	}
	free(*cmd);
	return (tmp);
}

t_list	*parse_option(char **command)
{
	t_list	*list;
	t_ops	ops;
	int		i;
	char	*cmd;

	i = -1;
	list = NULL;
	if (cmd_chk(command))
		return (NULL);
	*command = cmd_change(command);
	cmd = *command;
	while (1)
	{
		if (cmd[++i] == '\'' || cmd[i] == '\"')
			i = quote_skip(cmd, i, cmd[i], &ops);
		else if (!cmd[i] || ft_strchr("|<>", cmd[i]))
		{
			if (add_list(&list, cmd, i))
				return (NULL);
			if (!cmd[i])
				return (list);
			cmd_jump(&cmd, &i);
		}
	}
	return (list);
}
