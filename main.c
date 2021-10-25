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

void	q_d(t_shell *mini, char **args, char **tmp, int i)
{
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
		if ((line[ft_strlen(args[i]) - 1] == '\"' || \
		line[ft_strlen(args[i]) - 1] == '\'') \
		&& line[ft_strlen(args[i])] != ' ')
			((t_ops *)(list->content))->q_chk[i] = '0';
		q_d(mini, args, tmp, i);
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
		list = parse_option(cmd);
		q_chk(&mini, list);
		mini_c_p(&mini, list);
		restore_term(&mini);
		i = run_cmd1(&mini, list);
		free_all(&mini, list, cmd);
		reset_fds(&mini);
		if (i == -1)
			break ;
	}
}
