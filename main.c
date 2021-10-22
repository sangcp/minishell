#include "minishell.h"

//===========****===========****===========**//
char **put_evs(t_shell *mini, char **envp)
{
    int i;
	//int len;
	char **ret;

    /*i = 0;
    while (envp[i])
        i++;
    mini->evs = (char**)malloc(sizeof(char *) * i);
	mini->c_evs = (char**)malloc(sizeof(char *) * i);
    i= 0;
    while (envp[i])
    {
        len = ft_strlen(envp[i]);
		//mini->evs[i] = (char *)malloc(sizeof(char) * len);
		mini->evs[i] = ft_strdup(envp[i]);
		//mini->c_evs[i] = (char *)malloc(sizeof(char) * len);
        mini->c_evs[i] = ft_strdup(envp[i]);
        i++;
    }*/
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

char	*get_env(char **envp, char *option)
{
	int		i;
	int		j;
	char	find[30];

	i = 0;
	ft_strlcpy(find, option, 30);
	while (envp[i])
	{
		j = 0;
		while (envp[i][j] != '=')
			j++;
		if (ft_strncmp(find, envp[i], j) == 0)
			return (envp[i] + j + 1);
		++i;
	}
	return (NULL);
}
/*
void	terminal_msg(void)
{
	char	buf[100];
	char	*msg;

	msg = getcwd(buf, 100);
	ft_putstr_fd("minishell$ ", 1);
}
*/
void	exit_shell(void)
{
	write(1, "\n", 1);
	exit(0);
}

char	*get_cmd(void)
{
	int		i;
	char	*cmd;

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

char	*get_path(char **strs)
{
	if (!strs[1][1])
		return ("");
	else
		return (strs[1] + 1);
}

int	cmd_cd(char **args, char **envp)
{
	if (args[1][0] == '~')
	{
		if ((chdir(ft_strjoin(get_env(envp, "HOME"), get_path(args)))) == -1)
			ft_putstr_fd("cd fail\n", 2);
		return (0);
	}
	if ((chdir(args[1])) == -1)
		ft_putstr_fd("cd fail\n", 2);
	return (0);
}

void	print_echo(char **str, int i)
{
	int	starts_qu;
	int	ends_qu;
	int	len;
	int j;

	j = 0;
	if (!str[i])
		return ;
	starts_qu = is_quotes(str[i][0]);
	len = (int)ft_strlen(str[i]);
	ends_qu = is_quotes(str[i][len - 1]);
	if (ends_qu && starts_qu)
		ft_putnstr(str[i] + 1, -1);
	else if (ends_qu)
		ft_putnstr(str[i], -1);
	else if (starts_qu)
		ft_putstr_fd(str[i] + 1, 1);
	else
	{
		//ft_putstr_fd(str[i], 1);
		while (str[i][j])
		{
			if (str[i][j] != '\'' && str[i][j] != '\"')
				ft_putchar_fd(str[i][j], 1);
			j++;
		}
	}
}

int	cmd_echo(t_shell *mini, t_list *list, char **args, char **envp)
{
	(void)mini;
	//char *tmp;
	int	n_flag;
	int	i;
	int j;

	(void)envp;
	i = 1;
	j = 0;
	if (!args[1])
	{
		ft_putchar_fd('\n', 1);
		return (0);
	}
	n_flag = 0;
	/*if (args[1][0] == '$')
		return (print_export(args[1], envp));*/
	if (args[1][0] == '-' && args[1][1] == 'n' && args[1][2] == '\0')
		n_flag = 1;
	if (n_flag)
		i++;
	while (args[i])
	{
		/*if (args[i][0] == '$')
		{
			if (!args[i][1])
			{
				ft_putstr_fd("$", 1);
				break ;
			}
			tmp = get_env(mini->c_evs, args[i] + 1);
			if (!tmp)
				break ;
			ft_putstr_fd(tmp, 1);
		}
		else*/
		print_echo(args, i);
		if (args[i + 1] && ((t_ops*)(list->content))->q_chk[i] != '0')
			ft_putchar_fd(' ', 1);
		i++;
		j++;
	}
	if (!n_flag)
			ft_putchar_fd('\n', 1);
	/*while (args[i])
	{
		print_echo(args, i);
		if (!n_flag && !args[i + 1])
			ft_putchar_fd('\n', 1);
		i++;
	}*/
	return (0);
}

//==============*====//

void	reset_fds(t_shell *mini)
{
	dup2(mini->fds[0], 0);
	dup2(mini->fds[1], 1);
	dup2(mini->stdinp, 0);
	dup2(mini->stdout, 1);
}


char	**q_del(t_shell *mini, t_list *list, char **args)
{
	char **tmp;
	char	*line;
	int i;
	int j;
	//int q_chk;

	i = 0;
	while (args[i])
		i++;
	tmp = (char **)malloc(sizeof(char*) * (i + 1));
	((t_ops*)(list->content))->q_chk = (char *)malloc(sizeof(char) * (i + 1));
	((t_ops*)(list->content))->q_chk[i] = '\0';
	line = ((t_ops*)(list->content))->operation;
	i = 0;
	while (args[i])
	{
		j = 0;
		while (ft_strncmp(line, args[i], ft_strlen(args[i])))
			line++;
		if ((line[ft_strlen(args[i]) - 1] == '\"' || line[ft_strlen(args[i]) - 1] == '\'')\
		&& line[ft_strlen(args[i])] != ' ')
			((t_ops*)(list->content))->q_chk[i] = '0';
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
		/*printf("a = (%s)", args[i]);
		printf(" || t = (%s)\n", tmp[i]);*/
		i++;
	}
	tmp[i] = NULL;
	path_free(args);
	return (tmp);
}

void q_chk(t_shell *mini, t_list *list)
{
	t_list *tlist;

	if (!list)
		return ;
	tlist = list;
	while (tlist->next)
	{
		((t_ops*)(tlist->content))->args = q_del(mini, tlist, ((t_ops*)(tlist->content))->args);
		tlist = tlist->next;
	}
	((t_ops*)(tlist->content))->args = q_del(mini, tlist, ((t_ops*)(tlist->content))->args);
}

int	main(int ac, char **av, char **envp)
{
	char	*cmd;
	int		i;
	t_shell	mini;
	t_list	*list;

	mini.fds[0] = dup(STDIN_FILENO);
	mini.fds[1] = dup(STDOUT_FILENO);
	(void)av;
	(void)ac;
	//put_evs(&mini, envp);
	mini.evs = put_evs(&mini, envp);
	mini.c_evs = put_evs(&mini, envp);
	init_term(&mini);
	while (1)
	{
		i = 0;
		//terminal_msg();
		signal(SIGINT, &sighandler1);
		//signal(SIGQUIT, &pipe_sighandler1);
		signal(SIGQUIT, SIG_IGN);
		tcsetattr(STDIN_FILENO, TCSANOW, &mini.term);
		cmd = get_cmd();
		if (*cmd)
			add_history(rl_line_buffer);
		list = parse_option(cmd);
		q_chk(&mini, list);
		mini.prev_pipe = STDIN_FILENO;
		mini.count = ft_lstsize(list);
		restore_term(&mini);
		/*while (((t_ops *)(list->content))->args[i])
			printf("(%s)\n", ((t_ops *)(list->content))->args[i++]);*/
		i = run_cmd1(&mini, list, envp);
		free_all(&mini, list);
		reset_fds(&mini);
		free(cmd);
		if (i == -1)
			break ;
	}
}


//뇌를 자극하는윈도우즈 시스템 프로그래밍
