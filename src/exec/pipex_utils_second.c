/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex_utils_second.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ebellon <ebellon@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/13 18:06:17 by ebellon           #+#    #+#             */
/*   Updated: 2022/03/13 18:06:40 by ebellon          ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	ft_safe_close(int fd)
{
	if (close(fd) < 0)
	{
		print_error("close: ", NULL, NULL, errno);
		return (1);
	}
	return (0);
}

void	ft_free_job_exit(char **cmd_arg, char *exec_path,
						char **locations, int err)
{
	ft_free_tab(cmd_arg);
	free(exec_path);
	if (err > 0)
	{
		ft_free_tab(locations);
		exit (g_wstatus);
	}
}

void	ft_waitpids(t_shell *shell)
{
	size_t	i;
	int		wstatus;
	int		sig;

	i = 0;
	sig = 0;
	while (i < shell->i)
	{
		waitpid(shell->pid_ar[i++], &wstatus, 0);
		if (WIFEXITED(wstatus))
			g_wstatus = WEXITSTATUS(wstatus);
		if (WIFSIGNALED(wstatus) && !sig)
		{
			sig_child(WTERMSIG(wstatus));
			sig = 1;
		}
	}
}
