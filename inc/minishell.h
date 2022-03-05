/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ebellon <ebellon@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/03 15:14:39 by agardet           #+#    #+#             */
/*   Updated: 2022/03/05 15:29:55 by ebellon          ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include <stdlib.h>
# include <assert.h>
# include <stdio.h>
# include <readline/readline.h>
# include <readline/history.h>
# include <string.h>
# include <fcntl.h>
# include <sys/wait.h>

# include "utils.h"

typedef enum e_masks
{
	E_PIPEIN = (1 << 0),
	E_PIPEOUT = (1 << 1),
	E_FILEIN = (1 << 2),
	E_FILEOUT = (1 << 3),
}			t_masks;

typedef struct s_cmd
{
	char	flags;
	char	**av;
	size_t	ac;
	int		fd_in;
	int		fd_out;
	int		pipe_fd[2];
}				t_cmd;

typedef struct s_env_var
{
	char				*name;
	char				*value;
	struct s_env_var	*next;
}				t_env_var;

typedef struct s_shell
{
	t_cmd	**usr_cmd;
	size_t	n_cmd;
}				t_shell;

extern t_env_var	*g_env;

void	init_env(char **envp);
char	*get_env(char *var_name);

int		expand_variables(t_cmd *cmd);
int		expand_words(t_cmd *cmd);
int		expand_quotes(t_cmd *cmd);

t_cmd	*expand(char *command, size_t id_pipe_line, size_t n_cmd);
int 	redir_in_simple(t_cmd *cmd, size_t id_redir);
int		redir_out_simple(t_cmd *cmd, size_t id_redir);

#endif
