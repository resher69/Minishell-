/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ebellon <ebellon@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/12 16:48:04 by ebellon           #+#    #+#             */
/*   Updated: 2022/03/13 19:59:43 by ebellon          ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

size_t	check_pipe(t_cmd **cmd)
{
	size_t	k;

	k = 0;
	while (cmd && cmd[k])
	{
		if (cmd[k]->av[0])
			k++;
		else
			break ;
	}
	return (k);
}

void	print_error(char *cmd, char *value, char *error, int status)
{
	errno = status;
	g_wstatus = status;
	ft_putstr_fd("MiniSHEEEESH: ", STDERR_FILENO);
	if (cmd)
		ft_putstr_fd(cmd, STDERR_FILENO);
	if (value)
	{
		ft_putstr_fd(value, STDERR_FILENO);
		ft_putstr_fd(": ", STDERR_FILENO);
	}
	if (errno != 258 && errno != EXIT_FAILURE
		&& errno != 127 && errno != 255 && errno != 3)
		perror(NULL);
	if (error)
		ft_putstr_fd(error, STDERR_FILENO);
}
