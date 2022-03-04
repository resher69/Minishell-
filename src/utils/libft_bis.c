/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   libft_bis.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agardet <agardet@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/03 18:47:51 by agardet           #+#    #+#             */
/*   Updated: 2022/03/03 18:48:13 by agardet          ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*ft_strldup(char *str, size_t size)
{
	char	*dup;
	size_t	i;

	i = 0;
	dup = malloc((size + 1) * sizeof(char));
	while (i < size && str[i])
	{
		dup[i] = str[i];
		i++;
	}
	dup[i] = 0;
	return (dup);
}

void	ft_strlcpy(char *dst, char *src, size_t len)
{
	while (dst && src && *src && len-- > 0)
		*dst++ = *src++;
}
