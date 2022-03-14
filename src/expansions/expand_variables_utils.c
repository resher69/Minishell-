/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_variables_utils.c                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ebellon <ebellon@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/14 18:36:38 by ebellon           #+#    #+#             */
/*   Updated: 2022/03/14 18:37:02 by ebellon          ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	cpy_g_wstatus(char *expand, size_t *i, size_t *j)
{
	char	*buffer;

	buffer = ft_nbtobase(g_wstatus, "0123456789");
	ft_strlcpy(expand + *j, buffer, ft_strlen(buffer));
	*i += 2;
	*j += ft_strlen(buffer);
	free(buffer);
}

void	cpy_env_var(size_t *j, char *buffer, char *expand, t_shell *shell)
{
	ft_strlcpy(expand + *j, get_env(buffer, shell),
		ft_strlen(get_env(buffer, shell)));
	*j += ft_strlen(get_env(buffer, shell));
	free(buffer);
}

void	cpy_char_expand(char *expand, char *str, size_t *i, size_t *j)
{
	expand[*j] = str[*i];
	*i += 1;
	*j += 1;
}
