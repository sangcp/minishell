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

char	*change_env(char *new_env)
{
	int		i;
	char	*new_str;
	int		check;
	int		flag;

	flag = 0;
	check = 0;
	new_str = (char *)malloc(sizeof(char) * (ft_strlen(new_env) + 2));
	i = 0;
	while (new_env[i])
	{
		new_str[check] = new_env[i];
		check++;
		i++;
		if (new_env[i] == '=')
		{
			new_str[check++] = new_env[i];
			new_str[check] = '\"';
			check++;
			i++;
			flag = 1;
		}
	}
	if (flag)
		new_str[check] = '\"';
	return (new_str);
}

char	**plus_line(char **env, char *new_env)
{
	char	**envp;
	char	*new_change_env;
	int		i;

	new_change_env = change_env(new_env);
	i = 0;
	while (env[i])
		i++;
	envp = (char **)malloc(sizeof(char *) * (i + 1));
	i = 0;
	while (env[i])
	{
		envp[i] = env[i];
		i++;
	}
	envp[i] = new_change_env;
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
	i = 0;
	if (envp)
	{
		while (envp[i])
			free(envp[i++]);
		free(envp);
	}
	return (desen_envp);
}

void	cmd_export(t_shell *mini, char **args)
{
	int	i;

	mini->c_evs = desending_envp(mini->c_evs);
	if (!(ft_strcmp(args[0], "export")) && !args[1])
	{
		i = 0;
		while (mini->c_evs[i])
		{
			printf("declare -x %s\n", mini->c_evs[i]);
			i++;
		}
		printf("fin\n");
	}
	else if (args[1][0] == '$')
	{
		i = 0;
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
