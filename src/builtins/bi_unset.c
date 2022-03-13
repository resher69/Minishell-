/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bi_unset.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agardet <agardet@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/12 17:29:13 by ebellon           #+#    #+#             */
/*   Updated: 2022/03/13 18:03:36 by agardet          ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	bi_unset(t_shell *s, char **av)
{
	t_env_var	*unset;
	size_t		i;

	i = 1;
	g_wstatus = EXIT_SUCCESS;
	while (av[i])
	{
		if (!is_word(av[i]))
			print_error("unset: ", av[i],
				"not a valid identifier\n", EXIT_FAILURE);
		else
		{
			unset = find_env_var(s, av[i]);
			if (unset)
				env_del(s, unset);
		}
		i++;
	}
}
