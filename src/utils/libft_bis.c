/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   libft_bis.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agardet <agardet@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/03 18:47:51 by agardet           #+#    #+#             */
/*   Updated: 2022/03/13 18:07:22 by agardet          ###   ########lyon.fr   */
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

char	*ft_strjoin(char *s1, char *s2, int alloc_args)
{
	size_t	s1_len;
	size_t	s2_len;
	char	*new;

	if (!s1 || !s2)
		return (error_handling(s1, s2, alloc_args));
	s1_len = ft_strlen(s1);
	s2_len = ft_strlen(s2);
	new = calloc((s1_len + s2_len + 1), sizeof(char));
	if (!new)
		return (error_handling(NULL, NULL, alloc_args));
	ft_strlcpy(new, s1, s1_len + 1);
	ft_strlcat(new, s2, s1_len + s2_len + 1);
	if (alloc_args == 1 || alloc_args == 3)
		free((char *)s1);
	if (alloc_args > 2)
		free((char *)s2);
	return (new);
}

char	*ft_nbtobase(long long nb, char *base)
{
	int		base_len;
	int		i;
	char	*nbr;

	base_len = ft_strlen(base);
	i = get_nb_len(nb, base_len);
	nbr = calloc(i + 1, sizeof(char));
	if (!nbr)
		return (NULL);
	if (nb < 0)
	{
		nbr[0] = '-';
		nb *= -1;
	}
	nbr[i] = '\0';
	i--;
	while (nb >= base_len)
	{
		nbr[i] = base[nb % base_len];
		nb /= base_len;
		i--;
	}
	nbr[i] = base[nb];
	return (nbr);
}

int	get_nb_len(long long nb, int baselen)
{
	size_t	i;

	i = 0;
	if (nb <= 0)
		i++;
	while (nb)
	{
		nb /= baselen;
		i++;
	}
	return (i);
}
