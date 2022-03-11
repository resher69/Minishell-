#include "minishell.h"

size_t	ft_strlcat(char *dst, char *src, size_t dst_size)
{
	size_t	dst_len;
	size_t	src_len;
	size_t	i;

	dst_len = ft_strlen(dst);
	src_len = ft_strlen(src);
	if (dst_len >= dst_size)
		return (dst_size + src_len);
	i = 0;
	while (src[i] && (i + dst_len < dst_size - 1))
	{
		dst[i + dst_len] = src[i];
		i++;
	}
	dst[i + dst_len] = '\0';
	return (dst_len + src_len);
}

static char	*error_handling(char *s1, char *s2, int alloc_args)
{
	char	*new;

	new = NULL;
	if (!s1)
		new = strdup(s2);
	else if (!s2)
		new = strdup(s1);
	if (alloc_args == 1 || alloc_args == 3)
		free((char *)s1);
	if (alloc_args > 2)
		free((char *)s2);
	return (new);
}

static int	get_nb_len(long long nb, int baselen)
{
	size_t	i;

	i = 0;
	if (nb <= 0)
		i++;
	while (nb)
	{
		nb /= baselen;
		i++;
	}
	return (i);
}

char	*ft_nbtobase(long long nb, char *base)
{
	int		base_len;
	int		i;
	char	*nbr;

	base_len = ft_strlen(base);
	i = get_nb_len(nb, base_len);
	nbr = calloc(i + 1, sizeof(char));
	if (!nbr)
		return (NULL);
	if (nb < 0)
	{
		nbr[0] = '-';
		nb *= -1;
	}
	nbr[i] = '\0';
	i--;
	while (nb >= base_len)
	{
		nbr[i] = base[nb % base_len];
		nb /= base_len;
		i--;
	}
	nbr[i] = base[nb];
	return (nbr);
}

char	*ft_strjoin(char *s1, char *s2, int alloc_args)
{
	size_t	s1_len;
	size_t	s2_len;
	char	*new;

	if (!s1 || !s2)
		return (error_handling(s1, s2, alloc_args));
	s1_len = ft_strlen(s1);
	s2_len = ft_strlen(s2);
	new = calloc((s1_len + s2_len + 1), sizeof(char));
	if (!new)
		return (error_handling(NULL, NULL, alloc_args));
	ft_strlcpy(new, s1, s1_len + 1);
	ft_strlcat(new, s2, s1_len + s2_len + 1);
	if (alloc_args == 1 || alloc_args == 3)
		free((char *)s1);
	if (alloc_args > 2)
		free((char *)s2);
	return (new);
}

static char	*heredoc_loop(char *stop, t_shell *shell)
{
	char	*line;
	char	*content;
    t_cmd   *tmp;

	line = NULL;
	content = NULL;
    tmp = calloc(sizeof(t_cmd), 1);
    tmp->av = calloc(sizeof(char*), 1);
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
	return (content);
}

static void	heredoc(char *stop, int pipe_fd[2], t_shell *shell)
{
	char	*content;

	content = heredoc_loop(stop, shell);
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
		// free_all(s, 1);
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

int redir_in_double(t_cmd *cmd, size_t id_redir, t_shell *shell)
{
	size_t  i;
	size_t  j;
	char	**av;

	i = 0;
	av = malloc(sizeof(char*) * cmd->ac);
	while (i < id_redir)
	{
		av[i] = cmd->av[i];
		i++;
	}
	j = i;
	if (cmd->av[i])
	{
		free(cmd->av[i]);
		i++;
		if (cmd->av[i])
		{
			if (heredoc_handler(cmd, cmd->av[i], shell) == -1)
			{
				cmd->valid = 0;
			}
			if (cmd->fd_in < 0)
			{
				cmd->valid = 0;
				print_error("open: ", cmd->av[i], NULL, errno);
			}
			else
				cmd->flags |= E_FILEIN;
		}
		else
		{
			cmd->valid = 0;
			print_error(NULL, NULL, "syntax error near unexpected token `newline'\n", 258);
		}
	}
	if (cmd->av[i])
	{
		free(cmd->av[i]);
		i++;
        while (i < cmd->ac)
		{
			av[j] = cmd->av[i];
			i++;
			j++;
		}
		av[j] = NULL;
		free(cmd->av);
	}
	cmd->av = av;
	cmd->ac = j;
	return (0);
}
