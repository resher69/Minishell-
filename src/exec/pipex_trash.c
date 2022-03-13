/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex_trash.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ebellon <ebellon@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/13 17:53:43 by ebellon           #+#    #+#             */
/*   Updated: 2022/03/13 18:04:54 by ebellon          ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	**list_to_char(t_env_var *env)
{
	char		**envp;
	t_env_var	*current;
	size_t		i;

	i = 0;
	current = env;
	while (current)
	{
		current = current->next;
		i++;
	}
	envp = malloc(sizeof(char *) * i + 1);
	if (!envp)
		return (NULL);
	i = 0;
	current = env;
	while (current)
	{
		envp[i] = ft_strjoin(current->name, "=", 0);
		envp[i] = ft_strjoin(envp[i], current->value, 1);
		current = current->next;
		i++;
	}
	envp[i] = NULL;
	return (envp);
}

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
