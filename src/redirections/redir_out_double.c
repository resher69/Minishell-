/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redir_out_double.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ebellon <ebellon@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/12 17:15:50 by ebellon           #+#    #+#             */
/*   Updated: 2022/03/14 15:08:12 by ebellon          ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	open_redir_out_double(t_cmd *cmd, size_t i)
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

int	redir_out_double(t_cmd *cmd, size_t id_redir)
{
	size_t	i;
	size_t	j;
	char	**av;

	i = 0;
	av = ft_calloc(sizeof(char *), cmd->ac);
	while (i < id_redir)
	{
		av[i] = cmd->av[i];
		i++;
	}
	j = i;
	if (cmd->av[i])
	{
		free(cmd->av[i++]);
		if (cmd->av[i])
			open_redir_out_double(cmd, i);
		else
		{
			cmd->valid = 0;
			print_error(NULL, NULL,
				"syntax error near unexpected token `newline'\n", 258);
		}
	}
	return (cpy_after_redir(cmd, av, i, j));
}
