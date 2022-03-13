/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ebellon <ebellon@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/03 15:14:39 by agardet           #+#    #+#             */
/*   Updated: 2022/03/13 19:58:17 by ebellon          ###   ########lyon.fr   */
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
	char	*name;
	char	*value;
	size_t	index;
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
	char				**locations;
	int					exit;
	int					do_exit;
}				t_shell;

int		g_wstatus;

void		init_env(char **envp, t_shell *shell);
char		*get_env(char *var_name, t_shell *shell);

int			expand_variables(t_cmd *cmd, int heredoc, t_shell *shell);
int			expand_words(t_cmd *cmd);
int			expand_quotes(t_cmd *cmd);

t_cmd		*expand(char *command, size_t id_pipe_line,
				size_t n_cmd, t_shell *shell);
int			redir_in_simple(t_cmd *cmd, size_t id_redir);
int			redir_in_double(t_cmd *cmd, size_t id_redir, t_shell *shell);
int			redir_out_simple(t_cmd *cmd, size_t id_redir);
int			redir_out_double(t_cmd *cmd, size_t id_redir);

void		ft_free_tab(char **tab);
char		**get_locations(char **envp);
char		*concat_path(char *location, char *exec);
char		*get_exec_path(char *exec_name, char **locations,
				t_shell *shell, char *f_arg);
char		*ft_nbtobase(long long nb, char *base);
void		ft_dup_close(int fd[2], char **cmd_arg,
				char *exec_path, char **locations);
int			ft_safe_close(int fd);
void		ft_free_job_exit(char **cmd_arg, char *exec_path,
				char **locations, int err);
void		ft_waitpids(t_shell *shell);
void		ft_pipex(t_cmd *cmd, t_shell *shell);
void		ft_putstr_fd(char *s, int fd);
void		sig_int(int sig);
void		heredoc_sig_int(int sig);
char		*ft_strjoin(char *s1, char *s2, int alloc_args);
void		sig_child(int sig);
int			is_builtin(char *str, t_shell *shell, char *f_arg);
void		bi_echo(char **av, int fd);
void		bi_env(t_shell *shell);
void		bi_exit(t_shell *shell, char **av);
void		bi_export(t_shell *shell, char **av, int fd);
void		bi_pwd(void);
void		bi_unset(t_shell *shell, char **av);
void		bi_cd(char **av, t_shell *shell);
void		print_error(char *cmd, char *value, char *error, int status);
char		*ft_strchrstr(const char *s1, const char *s2);
int			ft_atoi(const char *str);
void		exec_builtins(char *exec_path, char **av, t_shell *shell, int fd);
int			is_word(char *str);
void		env_del(t_shell *s, t_env_var *elem);
t_env_var	*find_env_var(t_shell *shell, char *name);
int			env_new(t_shell *s, char *var);
void		create_env_variable(t_shell *s, char **av);
int			create_variable(t_shell *s, char *cmd, char *name);
void		exec_export(int fd, size_t i, t_env_var *export);
int			get_nb_len(long long nb, int baselen);
size_t		ft_strlcat(char *dst, char *src, size_t dst_size);
char		*error_handling(char *s1, char *s2, int alloc_args);
void		free_redir(t_cmd *cmd, size_t i, size_t j, char **av);
int			heredoc_handler(t_cmd *current, char *stop, t_shell *shell);
void		*ft_calloc(size_t count, size_t size);
char		*ft_strndup(char *src, size_t size);
char		**ft_split(char const *s, char c);
char		**list_to_char(t_env_var *env);
void		set_new_terminal(t_shell *s);
int			split_usr_input(char *usr_input, t_shell *shell);
size_t		check_pipe(t_cmd **cmd);

#endif
