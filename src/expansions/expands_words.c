/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expands_words.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ebellon <ebellon@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/03 16:16:39 by agardet           #+#    #+#             */
/*   Updated: 2022/03/12 17:10:40 by ebellon          ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	expand_words(t_cmd *cmd)
{
	int		i;
	int		j;
	size_t	n_words;
	char	**argv;
	int		quote;

	i = 0;
	i += skip_ifs(*cmd->av + i);
	n_words = 0;
	while (*cmd->av && (*cmd->av)[i])
	{
		quote = get_quote((*cmd->av)[i]);
		if ((*cmd->av)[i] && ((*cmd->av)[i] == '>'
			|| (*cmd->av)[i] == '<') && quote == QUOTE_NONE)
		{
			i += skip_redir(*cmd->av + i);
			n_words++;
		}
		else if ((*cmd->av)[i] && (!is_ifs((*cmd->av)[i])
			|| quote != QUOTE_NONE))
		{
			i += skip_car(*cmd->av + i);
			n_words++;
		}
		else
			i += skip_ifs(*cmd->av + i);
	}
	cmd->ac = n_words;
	argv = malloc(sizeof(char *) * (n_words + 1));
	if (!argv)
		return (1);
	i = 0;
	while (*cmd->av && (*cmd->av)[i])
	{
		quote = get_quote((*cmd->av)[i]);
		j = 0;
		while ((*cmd->av)[i + j] && ((!is_ifs((*cmd->av)[i + j])
			&& !((*cmd->av)[i + j] == '>' || (*cmd->av)[i + j] == '<'))
			|| quote != QUOTE_NONE))
		{
			j++;
			quote = get_quote((*cmd->av)[i + j]);
		}
		if (j > 0)
		{
			*argv = malloc((j + 1) * sizeof(char));
			if (!argv)
				return (1);
			ft_strlcpy(*argv, *cmd->av + i, j);
			(*argv++)[j] = 0;
			i += j;
			j = 0;
		}
		else if (((*cmd->av)[i + j] == '>' || (*cmd->av)[i + j] == '<'))
		{
			while ((*cmd->av)[i + j]
				&& ((*cmd->av)[i + j] == '>' || (*cmd->av)[i + j] == '<')
				&& (!is_ifs((*cmd->av)[i + j]) || quote != QUOTE_NONE))
			{
				j++;
				quote = get_quote((*cmd->av)[i + j]);
			}
		}
		if (j > 0)
		{
			*argv = malloc((j + 1) * sizeof(char));
			if (!argv)
				return (1);
			ft_strlcpy(*argv, *cmd->av + i, j);
			(*argv++)[j] = 0;
			i += j;
			j = 0;
		}
		i += skip_ifs(*cmd->av + i);
	}
	*argv = NULL;
	argv -= n_words;
	free(*cmd->av);
	free(cmd->av);
	cmd->av = argv;
	return (0);
}
