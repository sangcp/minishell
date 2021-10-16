#include "minishell.h"

//===========****===========****===========**//
void put_evs(t_shell *mini, char **envp)
{
    int i;
    int len;
    i = 0;
    len = 0;
    while (envp[i])
        i++;
    mini->evs = (char**)malloc(sizeof(char *) * i);
	mini->c_evs = (char**)malloc(sizeof(char *) * i);
    i= 0;
    while (envp[i])
    {
        len = ft_strlen(envp[i]);
        mini->evs[i] = (char *)malloc(sizeof(char) * len);
        mini->evs[i] = envp[i];
		mini->c_evs[i] = (char *)malloc(sizeof(char) * len);
        mini->c_evs[i] = envp[i];
        i++;
    }
}

char	*get_env(char **envp, char *option)
{
	int		i;
	int		j;
	char	find[5];

	i = 0;
	ft_strlcpy(find, option, 5);
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

void	terminal_msg(void)
{
	char	buf[100];
	char	*msg;

	msg = getcwd(buf, 100);
	ft_putstr_fd("minishell$ ", 1);
}

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
		ft_putstr_fd(str[i], 1);
}

int	cmd_echo(char **args, char **envp)
{
	char *tmp;
	int	n_flag;
	int	i;
	int j;

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
		if (args[i][0] == '$')
		{
			tmp = get_env(envp, args[i] + 1);
			ft_putstr_fd(tmp, 1);
		}
		else
			print_echo(args, i);
		if (args[i + 1])
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
	(void)envp;
	put_evs(&mini, envp);
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
		mini.prev_pipe = STDIN_FILENO;
		mini.count = ft_lstsize(list);
		restore_term(&mini);
		while (((t_ops *)(list->content))->args[i])
			printf("(%s)\n", ((t_ops *)(list->content))->args[i++]);
		i = run_cmd1(&mini, list, envp);
		free_all(&mini, list);
		reset_fds(&mini);
		free(cmd);
		if (i == -1)
			break ;
	}
}
