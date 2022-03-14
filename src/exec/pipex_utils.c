/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ebellon <ebellon@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/07/23 14:08:46 by ebellon           #+#    #+#             */
/*   Updated: 2022/03/14 20:35:15 by ebellon          ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	**get_locations(char **envp)
{
	int	i;

	i = 0;
	while (envp && envp[i])
	{
		if (ft_strncmp(envp[i], "PATH=", 5) == 0)
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

char	*find_in_path(char *exec_name, char **locations)
{
	char	*buf;
	int		fd_buf;

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
	return (NULL);
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
		if (ft_strncmp(exec_name, "./", 2) == 0)
		{
			fd_buf = open(exec_name, O_RDONLY | S_IRUSR | S_IRGRP | S_IROTH);
			if (fd_buf > 0)
			{
				close(fd_buf);
				return (ft_strldup(exec_name, ft_strlen(exec_name)));
			}
		}
		buf = find_in_path(exec_name, locations);
		if (buf)
			return (buf);
	}
	print_error(NULL, exec_name, "command not found\n", 127);
	return (NULL);
}
