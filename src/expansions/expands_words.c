/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expands_words.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ebellon <ebellon@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/03 16:16:39 by agardet           #+#    #+#             */
/*   Updated: 2022/03/14 18:46:48 by ebellon          ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

size_t	count_exp_word(t_cmd *cmd)
{
	int		i;
	size_t	n_words;
	int		quote;

	i = skip_ifs(*cmd->av);
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
	return (n_words);
}

int	expand_words(t_cmd *cmd)
{
	int		i;
	int		j;
	char	**argv;
	int		quote;

	cmd->ac = count_exp_word(cmd);
	argv = ft_calloc(sizeof(char *), (cmd->ac + 1));
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
	argv -= cmd->ac;
	free(*cmd->av);
	free(cmd->av);
	cmd->av = argv;
	return (0);
}
