/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_variables.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ebellon <ebellon@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/03 16:14:53 by agardet           #+#    #+#             */
/*   Updated: 2022/03/10 20:13:33 by ebellon          ###   ########lyon.fr   */
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
	size_t	off;
	int		quote;

	expand = malloc((size + 1) * sizeof(char));
	id[0] = 0;
	id[1] = 0;
	while (str && str[id[0]])
	{
		quote = get_quote(str[id[0]]);
		if (str[id[0]] != '$' || (quote == QUOTE_SINGLE && !heredoc))
		{
			expand[id[1]] = str[id[0]];
			id[0] += 1;
			id[1] += 1;
		}
		else
		{
			off = get_next_var_len(str + id[0] + 1, &buffer) + 1;
			id[0] += off;
			ft_strlcpy(expand + id[1], get_env(buffer, shell), ft_strlen(get_env(buffer, shell)));
			id[1] += ft_strlen(get_env(buffer, shell));
			free(buffer);
		}
	}
	expand[id[1]] = 0;
	return (expand);
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
		{
			i++;
			expanded_size++;
		}
		else
		{
			int off = get_next_var_len(*cmd->av + i + 1, &buffer) + 1;
			i += off;
			expanded_size += ft_strlen(get_env(buffer, shell));
			free(buffer);
		}
	}
	*cmd->av = cpy_expand(*cmd->av, expanded_size, heredoc, shell);
	return (0);
}
