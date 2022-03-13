/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redir_in_double.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agardet <agardet@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/13 18:56:41 by agardet           #+#    #+#             */
/*   Updated: 2022/03/13 18:58:37 by agardet          ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	exec_redir_in_d(t_cmd *cmd, size_t i, t_shell *shell)
{
	if (cmd->av[i])
	{
		free(cmd->av[i]);
		i++;
		if (cmd->av[i])
		{
			if (heredoc_handler(cmd, cmd->av[i], shell) == -1)
			{
				cmd->valid = 0;
			}
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

int	redir_in_double(t_cmd *cmd, size_t id_redir, t_shell *shell)
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
	exec_redir_in_d(cmd, i, shell);
	free_redir(cmd, i, j, av);
	cmd->av = av;
	cmd->ac = j;
	return (0);
}
