/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bi_echo.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ebellon <ebellon@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/11 12:17:31 by agardet           #+#    #+#             */
/*   Updated: 2022/03/12 20:19:46 by ebellon          ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	check_option_n(char *arg)
{
	size_t	i;

	i = 0;
	while (arg[i])
	{
		if (arg[i] != 'n')
			return (0);
		i++;
	}
	return (1);
}

void	bi_echo(char **av, int fd)
{
	size_t	i;
	bool	option;

	g_wstatus = EXIT_SUCCESS;
	i = 0;
	option = 0;
	if (av[1])
	{
		while (av[++i] && av[i][0] == '-')
		{
			if (check_option_n(av[i] + 1))
				option = 1;
			else
				break ;
		}
		while (av[i])
		{
			ft_putstr_fd(av[i], fd);
			if (av[++i])
				ft_putstr_fd(" ", fd);
		}
	}
	if (!option)
		ft_putstr_fd("\n", fd);
}
