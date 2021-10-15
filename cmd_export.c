/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_export.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sangcpar <sangcpar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/15 16:47:23 by sangcpar          #+#    #+#             */
/*   Updated: 2021/10/15 16:47:24 by sangcpar         ###   ########.fr       */
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
	desen_envp = (char **)malloc(sizeof(char *) * i);
	i = 0;
	while (envp[i])
	{
		desen_envp[i] = envp[i];
		i++;
	}
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
	return (desen_envp);
}

char	**cmd_export(char **args, char **envp)
{
	int	i;
	//char **desen_envp;

	//desen_envp = desending_envp(envp);
	if (!(ft_strcmp(args[0], "export")))
	{
		i = 0;
		while (envp[i])
		{
			printf("declare -x %s\n", envp[i]);
			i++;
		}
	}
	else if (args[1][0] == '$')
	{
		i = 0;
		while (envp[i])
		{
			printf("declare -x %s\n", envp[i]);
			i++;
		}
	}
	else
		envp = plus_line(envp, args[1]);
	return (envp);
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
