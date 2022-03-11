/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bi_echo.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agardet <agardet@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/11 12:17:31 by agardet           #+#    #+#             */
/*   Updated: 2022/03/11 12:17:34 by agardet          ###   ########lyon.fr   */
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

	//g_error_number = EXIT_SUCCESS;
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
				ft_putchar_fd(' ', fd);
		}
	}
	if (!option)
		ft_putchar_fd('\n', fd);
}