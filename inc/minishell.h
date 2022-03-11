/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ebellon <ebellon@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/03 15:14:39 by agardet           #+#    #+#             */
/*   Updated: 2022/03/11 16:20:12 by ebellon          ###   ########lyon.fr   */
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
# include <stdbool.h>
# include <unistd.h>
# include <signal.h>
# include <sys/param.h>
# include <readline/readline.h>
# include <readline/history.h>
# include <dirent.h>
# include <termios.h>
# include <errno.h>

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
	int		valid;
	char	flags;
	char	**av;
	size_t	ac;
	int		fd_in;
	int		fd_out;
}				t_cmd;

typedef struct s_env_var
{
	char				*name;
	char				*value;
	size_t		index;
	void	*next;
}				t_env_var;

typedef struct s_shell
{
	t_cmd				**usr_cmd;
	size_t				n_cmd;
	pid_t				pid_ar[666];
	size_t				i;
	int					pipe_fd[2];
	struct termios		old_term;
	struct termios		new_term;
	t_env_var			*env;
	size_t				env_size;
	char				**locations;
}				t_shell;

void	init_env(char **envp, t_shell *shell);
char	*get_env(char *var_name, t_shell *shell);

int		expand_variables(t_cmd *cmd, int heredoc, t_shell *shell);
int		expand_words(t_cmd *cmd);
int		expand_quotes(t_cmd *cmd);

t_cmd	*expand(char *command, size_t id_pipe_line, size_t n_cmd, t_shell *shell);
int 	redir_in_simple(t_cmd *cmd, size_t id_redir);
int		redir_in_double(t_cmd *cmd, size_t id_redir, t_shell *shell);
int		redir_out_simple(t_cmd *cmd, size_t id_redir);
int 	redir_out_double(t_cmd *cmd, size_t id_redir);

void	ft_free_tab(char **tab);
char	**get_locations(char **envp);
char	*concat_path(char *location, char *exec);
char	*get_exec_path(char *exec_name, char **locations);

void	ft_dup_close(int fd[2], char **cmd_arg,
			char *exec_path, char **locations);
int		ft_safe_close(int fd);
void	ft_free_job_exit(char **cmd_arg, char *exec_path,
			char **locations, int err);
int		ft_waitpids(t_shell *shell);
void	ft_pipex(t_cmd *cmd, t_shell *shell);
void	ft_putstr_fd(char *s, int fd);
void	sig_int(int sig);
void	heredoc_sig_int(int sig);
char	*ft_strjoin(char *s1, char *s2, int alloc_args);
void	sig_child(int sig);
int		is_builtin(char *str);
void	bi_echo(char **av, t_shell *shell);
void	bi_env(t_shell *shell);
void	bi_exit(t_shell *shell);
void	bi_export(t_shell *shell, char **av);
void	bi_pwd(t_shell *shell);
void	bi_unset(t_shell *shell);
void	bi_cd(char **av, t_shell *shell);

#endif
