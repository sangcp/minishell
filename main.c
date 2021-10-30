#include "minishell.h"

char	*get_cmd(int ac, char **av)
{
	int		i;
	char	*cmd;

	(void)ac;
	(void)av;
	i = 0;
	cmd = readline("minishell$ ");
	if (!cmd)
	{
		printf("\x1b[2A");
		printf("\x1b[11C");
		printf("exit");
		exit_shell();
	}
	return (cmd);
}

int	env_chk(char *tmp)
{
	int	j;

	j = -1;
	while (tmp[++j])
		if (tmp[j] == '$')
			return (1);
	return (0);
}
int	env_len(char *env)
{
	int i;

	i = 0;
	if (env[i] == '$')
		i++;
	while (env[i] && ft_isalnum(env[i]))
		i++;
	if (env[i])
		i--;
	return (i);
}

char	*red_join(char **s1, char *s2)
{
	size_t	i;
	size_t	l;
	char	*a;

	a = (char *)malloc(ft_strlen(*s1) + 2);
	if (!a)
		return (NULL);
	i = -1;
	l = 0;
	while ((*s1)[++i])
		a[i] = (*s1)[i];
	if (s2[l])
		a[i++] = s2[l++];
	a[i] = '\0';
	free(*s1);
	return (a);
}

void	env_change(t_shell *mini, char **tmp, int	i, int qq)
{
	int		j;
	char	*tm;
	char	*tm2;
	char	*sub;
	int		chk;

	j = 0;
	chk = 0;
	tm2 = NULL;
	tm = ft_strdup("");
	while (tmp[i][j] && qq == 0 && env_chk(&tmp[i][j]))
	{
		chk = 1;
		while (tmp[i][j] != '$')
			tm = red_join(&tm, tmp[i] + j++);
		sub = ft_substr(tmp[i], j + 1, env_len(tmp[i] + j));
		tm = ft_strjoin(tm, get_env(mini->c_evs, sub), 1);
		if (sub)
			free(sub);
		j += env_len(tmp[i] + j);
		if (tmp[i][j])
			j++;
		if (tm2)
			free(tm2);
		tm2 = ft_strjoin(tm, (tmp[i] + j), 4);
	}
	if (chk)
	{
		free(tmp[i]);
		tmp[i] = ft_strdup(tm2);
		free(tm2);
	}
	free(tm);
}

void	q_d(t_shell *mini, char **args, char **tmp, int i)
{
	//char	*tm;
	int		qq;
	/*int		j;
	char	*tm2 = NULL;
	char	*sub;
	int		chk = 0;*/

	/*if (args[i][0] == '\"' && args[i][1] == '$')
		tmp[i] = ft_strdup(get_env(mini->c_evs, args[i] + 2));*/
	qq = 0;
	if (args[i][0] == '\'')
		qq = 1;
	if (args[i][0] == '\"' && args[i][1] == '$')
		tmp[i] = ft_substr(args[i], 1, (ft_strlen(args[i]) - 2));
	else if (args[i][0] == '\'' && args[i][1] == '$')
		tmp[i] = ft_substr(args[i], 1, (ft_strlen(args[i]) - 2));
	else if (args[i][0] == '\'' || args[i][0] == '\"')
		tmp[i] = ft_substr(args[i], 1, (ft_strlen(args[i]) - 2));
	else
		tmp[i] = ft_strdup(args[i]);
	/*if ((args[i][0] == '$' && args[i][1]) || (tmp[i][0] == '$' && tmp[i][1] && !qq))
	{
		if (args[i][0] == '$')
			tm = ft_strdup(get_env(mini->c_evs, args[i] + 1));
		else
			tm = ft_strdup(get_env(mini->c_evs, tmp[i] + 1));
		if (tm)
		{
			free(tmp[i]);
			tmp[i] = tm;
		}
	}*/
	env_change(mini, tmp, i, qq);
	/*j = 0;
	tm = ft_strdup("");
	while (tmp[i][j] && qq == 0 && env_chk(&tmp[i][j]))
	{
		chk = 1;
		while (tmp[i][j] != '$')
			tm = red_join(&tm, tmp[i] + j++);
		sub = ft_substr(tmp[i], j + 1, env_len(tmp[i] + j));
		tm = ft_strjoin(tm, get_env(mini->c_evs, sub), 1);
		if (sub)
			free(sub);
		j += env_len(tmp[i] + j);
		if (tmp[i][j])
			j++;
		if (tm2)
			free(tm2);
		tm2 = ft_strjoin(tm, (tmp[i] + j), 4);
	}
	if (chk)
	{
		free(tmp[i]);
		tmp[i] = ft_strdup(tm2);
		free(tm2);
	}
	free(tm);*/
}

char	**q_del(t_shell *mini, t_list *list, char **args)
{
	char	**tmp;
	char	*line;
	int		i;

	i = 0;
	while (args[i])
		i++;
	tmp = (char **)malloc(sizeof(char *) * (i + 1));
	((t_ops *)(list->content))->q_chk = (char *)malloc(sizeof(char) * (i + 1));
	((t_ops *)(list->content))->q_chk[i] = '\0';
	line = ((t_ops *)(list->content))->operation;
	i = -1;
	while (args[++i])
	{
		while (ft_strncmp(line, args[i], ft_strlen(args[i])))
			line++;
		/*if ((line[ft_strlen(args[i]) - 1] == '\"' || \
		line[ft_strlen(args[i]) - 1] == '\'') \
		&& line[ft_strlen(args[i])] != ' ')*/
		if ((line[ft_strlen(args[i]) - 1] || \
		line[ft_strlen(args[i]) - 1] ) \
		&& line[ft_strlen(args[i])] != ' ')
			((t_ops *)(list->content))->q_chk[i] = '0';
		q_d(mini, args, tmp, i);
		printf("tmp=(%s)\n", tmp[i]);
		line += ft_strlen(args[i]);
	}
	tmp[i] = NULL;
	path_free(args);
	return (tmp);
}

int	q_chk(t_shell *mini, t_list *list)
{
	t_list	*tlist;

	if (!list)
		return (-1);
	tlist = list;
	while (tlist)
	{
		((t_ops *)(tlist->content))->args = q_del(mini, tlist, \
		((t_ops *)(tlist->content))->args);
		tlist = tlist->next;
	}
	return (0);
}

int	main(int ac, char **av, char **envp)
{
	char	*cmd;
	int		i;
	t_shell	mini;
	t_list	*list;

	reset_fds(&mini);
	init_term_env(&mini, envp);
	while (1)
	{
		signal(SIGINT, &sighandler1);
		signal(SIGQUIT, SIG_IGN);
		tcsetattr(STDIN_FILENO, TCSANOW, &mini.term);
		cmd = get_cmd(ac, av);
		if (*cmd)
			add_history(rl_line_buffer);
		list = parse_option(&cmd);
		i = q_chk(&mini, list);
		mini_c_p(&mini, list);
		restore_term(&mini);
		i = run_cmd1(&mini, list);
		free_all(&mini, list, &cmd);
		reset_fds(&mini);
		if (i == -1)
			break ;
	}
}
