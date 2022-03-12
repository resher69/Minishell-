/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ebellon <ebellon@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/07/23 14:08:46 by ebellon           #+#    #+#             */
/*   Updated: 2022/03/12 19:48:54 by ebellon          ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*ft_strndup(char *src, size_t size)
{
	char	*dst;
	size_t	i;

	if (!src)
		return (NULL);
	dst = (char *)calloc(sizeof(char), (size + 1));
	if (!dst)
		return (NULL);
	i = 0;
	while (i < size)
	{
		dst[i] = src[i];
		i++;
	}
	return (dst);
}

void	ft_putchar_fd(char c, int fd)
{
	write(fd, &c, 1);
}

void	ft_putstr_fd(char *s, int fd)
{
	if (s)
		while (*s)
			ft_putchar_fd(*s++, fd);
}

void	ft_dup_close(int fd[2], char **cmd_arg,
					char *exec_path, char **locations)
{
	if (dup2(fd[0], fd[1]) < 0)
	{
		print_error("dup2: ", NULL, NULL, errno);
		ft_free_job_exit(cmd_arg, exec_path, locations, 1);
	}
	if (close(fd[0]) < 0)
	{
		print_error("close: ", NULL, NULL, errno);
		ft_free_job_exit(cmd_arg, exec_path, locations, 1);
	}
}

int	ft_safe_close(int fd)
{
	if (close(fd) < 0)
	{
		print_error("close: ", NULL, NULL, errno);
		return (1);
	}
	return (0);
}

void	ft_free_job_exit(char **cmd_arg, char *exec_path,
						char **locations, int err)
{
	ft_free_tab(cmd_arg);
	free(exec_path);
	if (err > 0)
	{
		ft_free_tab(locations);
		exit (g_wstatus);
	}
}

void	ft_waitpids(t_shell *shell)
{
	size_t	i;
	int		wstatus;
	int		sig;

	i = 0;
	sig = 0;
	while (i < shell->i)
	{
		waitpid(shell->pid_ar[i++], &wstatus, 0);
		if (WIFEXITED(wstatus))
			g_wstatus = WEXITSTATUS(wstatus);
		if (WIFSIGNALED(wstatus) && !sig)
		{
			sig_child(WTERMSIG(wstatus));
			sig = 1;
		}
	}
}

static size_t	count_sep(const char *s, char c)
{
	size_t	i;
	size_t	nb_sep;
	char	*s_cpy;

	s_cpy = (char *)s;
	i = 0;
	nb_sep = 0;
	while (s_cpy[i])
	{
		while ((s_cpy[i] == c) && (s_cpy[i]))
			i++;
		while ((s_cpy[i] != c) && (s_cpy[i]))
			i++;
		if (s_cpy[i] == c && (s_cpy[i]))
			nb_sep++;
		if (s_cpy[i - 1] != c && (s_cpy[i] == 0))
			nb_sep++;
	}
	return (nb_sep);
}

static char	**free_tab(char **strs)
{
	size_t	i;

	i = 0;
	while (strs[i])
	{
		free(strs[i]);
		i++;
	}
	free(strs);
	return (NULL);
}

char	**ft_split(char const *s, char c)
{
	char	**strs_split;
	char	*start;
	size_t	i;

	strs_split = calloc(sizeof(char *), (count_sep(s, c) + 1));
	if ((count_sep(s, c) == 0))
		return (strs_split);
	i = 0;
	while (*s)
	{
		while (*s && *s == c)
			s++;
		start = (char *)s;
		while (*s && *s != c)
			s++;
		if (start != s)
		{
			strs_split[i++] = ft_strndup(start, (s - start));
			if (!strs_split)
				return (free_tab(strs_split));
		}
	}
	strs_split[i] = NULL;
	return (strs_split);
}

size_t	ft_strnchr(const char *const str, const char c)
{
	size_t	i;

	i = 0;
	while (str && str[i])
	{
		if (str[i] == c)
			return (i);
		i++;
	}
	return (i);
}

void	ft_free_tab(char **tab)
{
	int	i;

	i = 0;
	while (tab[i])
		free(tab[i++]);
	free(tab);
}

char	**get_locations(char **envp)
{
	int	i;

	i = 0;
	while (envp && envp[i])
	{
		if (strncmp(envp[i], "PATH=", 5) == 0)
			return (ft_split(envp[i] + 5, ':'));
		i++;
	}
	i = 0;
	while (envp && envp[i])
	{
		free(envp[i]);
		i++;
	}
	free(envp);
	return (NULL);
}

char	*concat_path(char *location, char *exec)
{
	size_t	size;
	char	*out;

	size = ft_strlen(location) + ft_strlen(exec) + 1;
	out = malloc((size + 1) * sizeof(char));
	if (!out)
		return (NULL);
	while (location && *location)
		*out++ = *location++;
	*out++ = '/';
	while (exec && *exec)
		*out++ = *exec++;
	*out = '\0';
	return (out - size);
}

int	is_builtin(char *str, t_shell *shell, char *f_arg)
{
	if (!ft_strcmp(str, "echo"))
		return (1);
	else if (!ft_strcmp(str, "cd"))
		return (1);
	else if (!ft_strcmp(str, "pwd"))
		return (1);
	else if (!ft_strcmp(str, "export"))
		return (1);
	else if (!ft_strcmp(str, "unset"))
		return (1);
	else if (!ft_strcmp(str, "env"))
		return (1);
	else if (!ft_strcmp(str, "exit"))
	{
		if (shell->n_cmd == 1)
			shell->do_exit = 1;
		if (f_arg)
			shell->exit = ft_atoi(ft_strchrstr(f_arg, "-0123456789"));
		return (1);
	}
	return (0);
}

char	*get_exec_path(char *exec_name, char **locations,
	t_shell *shell, char *f_arg)
{
	char	*buf;
	int		fd_buf;

	if (is_builtin(exec_name, shell, f_arg))
		return (ft_strldup(exec_name, ft_strlen(exec_name)));
	if (*exec_name)
	{
		if (strncmp(exec_name, "./", 2) == 0)
		{
			fd_buf = open(exec_name, O_RDONLY | S_IRUSR | S_IRGRP | S_IROTH);
			if (fd_buf > 0)
			{
				close(fd_buf);
				return (ft_strldup(exec_name, ft_strlen(exec_name)));
			}
		}
		while (locations && *locations)
		{
			buf = concat_path(*locations, exec_name);
			fd_buf = open(buf, O_RDONLY | S_IRUSR | S_IRGRP | S_IROTH);
			if (fd_buf > 0)
			{
				close(fd_buf);
				return (buf);
			}	
			free(buf);
			locations++;
		}
	}
	print_error(NULL, exec_name, "command not found\n", 127);
	return (NULL);
}
