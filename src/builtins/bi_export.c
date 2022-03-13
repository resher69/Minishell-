/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bi_export.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agardet <agardet@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/12 17:29:07 by ebellon           #+#    #+#             */
/*   Updated: 2022/03/13 18:02:51 by agardet          ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	index_list(t_shell *s)
{
	t_env_var	*elem;
	t_env_var	*next;

	elem = s->env;
	while (elem)
	{
		elem->index = 1;
		next = s->env;
		while (next)
		{
			if (ft_strcmp(elem->name, next->name) > 0)
				elem->index++;
			next = next->next;
		}
		elem = elem->next;
	}
}

void	exec_export(int fd, size_t i, t_env_var *export)
{
	while (export)
	{
		if (i == export->index)
		{
			ft_putstr_fd("declare -x ", fd);
			ft_putstr_fd(export->name, fd);
			if (export->value)
			{
				ft_putstr_fd("=\"", fd);
				ft_putstr_fd(export->value + 1, fd);
				ft_putstr_fd("\"", fd);
			}
			ft_putstr_fd("\n", fd);
		}
		export = export->next;
	}
}

static void	print_export(t_shell *s, int fd)
{
	size_t		i;
	t_env_var	*export;
	size_t		env_size;

	i = 1;
	env_size = 0;
	export = s->env;
	while (export)
	{
		export = export->next;
		env_size++;
	}
	while (i <= env_size)
	{
		export = s->env;
		exec_export(fd, i, export);
		i++;
	}
}

void	bi_export(t_shell *s, char **av, int fd)
{
	g_wstatus = EXIT_SUCCESS;
	if (av[1])
		create_env_variable(s, av);
	else
	{
		index_list(s);
		print_export(s, fd);
	}
}
