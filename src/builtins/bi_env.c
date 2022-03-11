/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bi_env.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agardet <agardet@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/11 12:29:10 by agardet           #+#    #+#             */
/*   Updated: 2022/03/11 15:27:10 by agardet          ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	bi_env(t_shell *shell, int fd)
{
	t_env_var	*elem;

	//g_error_number = EXIT_SUCCESS;
	elem = shell->env;
	while (elem)
	{
		if (elem->value)
		{
			ft_putstr_fd(elem->name, fd);
			ft_putstr_fd(elem->value, fd);
			ft_putstr_fd("\n", fd);
		}
		elem = elem->next;
	}
}
