/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agardet <agardet@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/03 16:20:34 by agardet           #+#    #+#             */
/*   Updated: 2022/03/03 18:47:43 by agardet          ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "utils.h"
#include <stdio.h>

int	is_ifs(char c)
{
	return (c == ' ' || c == '\t' || c == '\n');
}

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
