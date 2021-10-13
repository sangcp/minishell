/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   termios.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sangcpar <sangcpar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/14 06:43:25 by sangcpar          #+#    #+#             */
/*   Updated: 2021/10/14 06:43:26 by sangcpar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	init_term(t_shell *mini)
{
	tcgetattr(STDIN_FILENO, &mini->t_sv);
	tcgetattr(STDIN_FILENO, &mini->term);
	mini->term.c_lflag &= ~ICANON;
	//mini->term.c_lflag &= ~ECHO;
	mini->term.c_lflag &= ~ECHOCTL;
	mini->term.c_cc[VMIN] = 1;
	mini->term.c_cc[VTIME] = 0;
	tcsetattr(STDIN_FILENO, TCSANOW, &mini->term);
	//tgetent(NULL, "xterm");
}

void	restore_term(t_shell *mini)
{
	tcsetattr(STDIN_FILENO, TCSANOW, &mini->t_sv);
}
