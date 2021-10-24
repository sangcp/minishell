#include "minishell.h"

//===========****===========****===========**//
char	**put_evs(t_shell *mini, char **envp)
{
	int		i;
	char	**ret;

	(void)mini;
	i = 0;
	while (envp[i])
		i++;
	ret = malloc((i + 1) * (sizeof(char *)));
	if (!ret)
		return (NULL);
	i = 0;
	while (envp[i])
	{
		ret[i] = ft_strdup(envp[i]);
		i++;
	}
	ret[i] = NULL;
	return (ret);
}

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
//==============*====//

char	**q_del(t_shell *mini, t_list *list, char **args)
{
	char	**tmp;
	char	*line;
	int		i;
	int		j;

	i = 0;
	while (args[i])
		i++;
	tmp = (char **)malloc(sizeof(char *) * (i + 1));
	((t_ops *)(list->content))->q_chk = (char *)malloc(sizeof(char) * (i + 1));
	((t_ops *)(list->content))->q_chk[i] = '\0';
	line = ((t_ops *)(list->content))->operation;
	i = 0;
	while (args[i])
	{
		j = 0;
		while (ft_strncmp(line, args[i], ft_strlen(args[i])))
			line++;
		if ((line[ft_strlen(args[i]) - 1] == '\"' || \
		line[ft_strlen(args[i]) - 1] == '\'') \
		&& line[ft_strlen(args[i])] != ' ')
			((t_ops *)(list->content))->q_chk[i] = '0';
		if (args[i][0] == '\"' && args[i][1] == '$')
			tmp[i] = ft_strdup(get_env(mini->c_evs, args[i] + 2));
		else if (args[i][0] == '\'' && args[i][1] == '$')
			tmp[i] = ft_substr(args[i], 1, (ft_strlen(args[i]) - 2));
		else if (args[i][0] == '\'' || args[i][0] == '\"')
			tmp[i] = ft_substr(args[i], 1, (ft_strlen(args[i]) - 2));
		else if (args[i][0] == '$' && args[i][1])
			tmp[i] = ft_strdup(get_env(mini->c_evs, args[i] + 1));
		else
			tmp[i] = ft_strdup(args[i]);
		i++;
	}
	tmp[i] = NULL;
	path_free(args);
	return (tmp);
}

void	q_chk(t_shell *mini, t_list *list)
{
	t_list	*tlist;

	if (!list)
		return ;
	tlist = list;
	while (tlist)
	{
		((t_ops *)(tlist->content))->args = q_del(mini, tlist, \
		((t_ops *)(tlist->content))->args);
		tlist = tlist->next;
	}
}

int	main(int ac, char **av, char **envp)
{
	char	*cmd;
	int		i;
	t_shell	mini;
	t_list	*list;

	mini.fds[0] = dup(STDIN_FILENO);
	mini.fds[1] = dup(STDOUT_FILENO);
	mini.evs = put_evs(&mini, envp);
	mini.c_evs = put_evs(&mini, envp);
	init_term(&mini);
	while (1)
	{
		signal(SIGINT, &sighandler1);
		signal(SIGQUIT, SIG_IGN);
		tcsetattr(STDIN_FILENO, TCSANOW, &mini.term);
		cmd = get_cmd(ac, av);
		if (*cmd)
			add_history(rl_line_buffer);
		list = parse_option(cmd);
		q_chk(&mini, list);
		mini.prev_pipe = STDIN_FILENO;
		mini.count = ft_lstsize(list);
		restore_term(&mini);
		i = run_cmd1(&mini, list, envp);
		free_all(&mini, list);
		reset_fds(&mini);
		free(cmd);
		if (i == -1)
			break ;
	}
}
