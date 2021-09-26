#include "minishell.h"

char		*get_path(char **envp)
{
	int		i;
	int		j;
	char	find[5];

	i = 0;
	ft_strlcpy(find, "PATH", 5);
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

int cmd_cd(char *cmd)
{
	char **strs;

	strs = ft_split(cmd, ' ');
	chdir(strs[1]);
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
		return (cmd_cd(cmd));
	pid = fork();
	i = 0;
	if (pid == 0)
	{
		path = ft_split(get_path(envp), ':');
		strs = ft_split(cmd, ' ');
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