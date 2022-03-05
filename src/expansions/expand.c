/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ebellon <ebellon@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/03 16:16:00 by agardet           #+#    #+#             */
/*   Updated: 2022/03/05 15:29:33 by ebellon          ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	set_flags(t_cmd *cmd, size_t id_pipe_line, size_t n_cmd)
{
	if (id_pipe_line == 0 && n_cmd > 1)
	{
		cmd->flags |= E_PIPEOUT;
		cmd->flags &= ~E_PIPEIN;
	}
	else if (id_pipe_line == n_cmd - 1 && n_cmd > 1)
	{
		cmd->flags |= E_PIPEIN;
		cmd->flags &= ~E_PIPEOUT;
	}
	else if (id_pipe_line > 0 && id_pipe_line < n_cmd - 1 && n_cmd > 1)
		cmd->flags |= (E_PIPEIN | E_PIPEOUT);
}

t_cmd	*expand(char *cmd, size_t id_pipe_line, size_t n_cmd)
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
	command->flags = 0;
	set_flags(command, id_pipe_line, n_cmd);
	command->ac = 0;
	if (expand_variables(command) == -1)
		return (NULL);
	expand_words(command);
	i = 0;
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
