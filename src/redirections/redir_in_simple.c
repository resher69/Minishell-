/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redir_in_simple.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agardet <agardet@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/12 17:14:40 by ebellon           #+#    #+#             */
/*   Updated: 2022/03/13 18:52:57 by agardet          ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	exec_redir_in_s(t_cmd *cmd, size_t i)
{
	if (cmd->av[i])
	{
		free(cmd->av[i]);
		i++;
		if (cmd->av[i])
		{
			cmd->fd_in = open(cmd->av[i], O_RDONLY, 0644);
			if (cmd->fd_in < 0)
			{
				cmd->valid = 0;
				print_error("open: ", cmd->av[i], NULL, errno);
			}
			else
				cmd->flags |= E_FILEIN;
		}
		else
		{
			cmd->valid = 0;
			print_error(NULL, NULL,
				"syntax error near unexpected token `newline'\n", 258);
		}
	}
}

void	free_redir(t_cmd *cmd, size_t i, size_t j, char **av)
{
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
}

int	redir_in_simple(t_cmd *cmd, size_t id_redir)
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
	exec_redir_in_s(cmd, i);
	free_redir(cmd, i, j, av);
	cmd->av = av;
	cmd->ac = j;
	return (0);
}
