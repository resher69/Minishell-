/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ebellon <ebellon@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/03 15:16:13 by agardet           #+#    #+#             */
/*   Updated: 2022/03/11 20:28:35 by ebellon          ###   ########lyon.fr   */
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
		print_error("termios: ", NULL, NULL, errno);
	s->new_term = s->old_term;
	s->new_term.c_cc[VQUIT] = 0;
	s->new_term.c_lflag &= ~ECHOCTL;
	if (tcsetattr(STDIN_FILENO, TCSANOW, &s->new_term) == -1)
		print_error("termios: ", NULL, NULL, errno);
	signal(SIGINT, &sig_int);
	signal(SIGQUIT, SIG_IGN);
}

static void	args_checker(int argc, char **argv)
{
	(void)argv;
	if (!isatty(STDIN_FILENO) || !isatty(STDOUT_FILENO)
		|| !isatty(STDERR_FILENO))
	{
		print_error(NULL, NULL, "not in a terminal\n", EXIT_FAILURE);
		exit(g_wstatus);
	}
	if (argc != 1)
	{
		print_error(NULL, NULL, NULL, E2BIG);
		exit(g_wstatus);
	}
}

int main(int ac, char **av, char **envp)
{
	t_shell	*shell;
	char	*usr_input;
	int		err;
	size_t	i;
	size_t	j;

	(void)av;
	args_checker(ac, av);
	shell = malloc(sizeof(t_shell));
	init_env(envp, shell);
	g_wstatus = 0;
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
			if (err == 0)
			{
				i = 0;
				free(usr_input);
				shell->i = 0;
				if (tcsetattr(STDIN_FILENO, TCSANOW, &shell->old_term) == -1)
					print_error("termios: ", NULL, NULL, errno);
				signal(SIGINT, SIG_IGN);
				while (i < shell->n_cmd)
				{
					shell->usr_cmd[i] = expand(*shell->usr_cmd[i]->av, i, shell->n_cmd, shell);
					if (!shell->usr_cmd[i]->valid)
						break ;
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
					ft_pipex(shell->usr_cmd[i], shell);
					i++;
				}
				ft_waitpids(shell);
				if (tcsetattr(STDIN_FILENO, TCSANOW, &shell->new_term) == -1)
					print_error("termios: ", NULL, NULL, errno);
				signal(SIGINT, &sig_int);
			}
		}
	}
	return (0);
}
