/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bi_pwd.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ebellon <ebellon@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/11 18:23:52 by agardet           #+#    #+#             */
/*   Updated: 2022/03/12 18:11:01 by ebellon          ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	bi_pwd(void)
{
	char	cwd[MAXPATHLEN];

	g_wstatus = EXIT_SUCCESS;
	if (!getcwd(cwd, MAXPATHLEN))
	{
		print_error("getcwd: ", NULL, NULL, errno);
		g_wstatus = EXIT_FAILURE;
		return ;
	}
	ft_putstr_fd(cwd, 1);
	ft_putstr_fd("\n", 1);
}
