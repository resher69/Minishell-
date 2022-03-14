/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redir_in_simple.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ebellon <ebellon@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/12 17:14:40 by ebellon           #+#    #+#             */
/*   Updated: 2022/03/14 15:25:51 by ebellon          ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	open_redir_in(t_cmd *cmd, size_t i)
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

void	open_heredoc(t_cmd *cmd, size_t i, t_shell *shell)
{
	if (heredoc_handler(cmd, cmd->av[i], shell) == -1)
		cmd->valid = 0;
	if (cmd->fd_in < 0)
	{
		cmd->valid = 0;
		print_error("open: ", cmd->av[i], NULL, errno);
	}
	else
		cmd->flags |= E_FILEIN;
}

int	redir_in_simple(t_cmd *cmd, size_t id_redir)
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
			open_redir_in(cmd, i);
		else
		{
			cmd->valid = 0;
			print_error(NULL, NULL,
				"syntax error near unexpected token `newline'\n", 258);
		}
	}
	return (cpy_after_redir(cmd, av, i, j));
}

int	redir_in_double(t_cmd *cmd, size_t id_redir, t_shell *shell)
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
			open_heredoc(cmd, i, shell);
		else
		{
			cmd->valid = 0;
			print_error(NULL, NULL,
				"syntax error near unexpected token `newline'\n", 258);
		}
	}
	return (cpy_after_redir(cmd, av, i, j));
}
