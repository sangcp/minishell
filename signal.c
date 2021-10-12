#include "minishell.h"

void pipe_sighandler1(int sig)
{
	(void)sig;
	rl_on_new_line();
	rl_redisplay();
	//ft_putstr_fd("\x1b[10C", 1);
	/*if (sig == SIGQUIT)
		ft_putstr_fd("", 1);*/
}

void pipe_sighandler2(int sig)
{
	if (sig == SIGQUIT)
	{
		ft_putstr_fd("Quit: 3\n", 1);
		signal(SIGQUIT, &sighandler2);
	}
}

void sighandler1(int sig)
{
	if (sig == SIGINT)
	{
		//ft_putstr_fd("\x1b[10C", 1);
		ft_putstr_fd("\n", 1); //\x1b1A 15C
        //ft_putstr_fd("bash-3.2$ ", 1);
		//terminal_msg();
		rl_on_new_line();
		rl_replace_line("", 0);
		rl_redisplay();
		//signal(SIGINT, &sighandler1);
	}
}

void sighandler2(int sig)
{
	if (sig == SIGINT)
	{
		ft_putchar_fd('\n', 1);
		signal(SIGINT, &sighandler2);
	}
}
