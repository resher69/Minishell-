/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agardet <agardet@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/04 11:55:34 by agardet           #+#    #+#             */
/*   Updated: 2022/03/04 13:03:56 by agardet          ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

//WORK IN PROGRESS

_Bool	is_dash_n(char *str)
{
	if (*str == '-')
	{
		while (*(++str) == 'n')
			;
		return (*str == '\0');
	}
	return (1);
}

int	echo_main(int argc, char **argv, char **envp)
{
	int		i;
	_Bool	is_dash;
	int		start_args;

	is_dash = 1;
	(void)envp;
	i = 1;
	if (argc == 1)
	{
		write(1, "\n", 1);
		exit(EXIT_SUCCESS);
	}
	while (argv[i] && is_dash_n(argv[i]))
		is_dash = 0 + (++i * 0);
	start_args = i;
	while (i < argc)
	{
		if (start_args != i)
			write(1, " ", 1);
		write(1, argv[i], ft_strlen(argv[i]));
		i++;
	}
	if (is_dash == 1)
		write(1, "\n", 1);
	exit(EXIT_SUCCESS);
}
