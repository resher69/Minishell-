/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ebellon <ebellon@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/03 15:16:13 by agardet           #+#    #+#             */
/*   Updated: 2022/03/14 17:37:38 by ebellon          ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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

void	exec_loop(t_shell *shell)
{
	size_t	i;

	i = 0;
	if (tcsetattr(STDIN_FILENO, TCSANOW, &shell->old_term) == -1)
		print_error("termios: ", NULL, NULL, errno);
	signal(SIGINT, SIG_IGN);
	while (i < shell->n_cmd)
	{
		if (i != 0)
			shell->usr_cmd[i] = expand(*shell->usr_cmd[i]->av,
					i, shell->n_cmd, shell);
		if (!shell->usr_cmd[i]->valid)
			break ;
		ft_pipex(shell->usr_cmd[i], shell);
		i++;
	}
	ft_waitpids(shell);
	if (tcsetattr(STDIN_FILENO, TCSANOW, &shell->new_term) == -1)
		print_error("termios: ", NULL, NULL, errno);
	signal(SIGINT, &sig_int);
}

void	ft_exec(t_shell *shell)
{
	t_cmd	*tmp;

	shell->i = 0;
	tmp = expand(shell->usr_cmd[0]->av[0], 0, shell->n_cmd, shell);
	if (tmp)
		shell->usr_cmd[0] = tmp;
	if (tmp->av[0] && shell->n_cmd == 1
		&& is_builtin(tmp->av[0], shell, tmp->av[1]) == 1)
		exec_builtins(tmp->av[0], tmp->av, shell, tmp->fd_out);
	else if (tmp->av[0])
		exec_loop(shell);
}

void	main_loop(t_shell *shell)
{
	int		err;
	char	*usr_input;

	while (!shell->exit && !shell->do_exit)
	{
		usr_input = readline("MiniSHEEEESH$>");
		if (!usr_input)
		{
			ft_putstr_fd("exit\n", STDERR_FILENO);
			free(shell);
			free(usr_input);
			break ;
		}
		add_history(usr_input);
		if (usr_input[0])
		{
			err = split_usr_input(usr_input, shell);
			free(usr_input);
			if (err == 0)
				ft_exec(shell);
		}
	}
}

int	main(int ac, char **av, char **envp)
{
	t_shell	*shell;

	(void)av;
	shell = malloc(sizeof(t_shell));
	init_env(envp, shell);
	g_wstatus = 0;
	set_new_terminal(shell);
	shell->exit = 0;
	shell->do_exit = 0;
	args_checker(ac, av);
	main_loop(shell);
	exit(shell->exit);
	return (0);
}
