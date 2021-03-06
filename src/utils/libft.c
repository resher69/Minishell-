/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   libft.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agardet <agardet@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/03 18:47:05 by agardet           #+#    #+#             */
/*   Updated: 2022/03/03 18:47:20 by agardet          ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

size_t	ft_strcmp(char *a, char *b)
{
	while (a && b && *a && *b && *a == *b)
	{
		a++;
		b++;
	}
	return (*b - *a);
}

size_t	ft_strlen(char *s)
{
	size_t	i;

	i = 0;
	while (s && s[i])
		i++;
	return (i);
}

size_t	ft_strchr(char *s, char c)
{
	size_t	i;

	i = 0;
	while (s && s[i] && s[i] != c)
		i++;
	return (i);
}

size_t	ft_bzero(char *str, size_t size)
{
	size_t	i;

	i = 0;
	while (i < size)
		str[i++] = 0;
	return (i);
}

size_t	ft_bcopy(char *dst, char *src, size_t size)
{	
	size_t	i;

	i = 0;
	while (i < size)
	{
		dst[i] = src[i];
		i++;
	}	
	return (i);
}
