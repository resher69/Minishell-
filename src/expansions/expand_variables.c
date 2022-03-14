/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_variables.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ebellon <ebellon@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/03 16:14:53 by agardet           #+#    #+#             */
/*   Updated: 2022/03/14 18:37:05 by ebellon          ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

ssize_t	get_next_var_len(char *cmd, char **str)
{
	size_t	i;
	size_t	j;

	i = 0;
	j = 0;
	while (cmd && is_valid_var_char(cmd[i]))
		i++;
	*str = malloc((i + 1) * sizeof(char));
	if (!str && *str)
		return (-1);
	while (j < i)
	{
		(*str)[j] = cmd[j];
		j++;
	}
	(*str)[j] = 0;
	return (i);
}

char	*cpy_expand(char *str, size_t size, int heredoc, t_shell *shell)
{
	size_t	id[2];
	char	*expand;
	char	*buffer;
	int		quote;

	expand = malloc((size + 1) * sizeof(char));
	id[0] = 0;
	id[1] = 0;
	while (str && str[id[0]])
	{
		quote = get_quote(str[id[0]]);
		if (str[id[0]] != '$' || (quote == QUOTE_SINGLE && !heredoc))
			cpy_char_expand(expand, str, &id[0], &id[1]);
		else if ((str[id[0]] == '$' || (quote == QUOTE_SINGLE && !heredoc))
			&& str[id[0] + 1] == '?')
			cpy_g_wstatus(expand, &id[0], &id[1]);
		else
		{
			id[0] += get_next_var_len(str + id[0] + 1, &buffer) + 1;
			cpy_env_var(&id[1], buffer, expand, shell);
		}
	}
	expand[id[1]] = 0;
	return (expand);
}

void	do_incr(size_t	*expanded_size, size_t *i,
				size_t i_incr, size_t exp_incr)
{
	*expanded_size += exp_incr;
	*i += i_incr;
}

int	expand_variables(t_cmd *cmd, int heredoc, t_shell *shell)
{
	size_t	expanded_size;
	size_t	i;
	char	*buffer;
	int		quote;

	i = 0;
	expanded_size = 0;
	while (*cmd->av && (*cmd->av)[i])
	{
		quote = get_quote((*cmd->av)[i]);
		if ((*cmd->av)[i] != '$' || (quote == QUOTE_SINGLE && !heredoc))
			do_incr(&expanded_size, &i, 1, 1);
		else if (((*cmd->av)[i] == '$' || (quote == QUOTE_SINGLE && !heredoc))
				&& (*cmd->av)[i + 1] == '?')
			do_incr(&expanded_size, &i, 2, 1);
		else
		{
			i += get_next_var_len(*cmd->av + i + 1, &buffer) + 1;
			expanded_size += ft_strlen(get_env(buffer, shell));
			free(buffer);
		}
	}
	*cmd->av = cpy_expand(*cmd->av, expanded_size, heredoc, shell);
	return (0);
}
