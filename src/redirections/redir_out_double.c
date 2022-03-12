/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redir_out_double.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ebellon <ebellon@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/12 17:15:50 by ebellon           #+#    #+#             */
/*   Updated: 2022/03/12 17:16:19 by ebellon          ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	redir_out_double(t_cmd *cmd, size_t id_redir)
{
	size_t	i;
	size_t	j;
	char	**av;

	i = 0;
	av = malloc(sizeof(char *) * cmd->ac);
	while (i < id_redir)
	{
		av[i] = cmd->av[i];
		i++;
	}
	j = i;
	if (cmd->av[i])
	{
		free(cmd->av[i]);
		i++;
		if (cmd->av[i])
		{
			cmd->fd_out = open(cmd->av[i], O_CREAT | O_WRONLY | O_APPEND, 0644);
			if (cmd->fd_out < 0)
			{
				cmd->valid = 0;
				print_error("open: ", cmd->av[i], NULL, errno);
			}	
			else
				cmd->flags |= E_FILEOUT;
		}
		else
		{
			cmd->valid = 0;
			print_error(NULL, NULL,
				"syntax error near unexpected token `newline'\n", 258);
		}
	}
	if (cmd->av[i])
	{
		free(cmd->av[i]);
		i++;
		while (i < cmd->ac)
		{
			av[j] = cmd->av[i];
			i++;
			j++;
		}
		av[j] = NULL;
		free(cmd->av);
	}
	cmd->av = av;
	cmd->ac = j;
	return (0);
}
