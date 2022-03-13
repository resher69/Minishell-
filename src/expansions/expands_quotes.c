/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expands_quotes.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ebellon <ebellon@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/03 16:16:14 by agardet           #+#    #+#             */
/*   Updated: 2022/03/13 20:26:12 by ebellon          ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	cpy_expand_quote(char *expand, t_cmd *cmd, size_t i)
{
	size_t	j;
	size_t	k;
	int		quote;

	j = 0;
	k = 0;
	while (cmd->av[i][j])
	{
		quote = get_quote(cmd->av[i][j]);
		if ((cmd->av[i][j] != '\'' || quote == QUOTE_DOUBLE)
			&& (cmd->av[i][j] != '\"' || quote == QUOTE_SINGLE))
		{
			expand[k] = cmd->av[i][j];
			k++;
		}
		j++;
	}
	expand[k] = 0;
}

int	expand_quotes(t_cmd *cmd)
{
	size_t	i;
	char	*expand;

	i = 0;
	while (i < cmd->ac)
	{
		if (cmd->av[i])
		{
			expand = malloc(sizeof(char) * strlen(cmd->av[i]));
			cpy_expand_quote(expand, cmd, i);
			free(cmd->av[i]);
			cmd->av[i] = expand;
		}
		i++;
	}
	return (0);
}
