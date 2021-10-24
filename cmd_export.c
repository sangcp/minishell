/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_export.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sangcpar <sangcpar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/15 16:47:23 by sangcpar          #+#    #+#             */
/*   Updated: 2021/10/16 19:17:49 by sangcpar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	**plus_line(char **env, char *new_env)
{
	char	**envp;
	char	*tmp;
	int		i;
	int		j;
	int		k;

	i = 0;
	j = -1;
	while (new_env[i] && new_env[i] != '=')
		i++;
	tmp = ft_substr(new_env, 0, i);
	i = 0;
	while (env[i])
	{
		if (!ft_strncmp(tmp, env[i], ft_strlen(tmp)))
			j = i;
		i++;
	}
	if (j != -1)
		i--;
	envp = (char **)malloc(sizeof(char *) * (i + 2));
	i = 0;
	k = 0;
	while (env[i])
	{
		if (i != j)
			envp[k++] = ft_strdup(env[i]);
		i++;
	}
	envp[k] = ft_strdup(new_env);
	envp[++k] = NULL;
	path_free(env);
	free(tmp);
	return (envp);
}

int	check_equal_len(char *str)
{
	int	i;

	i = 0;
	while (str[i] != '=')
		i++;
	return (i);
}

int	swap_envp(char *str1, char *str2)
{
	int	i;

	i = 0;
	while (str1[i] == str2[i])
		i++;
	if (str1[i] > str2[i])
		return (1);
	return (0);
}

char	**desending_envp(char **envp)
{
	char	**desen_envp;
	char	*tmp;
	int		i;
	int		j;

	i = 0;
	while (envp[i])
		i++;
	desen_envp = (char **)malloc(sizeof(char *) * (i + 1));
	i = 0;
	while (envp[i])
	{
		desen_envp[i] = ft_strdup(envp[i]);
		i++;
	}
	desen_envp[i] = NULL;
	i = 0;
	while (desen_envp[i])
	{
		j = i + 1;
		while (desen_envp[j])
		{
			if (swap_envp(desen_envp[i], desen_envp[j]))
			{
				tmp = desen_envp[i];
				desen_envp[i] = desen_envp[j];
				desen_envp[j] = tmp;
			}
			j++;
		}
		i++;
	}
	path_free(envp);
	return (desen_envp);
}

void	cmd_export(t_shell *mini, char **args)
{
	int	i;
	int	j;
	int	q_flag;

	mini->c_evs = desending_envp(mini->c_evs);
	if (!(ft_strcmp(args[0], "export")) && !args[1])
	{
		i = 0;
		while (mini->c_evs[i])
		{
			j = 0;
			q_flag = 0;
			printf("declare -x ");
			while (mini->c_evs[i][j])
			{
				printf("%c", mini->c_evs[i][j]);
				if (mini->c_evs[i][j] == '=')
				{
					printf("\"");
					q_flag = 1;
				}
				j++;
			}
			if (q_flag)
				printf("\"");
			printf("\n");
			i++;
		}
	}
	else if (args[1][0] == '$')
	{
		i = 0;
		if (!args[1][1])
		{
			printf("minishell: export: `$': not a valid identifier\n");
			return ;
		}
		while (mini->c_evs[i])
		{
			printf("declare -x %s\n", mini->c_evs[i]);
			i++;
		}
	}
	else
		mini->c_evs = plus_line(mini->c_evs, args[1]);
}

int	find_str_len(char *str)
{
	int	i;
	int	j;

	i = 0;
	j = 0;
	while (str[i])
	{
		if (str[i] == '=')
		{
			while (str[i])
			{
				i++;
				j++;
			}
			break ;
		}
		i++;
	}
	return (j);
}

int	check_str_in_env(char *str, char **envp)
{
	printf("%s\n %s\n", str, envp[1]);
	return (1);
}

int	print_export(char *str, char **envp)
{
	char	*new_str;
	int		i;
	int		j;

	if (!check_str_in_env(str, envp))
	{
		j = find_str_len(str);
		new_str = (char *)malloc(sizeof(char) * j);
		i = 0;
		j = 0;
		while (str[i])
		{
			if (str[i] == '=')
			{
				while (str[i])
				{
					new_str[j] = str[i];
					j++;
					i++;
				}
				break ;
			}
			i++;
		}
		printf("%s\n", new_str);
	}
	else
	{
		j = find_str_len(str);
		new_str = (char *)malloc(sizeof(char) * (j - 1));
		i = 0;
		j = 0;
		while (str[i])
		{
			if (str[i] == '=')
			{
				i++;
				while (str[i])
				{
					new_str[j] = str[i];
					j++;
					i++;
				}
				break ;
			}
			i++;
		}
		printf("%s\n", new_str);
	}
	return (1);
}
