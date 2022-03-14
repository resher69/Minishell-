/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ebellon <ebellon@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/12 17:11:53 by ebellon           #+#    #+#             */
/*   Updated: 2022/03/14 15:26:20 by ebellon          ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static char	*heredoc_loop(t_cmd *tmp, t_shell *shell, char *stop)
{
	char	*line;
	char	*content;

	line = NULL;
	content = NULL;
	while (1)
	{
		line = readline("> ");
		if (content)
			content = ft_strjoin(content, "\n", 1);
		if (!line || !ft_strcmp(line, stop))
			break ;
		*tmp->av = line;
		expand_variables(tmp, 1, shell);
		line = *tmp->av;
		content = ft_strjoin(content, line, 3);
		if (!content)
		{
			print_error("malloc: ", NULL, NULL, ENOMEM);
			break ;
		}
	}
	free(tmp);
	return (content);
}

static char	*heredoc_init_loop(char *stop, t_shell *shell)
{
	char	*line;
	char	*content;
	t_cmd	*tmp;

	line = NULL;
	content = NULL;
	tmp = ft_calloc(sizeof(t_cmd), 1);
	if (!tmp)
	{
		print_error("malloc: ", NULL, NULL, ENOMEM);
		return (0);
	}
	tmp->av = ft_calloc(sizeof(char *), 1);
	if (!tmp->av)
	{
		print_error("malloc: ", NULL, NULL, ENOMEM);
		return (0);
	}
	return (heredoc_loop(tmp, shell, stop));
}

static void	heredoc(char *stop, int pipe_fd[2], t_shell *shell)
{
	char	*content;

	content = heredoc_init_loop(stop, shell);
	if (content)
		ft_putstr_fd(content, pipe_fd[STDOUT_FILENO]);
	free(content);
	if (close(pipe_fd[STDOUT_FILENO]) == -1
		|| close(pipe_fd[STDIN_FILENO]) == -1)
	{
		print_error("close: ", NULL, NULL, errno);
		return ;
	}
}

static void	heredoc_process_init(char *stop, int pipe_fd[2], t_shell *shell)
{
	pid_t	pid;
	int		status;

	pid = fork();
	if (pid == -1)
	{
		print_error("fork: ", NULL, NULL, errno);
		return ;
	}
	else if (pid == 0)
	{
		signal(SIGINT, &heredoc_sig_int);
		heredoc(stop, pipe_fd, shell);
		exit(g_wstatus);
	}
	else if (waitpid(-1, &status, WUNTRACED) == -1)
	{
		print_error("waitpid: ", NULL, NULL, errno);
		return ;
	}
	if (WIFEXITED(status))
		g_wstatus = WEXITSTATUS(status);
}

int	heredoc_handler(t_cmd *current, char *stop, t_shell *shell)
{
	int		pipe_fd[2];

	if (pipe(pipe_fd) == -1)
	{
		print_error("pipe: ", NULL, NULL, errno);
		return (-1);
	}
	signal(SIGINT, SIG_IGN);
	heredoc_process_init(stop, pipe_fd, shell);
	signal(SIGINT, &sig_int);
	if (g_wstatus)
		return (-1);
	current->fd_in = pipe_fd[STDIN_FILENO];
	if (close(pipe_fd[STDOUT_FILENO]) == -1)
	{
		print_error("close: ", NULL, NULL, errno);
		return (-1);
	}
	return (0);
}
