/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   quote_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agardet <agardet@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/03 16:18:53 by agardet           #+#    #+#             */
/*   Updated: 2022/03/03 16:20:23 by agardet          ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "utils.h"

int	get_quote(char c)
{
	static int	quote = QUOTE_NONE;

	if (c == '\'' && quote != QUOTE_DOUBLE)
	{
		if (quote == QUOTE_SINGLE)
			quote = QUOTE_NONE;
		else
			quote = QUOTE_SINGLE;
	}
	else if (c == '\"' && quote != QUOTE_SINGLE)
	{
		if (quote == QUOTE_DOUBLE)
			quote = QUOTE_NONE;
		else
			quote = QUOTE_DOUBLE;
	}
	return (quote);
}
