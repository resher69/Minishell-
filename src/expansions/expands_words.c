/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expands_words.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ebellon <ebellon@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/03 16:16:39 by agardet           #+#    #+#             */
/*   Updated: 2022/03/14 19:59:38 by ebellon          ###   ########lyon.fr   */
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

void	get_size_word(t_cmd *cmd, int *i, int *j, int *quote)
{
	while ((*cmd->av)[*i + *j] && ((!is_ifs((*cmd->av)[*i + *j])
		&& !((*cmd->av)[*i + *j] == '>' || (*cmd->av)[*i + *j] == '<'))
		|| *quote != QUOTE_NONE))
	{
		*j += 1;
		*quote = get_quote((*cmd->av)[*i + *j]);
	}
}

void	get_size_redir(t_cmd *cmd, int *i, int *j, int *quote)
{
	while ((*cmd->av)[*i + *j]
		&& ((*cmd->av)[*i + *j] == '>' || (*cmd->av)[*i + *j] == '<')
		&& (!is_ifs((*cmd->av)[*i + *j]) || *quote != QUOTE_NONE))
	{
		*j += 1;
		*quote = get_quote((*cmd->av)[*i + *j]);
	}
}

char	**expand_words_loop(t_cmd *cmd, int id[3], char **argv)
{
	int	quote;

	while (*cmd->av && (*cmd->av)[id[0]])
	{
		id[1] = 0;
		quote = get_quote((*cmd->av)[id[0]]);
		get_size_word(cmd, &id[0], &id[1], &quote);
		if (id[1] > 0)
		{
			argv[id[2]++] = ft_strndup(*cmd->av + id[0], id[1]);
			id[0] += id[1];
			id[1] = 0;
		}
		else if (((*cmd->av)[id[0] + id[1]] == '>'
			|| (*cmd->av)[id[0] + id[1]] == '<'))
			get_size_redir(cmd, &id[0], &id[1], &quote);
		if (id[1] > 0)
		{
			argv[id[2]++] = ft_strndup(*cmd->av + id[0], id[1]);
			id[0] += id[1];
		}
		id[0] += skip_ifs(*cmd->av + id[0]);
	}
	argv[id[2]] = NULL;
	return (argv);
}

int	expand_words(t_cmd *cmd)
{
	char	**argv;

	cmd->ac = count_exp_word(cmd);
	argv = ft_calloc(sizeof(char *), (cmd->ac + 1));
	if (!argv)
		return (1);
	argv = expand_words_loop(cmd, (int [3]){0, 0, 0}, argv);
	free(*cmd->av);
	free(cmd->av);
	cmd->av = argv;
	return (0);
}
