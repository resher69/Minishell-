/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ebellon <ebellon@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/07/23 14:08:46 by ebellon           #+#    #+#             */
/*   Updated: 2022/03/06 16:33:21 by ebellon          ###   ########lyon.fr   */
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
		ft_putstr_fd("dup2 failed\n", STDERR_FILENO);
		ft_free_job_exit(cmd_arg, exec_path, locations, 1);
	}
	if (close(fd[0]) < 0)
	{
		ft_putstr_fd("close failed\n", STDERR_FILENO);
		ft_free_job_exit(cmd_arg, exec_path, locations, 1);
	}
}

int	ft_safe_close(int fd)
{
	if (close(fd) < 0)
	{
		ft_putstr_fd("close failed\n", STDERR_FILENO);
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
		exit (1);
	}
}

int	ft_waitpids(t_shell *shell)
{
	size_t	i;
	int		ret;
	int		wstatus;

	i = 0;
	while (i < shell->i)
	{
		waitpid(shell->pid_ar[i++], &wstatus, 0);
		if (WIFEXITED(wstatus))
			ret = WEXITSTATUS(wstatus);
	}
	return (ret);
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
	while (envp && *envp)
	{
		if (strncmp(*envp, "PATH=", 5) == 0)
			return (ft_split(*envp + 5, ':'));
		envp++;
	}
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

char	*get_exec_path(char *exec_name, char **locations)
{
	char	*buf;
	int		fd_buf;

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
	if (strcmp(exec_name, "exit"))
	{
		ft_putstr_fd("Minishell : command not found : ", STDERR_FILENO);
		ft_putstr_fd(exec_name, STDERR_FILENO);
		ft_putstr_fd("\n", STDERR_FILENO);
	}
	return (NULL);
}
