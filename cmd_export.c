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

void	envp_dup(char **envp, char **env, char *new_env, int j)
{
	int	i;
	int	k;

	i = -1;
	k = 0;
	while (env[++i])
		if (i != j)
			envp[k++] = ft_strdup(env[i]);
	envp[k] = ft_strdup(new_env);
	envp[++k] = NULL;
}

char	**plus_line(char **env, char *new_env)
{
	char	**envp;
	char	*tmp;
	int		i;
	int		j;

	i = 0;
	j = -1;
	while (new_env[i] && new_env[i] != '=')
		i++;
	tmp = ft_substr(new_env, 0, i);
	i = -1;
	while (env[++i])
		if (!ft_strncmp(tmp, env[i], ft_strlen(tmp)))
			j = i;
	if (j != -1)
		i--;
	envp = (char **)malloc(sizeof(char *) * (i + 2));
	envp_dup(envp, env, new_env, j);
	path_free(env);
	free(tmp);
	return (envp);
}

char	**desending_envp(char **envp)
{
	char	**desen_envp;
	int		i;
	int		j;

	i = 0;
	while (envp[i])
		i++;
	desen_envp = (char **)malloc(sizeof(char *) * (i + 1));
	i = -1;
	while (envp[++i])
		desen_envp[i] = ft_strdup(envp[i]);
	desen_envp[i] = NULL;
	i = -1;
	while (desen_envp[++i])
	{
		j = i;
		while (desen_envp[++j])
		{
			if (swap_envp(desen_envp[i], desen_envp[j]))
				swap_cha(desen_envp, i, j);
		}
	}
	path_free(envp);
	return (desen_envp);
}

void	print_ex(t_shell *mini)
{
	int	i;
	int	j;
	int	q_flag;

	i = -1;
	while (mini->c_evs[++i])
	{
		j = -1;
		q_flag = 0;
		printf("declare -x ");
		while (mini->c_evs[i][++j])
		{
			printf("%c", mini->c_evs[i][j]);
			if (mini->c_evs[i][j] == '=')
			{
				printf("\"");
				q_flag = 1;
			}
		}
		if (q_flag)
			printf("\"");
		printf("\n");
	}
}

void	cmd_export(t_shell *mini, char **args)
{
	int	i;

	mini->c_evs = desending_envp(mini->c_evs);
	if (!(ft_strcmp(args[0], "export")) && !args[1])
		print_ex(mini);
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
