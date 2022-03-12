/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bi_exit.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ebellon <ebellon@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/11 18:25:45 by agardet           #+#    #+#             */
/*   Updated: 2022/03/12 17:44:23 by ebellon          ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	skip_spaces(char **line)
{
	while (**line == ' ')
		(*line)++;
}

int	ft_isdigit(int c)
{
	return ('0' <= c && c <= '9');
}

int	ft_atoi(const char *str)
{
	int	nb;
	int	sign;

	nb = 0;
	sign = 1;
	if (*str == '-' || *str == '+')
	{
		if (*str == '-')
			sign = -1;
		str++;
	}
	while (ft_isdigit(*str))
	{
		nb = nb * 10 + (*str - 48);
		str++;
	}
	return (nb * sign);
}

char	*ft_strchrstr(const char *s1, const char *s2)
{
	size_t	i;
	size_t	j;

	i = 0;
	while (s1[i])
	{
		j = 0;
		while (s2[j])
		{
			if (s1[i] == s2[j])
				return ((char *)&(s1[i]));
			j++;
		}
		i++;
	}
	return (NULL);
}

static int	is_arg_numeric(char *arg)
{
	skip_spaces(&arg);
	if (*arg == '-' || *arg == '+' || ft_isdigit(*arg))
		arg++;
	while (ft_isdigit(*arg))
		arg++;
	skip_spaces(&arg);
	if (*arg)
		return (0);
	else
		return (1);
}

void	bi_exit(t_shell *shell, char **av)
{
	if (shell->n_cmd == 1)
		ft_putstr_fd("exit\n", STDERR_FILENO);
	if (av[1] && !is_arg_numeric(av[1]))
	{
		print_error("exit: ", av[1],
			"numeric argument required\n", 255);
	}
	else if (av[1] && av[2])
	{
		print_error("exit: ", NULL, "too many arguments\n", EXIT_FAILURE);
		return ;
	}
	else if (av[1])
		g_wstatus = ft_atoi(ft_strchrstr(av[1], "-0123456789"));
	exit(g_wstatus);
}
