/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ebellon <ebellon@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/13 19:48:16 by ebellon           #+#    #+#             */
/*   Updated: 2022/03/13 20:18:50 by ebellon          ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	count_pipe(char *usr_input, t_shell *shell)
{
	size_t	i;
	int		quote;

	i = 0;
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
	return (0);
}

size_t	check_pipe_quote(char *usr_input, size_t i)
{
	int		quote;
	size_t	j;

	j = 0;
	quote = get_quote(usr_input[i]);
	while (usr_input[i + j] && (usr_input[i + j] != '|'
			|| quote != QUOTE_NONE))
	{
		j++;
		quote = get_quote(usr_input[i + j]);
	}
	return (j);
}

int	cpy_split(char *usr_input, t_cmd **cmd_tmp, t_shell *shell)
{
	size_t	i;
	size_t	j;
	size_t	n_pipe;

	i = 0;
	n_pipe = 0;
	while (usr_input[i] && n_pipe < shell->n_cmd)
	{
		j = check_pipe_quote(usr_input, i);
		cmd_tmp[n_pipe] = ft_calloc(sizeof(t_cmd *), 1);
		if (!cmd_tmp[n_pipe])
			return (1);
		cmd_tmp[n_pipe]->av = ft_calloc(sizeof(char *), 1);
		*cmd_tmp[n_pipe]->av = ft_strldup(usr_input + i, j);
		i += j + 1;
		i += skip_ifs(usr_input + i);
		n_pipe++;
	}
	cmd_tmp[n_pipe] = NULL;
	return (0);
}

int	split_usr_input(char *usr_input, t_shell *shell)
{
	t_cmd	**cmd_tmp;

	shell->n_cmd = 1;
	if (count_pipe(usr_input, shell))
		return (1);
	cmd_tmp = malloc(sizeof(t_cmd *) * (shell->n_cmd + 1));
	if (!cmd_tmp)
		return (1);
	if (cpy_split(usr_input, cmd_tmp, shell))
		return (1);
	if (check_pipe(cmd_tmp) < shell->n_cmd)
		return (printf("Minishell : parse error near `|'\n"));
	shell->usr_cmd = cmd_tmp;
	return (0);
}

void	set_new_terminal(t_shell *s)
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
