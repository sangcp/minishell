/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sangcpar <sangcpar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/26 02:20:40 by sangcpar          #+#    #+#             */
/*   Updated: 2021/10/27 18:23:53 by sangcpar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	q_count(char **cmd);

void	join_cmd(char **cmd, char q)
{
	char	*tmp;
	//int		i;
	(void)q;

	tmp = readline("> ");
	*cmd = pipe_join(cmd, "\n");
	*cmd = pipe_join(cmd, tmp);
	free(tmp);
	/*while (1)
	{
		*cmd = pipe_join(cmd, "\n");
		*cmd = pipe_join(cmd, tmp);
		free(tmp);
		i = 0;
		while ((*cmd)[i])
			i++;
		if ((*cmd)[i - 1] == q)
			break ;
		tmp = readline("> ");
	}*/
}

int	q_count(char **cmd)
{
	int		i;
	int		re;
	char	q;

	i = -1;
	re = 0;
	while ((*cmd)[++i])
	{
		if ((*cmd)[i] == '\'' || (*cmd)[i] == '\"')
		{
			q = (*cmd)[i];
			i++;
			while ((*cmd)[i] != '\0' && (*cmd)[i] != q)
				i++;
			if ((*cmd)[i] == '\0')
			{
				re = 1;
				join_cmd(cmd, q);
			}
			break ;
		}
	}
	return (re);
}

void	plus_cmd(char **cmd)
{
	char	*tmp;
	int		j;

	j = 1;
	while ((*cmd)[ft_strlen(*cmd) - j] == ' ')
		j++;
	if ((*cmd)[ft_strlen(*cmd) - j] == '|')
	{
		tmp = readline("pipe> ");
		while (1)
		{
			*cmd = pipe_join(cmd, tmp);
			free(tmp);
			while ((*cmd)[ft_strlen(*cmd) - j] == ' ')
				j++;
			if ((*cmd)[ft_strlen(*cmd) - j] != '|')
				break ;
			tmp = readline("pipe> ");
		}
	}
	j = 1;
	while (j)
		j = q_count(cmd);
}

int	oper_err_msg(char **cmd, int i)
{
	if (ft_strchr("<>|;", (*cmd)[i]))
	{
		i++;
		if ((*cmd)[0] == '|' || (*cmd)[0] == ';')
		{
			ft_putstr_fd("minishell: syntax error near unexpected token ", 2);
			printf("\'%c\'\n", (*cmd)[0]);
			return (1);
		}
		while ((*cmd)[i] == '<' || (*cmd)[i] == '>')
			i++;
		while ((*cmd)[i] && (*cmd)[i] == ' ')
			i++;
		if (!(*cmd)[i])
		{
			ft_putstr_fd("minishell: syntax error near unexpected token ", 2);
			if ((*cmd)[0] == '<' || (*cmd)[0] == '>')
				ft_putstr_fd("\'newline\'\n", 2);
			else
				printf("\'%c\'\n", (*cmd)[0]);
			return (1);
		}
	}
	plus_cmd(cmd);
	return (0);
}

int	add_list(t_list **list, char *cmd, int i)
{
	int		j;
	t_ops	*ops;

	j = 1;
	if (cmd[i])
	{
		while (cmd[i + j] && cmd[i + j] == ' ')
			j++;
		if (!cmd[i + j])
		{
			if (cmd[i] == '<' || cmd[i] == '>')
			{
				ft_putstr_fd("minishell: syntax error ", 2);
				ft_putstr_fd("near unexpected token `newline'\n", 2);
			}
			free_list(list);
			return (1);
		}
	}
	ops = set_ops(cmd, i);
	ft_lstadd_back(list, ft_lstnew(ops));
	return (0);
}

int	cmd_chk(char **cmd)
{
	int	i;

	i = 0;
	if (!ft_strcmp(*cmd, ""))
		return (1);
	while ((*cmd)[i] == ' ')
		i++;
	if (!(*cmd)[i])
		return (1);
	i = 0;
	if (oper_err_msg(cmd, i))
		return (1);
	return (0);
}

int	quote_skip(char *cmd, int i, char q, t_ops *ops)
{
	ops->in_quotes = 1;
	while (1)
	{
		i++;
		if (cmd[i] == q)
			return (i);
	}
}

int	line_chk(char *line, int i)
{
	return (((line[0] == '\"' || line[0] == '\'' || !line[i + 1] || \
	(line[i] != ' ' && line[i + 1] == ' ')) && i > 0) || \
	(i == 0 && line[0] && !line[1]) || \
	(i == 1 && line[i] == ' ' && line[i + 1] != ' '));
}
