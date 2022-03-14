/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bi_env.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ebellon <ebellon@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/11 12:29:10 by agardet           #+#    #+#             */
/*   Updated: 2022/03/14 20:46:01 by ebellon          ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	ft_isalpha(int c)
{
	return (('a' <= c && c <= 'z') || ('A' <= c && c <= 'Z'));
}

int	ft_isalnum(int c)
{
	return (ft_isalpha(c) || ft_isdigit(c) || c == '=');
}

void	skip_spaces(char **line)
{
	while (**line == ' ')
		(*line)++;
}

int	ft_isdigit(int c)
{
	return ('0' <= c && c <= '9');
}

void	bi_env(t_shell *shell)
{
	t_env_var	*elem;

	g_wstatus = EXIT_SUCCESS;
	elem = shell->env;
	while (elem)
	{
		if (elem->value)
		{
			ft_putstr_fd(elem->name, 1);
			ft_putstr_fd(elem->value, 1);
			ft_putstr_fd("\n", 1);
		}
		elem = elem->next;
	}
}
