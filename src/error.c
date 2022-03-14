/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ebellon <ebellon@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/12 16:48:04 by ebellon           #+#    #+#             */
/*   Updated: 2022/03/14 20:36:02 by ebellon          ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	ft_strncmp(const char *s1, const char *s2, size_t n)
{
	size_t	i;
	size_t	diff;

	if (!s1 || !s2)
		return (1);
	i = 0;
	while (s1[i] && i < n - 1)
	{
		diff = (unsigned char)s1[i] - (unsigned char)s2[i];
		if (diff)
			return (diff);
		i++;
	}
	return (diff);
}

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
