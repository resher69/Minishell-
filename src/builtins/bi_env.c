/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bi_env.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ebellon <ebellon@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/11 12:29:10 by agardet           #+#    #+#             */
/*   Updated: 2022/03/11 16:18:59 by ebellon          ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	bi_env(t_shell *shell)
{
	t_env_var	*elem;

	//g_error_number = EXIT_SUCCESS;
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
