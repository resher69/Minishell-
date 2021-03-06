/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signal.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ebellon <ebellon@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/12 17:17:56 by ebellon           #+#    #+#             */
/*   Updated: 2022/03/12 17:18:12 by ebellon          ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	sig_int(int sig)
{
	(void)sig;
	rl_replace_line("", 0);
	ft_putstr_fd("\n", STDERR_FILENO);
	rl_on_new_line();
	rl_redisplay();
	g_wstatus = 1;
}

void	heredoc_sig_int(int sig)
{
	(void)sig;
	ft_putstr_fd("\n", STDERR_FILENO);
	g_wstatus = 1;
	exit(g_wstatus);
}

void	sig_child(int sig)
{
	if (sig == SIGINT)
	{
		ft_putstr_fd("\n", STDERR_FILENO);
		g_wstatus = 130;
	}
	else if (sig == SIGQUIT)
	{
		ft_putstr_fd("Quit: 3\n", STDERR_FILENO);
		g_wstatus = 131;
	}
}
