/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ebellon <ebellon@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/03 16:20:34 by agardet           #+#    #+#             */
/*   Updated: 2022/03/12 17:18:56 by ebellon          ###   ########lyon.fr   */
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
