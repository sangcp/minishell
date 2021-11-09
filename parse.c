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

char	*qskip_substr(char const *s, unsigned int start, size_t len)
{
	char	*new;
	size_t	i;
	size_t	j;

	new = 0;
	i = 0;
	j = 0;
	if (s == 0)
		return (NULL);
	while (s[j])
	{
		if (s[j] != '\'' && s[j] != '\"')
			i++;
		j++;
	}
	new = (char *)malloc(sizeof(char) * (i + 1));
	if (new == 0)
		return (NULL);
	i = 0;
	j = start;
	while (j < start + len && s[j])
	{
		if (s[j] != '\'' && s[j] != '\"')
		{
			new[i] = s[j];
			i++;
		}
		j++;
	}
	new[i] = '\0';
	return (new);
}

void	add_back(t_list **list, t_ops *ops, char **line, int i)
{
	(void)ops;
	/*if ((*line)[0] == '\"' || (*line)[0] == '\'')
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
		i = i_jump(*line);*/
	while ((*line)[i] && (*line)[i] != ' ')
		i++;
	ft_lstadd_back(list, ft_lstnew(qskip_substr(*line, 0, i)));
	(*line) += i;
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

static char	*repl_change(char *in, int i, int len, char *val)
{
	char	*out;

	if (val)
	{
		out = malloc(ft_strlen(in) - len + ft_strlen(val) + 1);
		if (!out)
			return (NULL);
		ft_strlcpy(out, in, i + 1);
		ft_strlcpy(out + i, val, ft_strlen(val) + 1);
		ft_strlcpy(out + i + ft_strlen(val), in + i + len, \
			ft_strlen(in) - len - i + 1);
	}
	else
	{
		out = malloc(ft_strlen(in) - len);
		if (!out)
			return (NULL);
		ft_strlcpy(out, in, i + 1);
		ft_strlcpy(out + i, in + i + len, ft_strlen(in) - i - len + 1);
	}
	free(val);
	return (out);
}

int	max(int a, int b)
{
	if (a > b)
		return (a);
	return (b);
}

static int	repl_env_name(char *in, int i, char **env, char **val)
{
	int		clip;
	int		j;
	int		k;
	char	*tkn;

	clip = 0;
	while (in[i + 1 + clip] && !ft_strchr(" \t$\"\'\\", in[i + 1 + clip]))
		clip++;
	tkn = in + i + 1;
	j = 0;
	while (env[j])
	{
		k = ft_strnstr(env[j], "=", ft_strlen(env[j])) - env[j];
		if (!ft_strncmp(env[j], tkn, max(clip, k)))
		{
			*val = ft_strdup(env[j] + k + 1);
			break ;
		}
		j++;
	}
	return (clip);
}
int	repl_env(int i, char **in, t_shell *mini)
{
	char	*val;
	int		len;

	val = NULL;
	if ((*in)[i + 1] == '?')
	{
		*in = repl_change(*in, i, 2, ft_itoa(mini->rv));
		return (1);
	}
	else
	{
		len = repl_env_name(*in, i, mini->c_evs, &val) + 1;
		*in = repl_change(*in, i, len, val);
		if (!val)
			return (0);
		return (len);
	}
}

char	*cmd_change(t_shell *mini, char *in)
{
	int		i;
	char	inquotes;

	i = 0;
	inquotes = 0;
	while (in && in[i])
	{
		if (in[i] == '\"' && (i == 0 || (i > 0 && in[i - 1] != '\\')))
			inquotes = (inquotes + 1) % 2;
		if (in[i] == '\'' && (i == 0 || (i > 0 && in[i - 1] != '\\')) \
			&& !inquotes)
		{
			i++;
			while (!(in[i] == '\'' && in[i - 1] != '\\'))
				i++;
		}
		if (in[i] == '$' && in[i + 1] && in[i + 1] != '$')
			i += repl_env(i, &in, mini) - 1;
		i++;
	}
	return (in);
}

t_list	*parse_option(t_shell *mini, char **command)
{
	t_list	*list;
	t_ops	ops;
	int		i;
	char	*cmd;

	i = -1;
	list = NULL;
	(void)mini;
	if (cmd_chk(command))
		return (NULL);
	cmd = *command;
	cmd = cmd_change(mini, cmd);
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
