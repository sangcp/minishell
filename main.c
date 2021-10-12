#include "minishell.h"


//===========****===========****===========****===========****===========****===========****===========****===========****===========****===========****===========****//

char		*get_env(char **envp, char *option)
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

void terminal_msg()
{
	char buf[100];
	char *msg;

	msg = getcwd(buf, 100);
	//ft_putstr_fd(msg, 1);
	ft_putstr_fd("bash-3.2$", 1);
}

void	exit_shell(void)
{
	write(1, "\n", 1);
	exit(0);
}

char *get_cmd()
{
	int i;
	char *cmd;

	i = 0;
	//cmd = NULL;
	/*if (get_next_line(1, &cmd) == -1)
	{
		ft_putstr_fd("\b   \b\bexit", 1);
		exit_shell();
	}*/
	cmd = readline("bash-3.2$ ");
	if (!cmd)
	{
		printf("\x1b[2A");
		printf("\x1b[10C");
		printf("exit"); //\x1b1A 15C
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

int cmd_cd(char **args, char **envp)
{
	if (args[1][0] == '~')
	{
		if ((chdir(ft_strjoin(get_env(envp, "HOME"), get_path(args)))) == -1)
			ft_putstr_fd("cd fail\n", 2);
		return (0);
	}
	if ((chdir(args[1])) == -1)
		ft_putstr_fd("cd fail\n", 2);
	path_free(args);
	return (0);
}

void		print_echo(char **str, int i)
{
	int		starts_qu;
	int		ends_qu;
	int		len;

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
	if (str[i + 1])
		ft_putchar_fd(' ', 1);
}

int cmd_echo(char **args, char **envp)
{
	int n_flag;
	int i;

	i = 1;
	if (!args[1])
	{
		ft_putchar_fd('\n', 1);
		return (0);
	}
	n_flag = 0;
    if (args[1][0] == '$')
        return(print_export(args[1], envp));
	if (args[1][0] == '-' && args[1][1] == 'n' && args[1][2] == '\0')
		n_flag = 1;
	if (n_flag)
		i++;
	while (args[i])
	{
		print_echo(args, i);
		if (!n_flag && !args[i + 1])
			ft_putchar_fd('\n', 1);
		i++;
	}
	return (0);
}
//==============*==============*==============*==============*==============*==============*==============*==============*==============*//

//==============*==============*==============*==============*==============*==============*==============*==============*==============*//

int run_cmd(char *cmd, char ***envp)// t_shell *mini, t_list *list)
{
	char **path;
	char **strs;
	char *tmp;
	pid_t pid;
	int i;

	if (!(ft_strcmp(cmd, "exit")))
		return (-1);
	if (!(ft_strncmp(cmd, "cd", 2)))
		return (cmd_cd(&cmd, *envp));
	if (!(ft_strncmp(cmd, "echo", 4)))
		return (cmd_echo((&cmd), *envp));
    if (!(ft_strncmp(cmd, "export", 5)))
    {
        *envp = cmd_export(&cmd, *envp);
        return (0);
    }
    if (!(ft_strncmp(cmd, "env", 3)))
        return (cmd_env(&cmd, *envp));
	if (ft_strchr(cmd, '|'))
		return (cmd_pipe(cmd, *envp));
	pid = fork();
	signal(SIGINT, sighandler2);
	signal(SIGQUIT, pipe_sighandler2);
	i = 0;
	if (pid == 0)
	{
		path = ft_split2(get_env(*envp, "PATH"), ':');
		strs = ft_split2(cmd, ' ');
		if (!ft_strncmp(*strs, "/bin/", 5))
			*strs = *strs + 5;
		while (path[i])
		{
			tmp = ft_strjoin(path[i], "/");
			path[i] = tmp;
			tmp = ft_strjoin(path[i], *strs);
			execve(tmp, strs, *envp);
			if (errno != ENOENT)
				perror("pipex:");
			free(tmp);
			i++;
		}
		ft_putstr_fd("pipex: command not found : ", STDERR_FILENO);
		ft_putstr_fd(cmd, STDERR_FILENO);
		write(STDERR_FILENO, "\n", 1);
		path_free(path);
		path_free(strs);
		return (-1);
	}
	wait(&pid);
	return (0);
}

void	reset_fds(t_shell *mini)
{
	dup2(mini->fds[0], 0);
	dup2(mini->fds[1], 1);
	dup2(mini->stdinp, 0);
	dup2(mini->stdout, 1);
}

void	init_term(t_shell *mini)
{
	tcgetattr(STDIN_FILENO, &mini->t_sv);
	tcgetattr(STDIN_FILENO, &mini->term);
	mini->term.c_lflag &= ~ICANON;
	//mini->term.c_lflag &= ~ECHO;
	mini->term.c_cc[VMIN] = 1;
	mini->term.c_cc[VTIME] = 0;
	tcsetattr(STDIN_FILENO, TCSANOW, &mini->term);
	//tgetent(NULL, "xterm");
}

void	restore_term(t_shell *mini)
{
	tcsetattr(STDIN_FILENO, TCSANOW, &mini->t_sv);
}

int main(int ac, char **av, char **envp)
{
	char *cmd;
	int i;
	t_shell mini;
	t_list *list;

	i = 0;
	mini.fds[0] = dup(STDIN_FILENO);
	mini.fds[1] = dup(STDOUT_FILENO);
	(void)av;
	(void)ac;
	(void)envp;
	//init_term(&mini);
	while (1)
	{
		//terminal_msg();
		signal(SIGINT, &sighandler1);
		signal(SIGQUIT, &pipe_sighandler1);
		//tcsetattr(STDIN_FILENO, TCSANOW, &mini.term);
		cmd = get_cmd();
		list = parse_option(cmd);
		free(cmd);
		mini.prev_pipe = STDIN_FILENO;
		mini.count = ft_lstsize(list);
		//restore_term(&mini);
		i = run_cmd1(&mini, list, envp);
		reset_fds(&mini);
		add_history(cmd);
		if (i == -1)
			break ;
	}
}
