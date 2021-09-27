#include "minishell.h"

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
	ft_putstr_fd(msg, 1);
	ft_putstr_fd(" $ ", 1);
}

void get_cmd(char **cmd)
{
	int i;

	i = 0;
	get_next_line(1, cmd);
}

char	*get_path(char **strs)
{
	if (!strs[1][1])
		return ("");
	else
		return (strs[1] + 1);
}

int cmd_cd(char *cmd, char **envp)
{
	char **strs;

	strs = ft_split(cmd, ' ');
	if (strs[1][0] == '~')
	{
		if ((chdir(ft_strjoin(get_env(envp, "HOME"), get_path(strs)))) == -1)
			ft_putstr_fd("cd fail\n", 2);
		return (0);
	}
	if ((chdir(strs[1])) == -1)
		ft_putstr_fd("cd fail\n", 2);
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

int cmd_echo(char *cmd)
{
	int n_flag;
	char **command;
	int i;

	i = 0;
	command = ft_split(cmd, ' ');
	if (!command[0])
	{
		ft_putchar_fd('\n', 1);
		return (0);
	}
	n_flag = 0;
	if (command[0][0] == '-' && command[0][1] == 'n' && command[0][2] == '\0')
		n_flag = 1;
	if (n_flag)
		i++;
	while (command[i])
	{
		print_echo(command, i);
		if (!n_flag && !command[i + 1])
			ft_putchar_fd('\n', 1);
		i++;
	}
	return (0);
}

int run_cmd(char *cmd, char **envp)
{
	char **path;
	char **strs;
	char *tmp;
	pid_t pid;
	int i;

	if (!(ft_strcmp(cmd, "exit")))
		return (-1);
	if (!(ft_strncmp(cmd, "cd", 2)))
		return (cmd_cd(cmd, envp));
	if (!(ft_strncmp(cmd, "echo", 4)))
		return (cmd_echo(cmd + 5));
	pid = fork();
	i = 0;
	if (pid == 0)
	{
		path = ft_split(get_env(envp, "PATH"), ':');
		strs = ft_split(cmd, ' ');
		if (!ft_strncmp(*strs, "/bin/", 5))
			*strs = *strs + 5;
		while (path[i])
		{
			tmp = ft_strjoin(path[i], "/");
			path[i] = tmp;
			tmp = ft_strjoin(path[i], *strs);
			execve(tmp, strs, envp);
			if (errno != ENOENT)
				perror("pipex:");
			free(tmp);
			i++;
		}
		ft_putstr_fd("pipex: command not found : ", STDERR_FILENO);
		ft_putstr_fd(cmd, STDERR_FILENO);
		write(STDERR_FILENO, "\n", 1);
		return (-1);
	}
	wait(&pid);
	return (0);
}

int main(int ac, char **av, char **envp)
{
	char *cmd;
	int i;

	i = 0;
	(void)av;
	(void)ac;
	while (1)
	{
		terminal_msg();
		get_cmd(&cmd);
		i = run_cmd(cmd, envp);
		if (i == -1)
			break ;
	}
}