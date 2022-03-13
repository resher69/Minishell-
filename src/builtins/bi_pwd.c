/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bi_pwd.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agardet <agardet@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/11 18:23:52 by agardet           #+#    #+#             */
/*   Updated: 2022/03/13 19:07:36 by agardet          ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	*ft_calloc(size_t count, size_t size)
{
	void	*s;

	s = malloc(count * size);
	if (!s)
		return (NULL);
	ft_bzero(s, count * size);
	return (s);
}

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
