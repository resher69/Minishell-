/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ebellon <ebellon@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/03 15:16:13 by agardet           #+#    #+#             */
/*   Updated: 2022/03/10 20:21:26 by ebellon          ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	split_usr_input(char *usr_input, t_shell *shell)
{
	t_cmd	**cmd_tmp;
	size_t	i;
	size_t	j;
	size_t	n_pipe;
	int		quote;

	i = 0;
	shell->n_cmd = 1;
	i += skip_ifs(&usr_input[i]);
	quote = QUOTE_NONE;
	while (usr_input && usr_input[i])
	{
		if (usr_input[i] == '|' && i == 0 + (size_t)skip_ifs(usr_input))
			return (printf("Minishell : parse error near `|'\n"));
		quote = get_quote(usr_input[i]);
		if (usr_input[i] && usr_input[i] == '|' && quote == QUOTE_NONE)
			shell->n_cmd++;
		i++;
		i += skip_ifs(&usr_input[i]);
	}
	if (quote != QUOTE_NONE)
	{
		if (quote == QUOTE_SINGLE)
			quote = get_quote('\'');
		else
			quote = get_quote('\"');
		return (printf("Minishell : parse error : unclosed quote\n"));
	}
	cmd_tmp = malloc(sizeof(t_cmd *) * (shell->n_cmd + 1));
	if (!cmd_tmp)
		return (1);
	i = 0;
	n_pipe = 0;
	while (usr_input[i] && n_pipe < shell->n_cmd)
	{
		j = 0;
		quote = get_quote(usr_input[i]);
		while (usr_input[i + j] && (usr_input[i + j] != '|' || quote != QUOTE_NONE))
		{
			j++;
			quote = get_quote(usr_input[i + j]);
		}
		cmd_tmp[n_pipe] = malloc(sizeof(t_cmd *));
		if (!cmd_tmp[n_pipe])
			return (1);
		cmd_tmp[n_pipe]->av = malloc(sizeof(char *));
		*cmd_tmp[n_pipe]->av = ft_strldup(usr_input + i, j);
		i += j + 1;
		i += skip_ifs(usr_input + i);
		n_pipe++;
	}
	cmd_tmp[n_pipe] = NULL;
	size_t k = 0;
	while (cmd_tmp && cmd_tmp[k])
	{
		if (cmd_tmp[k]->av[0])
			k++;
		else
			break ;
	}
	if (k < shell->n_cmd)
		return (printf("Minishell : parse error near `|'\n"));
	shell->usr_cmd = cmd_tmp;
	return (0);
}

static void	set_new_terminal(t_shell *s)
{
	if (tcgetattr(STDIN_FILENO, &s->old_term) == -1)
		;
		// print_error("termios: ", NULL, NULL, errno);
	s->new_term = s->old_term;
	s->new_term.c_cc[VQUIT] = 0;
	s->new_term.c_lflag &= ~ECHOCTL;
	if (tcsetattr(STDIN_FILENO, TCSANOW, &s->new_term) == -1)
		;
		// print_error("termios: ", NULL, NULL, errno);
	signal(SIGINT, &sig_int);
	signal(SIGQUIT, SIG_IGN);
}

int main(int ac, char **av, char **envp)
{
	t_shell	*shell;
	char	*usr_input;
	int		err;
	size_t	i;
	size_t	j;
	t_cmd	exec;

	(void)av;
	if (ac != 1)
		return (printf("No arg needed\n") * 0 + 1);
	shell = malloc(sizeof(t_shell));
	init_env(envp, shell);
	set_new_terminal(shell);
	while (1)
	{
		usr_input = readline("MiniSHEEEESH$>");
		if (!usr_input)
		{
			//free all
			ft_putstr_fd("exit\n", STDERR_FILENO);
			free(shell);
			free(usr_input);
			break;	
		}
		add_history(usr_input);
		if (usr_input[0])
		{
			if (strcmp(usr_input, "exit") == 0)
			{
				//free all
				free(shell);
				free(usr_input);
				break;
			}
			err = split_usr_input(usr_input, shell);
			exec = (t_cmd){0};
			if (err == 0)
			{
				i = 0;
				free(usr_input);
				shell->i = 0;
				while (i < shell->n_cmd)
				{
					shell->usr_cmd[i] = expand(*shell->usr_cmd[i]->av, i, shell->n_cmd, shell);
					j = 0;
					// // aff cmd
					// printf("-> Printing argv & flags\n");
					// if (shell->usr_cmd[i]->flags & E_FILEIN)
					// 	printf("-> file in\n");
					// if (shell->usr_cmd[i]->flags & E_FILEOUT)
					// 	printf("-> file out\n");
					// if (shell->usr_cmd[i]->flags & E_PIPEIN)
					// 	printf("-> pipe in\n");
					// if (shell->usr_cmd[i]->flags & E_PIPEOUT)
					// 	printf("-> pipe out\n");
					// while (shell->usr_cmd[i] && shell->usr_cmd[i]->av && shell->usr_cmd[i]->av[j])
					// 	printf("-> |%s|\n", shell->usr_cmd[i]->av[j++]);
					// //
					exec.flags = shell->usr_cmd[i]->flags;
					exec.av = shell->usr_cmd[i]->av;
					exec.ac = shell->usr_cmd[i]->ac;
					exec.fd_in = shell->usr_cmd[i]->fd_in;
					exec.fd_out = shell->usr_cmd[i]->fd_out;
					exec.valid = shell->usr_cmd[i]->valid;
					if (tcsetattr(STDIN_FILENO, TCSANOW, &shell->old_term) == -1)
						;
						// print_error("termios: ", NULL, NULL, errno);
					signal(SIGINT, SIG_IGN);
					ft_pipex(&exec, shell);
					if (tcsetattr(STDIN_FILENO, TCSANOW, &shell->new_term) == -1)
						;
						// print_error("termios: ", NULL, NULL, errno);
					signal(SIGINT, &sig_int);
					i++;
				}
				ft_waitpids(shell);
			}
		}
	}
	return (0);
}
