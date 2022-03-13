/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agardet <agardet@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/03 16:20:34 by agardet           #+#    #+#             */
/*   Updated: 2022/03/13 19:00:59 by agardet          ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "utils.h"
#include <stdio.h>

int	is_valid_var_char(char c)
{
	return (!is_ifs(c) && c != 0 && c != 61 && c != 36 && c != 32 \
		&& ((c >= 97 && c <= 126) || (c >= 65 && c <= 91)));
}

int	skip_ifs(char *str)
{
	int	i;

	i = 0;
	while (str && str[i] && is_ifs(str[i]))
		i++;
	return (i);
}

int	skip_redir(char *str)
{
	int	i;

	i = 0;
	while (str && str[i] && (str[i] == '>' || str[i] == '<'))
		i++;
	return (i);
}

int	skip_car(char *str)
{
	int	i;
	int	quote;

	i = 0;
	if (*str)
		quote = get_quote(*str);
	while (str && str[i] && ((!is_ifs(str[i])
				&& !(str[i] == '>' || str[i] == '<')) || quote != QUOTE_NONE))
	{
		quote = get_quote(str[i]);
		i++;
	}
	return (i);
}

char	*error_handling(char *s1, char *s2, int alloc_args)
{
	char	*new;

	new = NULL;
	if (!s1)
		new = ft_strldup(s2, ft_strlen(s2));
	else if (!s2)
		new = ft_strldup(s1, ft_strlen(s1));
	if (alloc_args == 1 || alloc_args == 3)
		free((char *)s1);
	if (alloc_args > 2)
		free((char *)s2);
	return (new);
}
