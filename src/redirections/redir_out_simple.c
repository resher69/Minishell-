/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redir_out_simple.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agardet <agardet@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/12 17:16:53 by ebellon           #+#    #+#             */
/*   Updated: 2022/03/13 18:54:25 by agardet          ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	exec_redir_out_s(t_cmd *cmd, size_t i)
{
	if (cmd->av[i])
	{
		free(cmd->av[i]);
		i++;
		if (cmd->av[i])
		{
			cmd->fd_out = open(cmd->av[i], O_WRONLY | O_TRUNC | O_CREAT, 0644);
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
}

int	redir_out_simple(t_cmd *cmd, size_t id_redir)
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
	exec_redir_out_s(cmd, i);
	free_redir(cmd, i, j, av);
	cmd->av = av;
	cmd->ac = j;
	return (0);
}
