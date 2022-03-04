/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ebellon <ebellon@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/03 16:16:00 by agardet           #+#    #+#             */
/*   Updated: 2022/03/04 18:19:10 by ebellon          ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_cmd	*expand(char *cmd)
{
	t_cmd	*command;
	size_t	i;

	command = malloc(sizeof(t_cmd));
	if (!command)
		return (NULL);
	command->av = malloc(sizeof(char *));
	if (!command->av)
		return (NULL);
	*command->av = cmd;
	command->ac = 0;
	if (expand_variables(command) == -1)
		return (NULL);
	expand_words(command);
	i = 0;
	// test < redir | <redir | <     redir | "<redir" | '<redir'
	while (i < command->ac)
	{
		if (strncmp(command->av[i], "<", 1) == 0)
		{
			i = redir_in_simple(command, i);
		}
		else if (strncmp(command->av[i], "<<", 2) == 0)
		{
			printf("redir in double\n");
			i++;
		}
		else if (strncmp(command->av[i], ">", 1) == 0)
		{
			i = redir_out_simple(command, i);
		}
		else if (strncmp(command->av[i], ">>", 2) == 0)
		{
			printf("redir out double\n");
			i++;
		}
		else
			i++;
	}
	expand_quotes(command);
	return (command);
}
