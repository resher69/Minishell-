/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   libft_third.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agardet <agardet@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/13 17:57:19 by agardet           #+#    #+#             */
/*   Updated: 2022/03/13 18:59:30 by agardet          ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	is_word(char *str)
{
	size_t	i;

	i = 0;
	if (ft_isdigit(str[i]))
		return (0);
	while (str[i])
	{
		if (!ft_isalnum(str[i]) && str[i] != '_')
			return (0);
		i++;
	}
	return (1);
}

char	*ft_strchr_bis(const char *str, int c)
{
	while (*str)
	{
		if (*str == c)
			return ((char *)str);
		str++;
	}
	if (*str == c)
		return ((char *)str);
	return (NULL);
}

char	*ft_substr(char *src, unsigned int start, size_t len)
{
	char	*new;
	size_t	new_max_len;

	if (start > ft_strlen(src))
		return (NULL);
	new_max_len = ft_strlen(&src[start]);
	if (len > new_max_len)
		len = new_max_len;
	new = calloc((len + 1), sizeof(char));
	if (!new)
		return (NULL);
	ft_strlcpy(new, &src[start], len + 1);
	return (new);
}

size_t	ft_strlcat(char *dst, char *src, size_t dst_size)
{
	size_t	dst_len;
	size_t	src_len;
	size_t	i;

	dst_len = ft_strlen(dst);
	src_len = ft_strlen(src);
	if (dst_len >= dst_size)
		return (dst_size + src_len);
	i = 0;
	while (src[i] && (i + dst_len < dst_size - 1))
	{
		dst[i + dst_len] = src[i];
		i++;
	}
	dst[i + dst_len] = '\0';
	return (dst_len + src_len);
}

int	is_ifs(char c)
{
	return (c == ' ' || c == '\t' || c == '\n');
}
