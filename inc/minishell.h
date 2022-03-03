/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agardet <agardet@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/03 15:14:39 by agardet           #+#    #+#             */
/*   Updated: 2022/03/03 15:15:23 by agardet          ###   ########lyon.fr   */
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

# include "utils.h"

typedef struct s_cmd
{
	char	**av;
	size_t	ac;
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

t_cmd	*expand(char *command);

#endif
