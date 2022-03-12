/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bi_cd.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ebellon <ebellon@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/12 17:00:09 by agardet           #+#    #+#             */
/*   Updated: 2022/03/12 19:01:24 by ebellon          ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_env_var	*find_env_var(t_shell *shell, char *name)
{
	t_env_var	*elem;

	elem = shell->env;
	while (elem)
	{
		if (!ft_strcmp(elem->name, name))
			break ;
		elem = elem->next;
	}
	return (elem);
}

char	*replace_by_home_path(t_shell *shell)
{
	t_env_var	*elem;
	char		*path;

	elem = find_env_var(shell, "HOME");
	if (!elem || !elem->value)
		return (NULL);
	else
		path = ft_strldup(elem->value, ft_strlen(elem->value));
	if (!path)
		return (NULL);
	return (path);
}

static void	set_pwd(t_shell *shell, char *name)
{
	t_env_var	*current;
	char		cwd[MAXPATHLEN];

	if (!getcwd(cwd, MAXPATHLEN))
	{
		print_error("getcwd: ", NULL, NULL, errno);
		g_wstatus = EXIT_FAILURE;
	}
	current = find_env_var(shell, name);
	if (current)
	{
		free(current->value);
		current->value = ft_strjoin("=", ft_strldup(cwd, ft_strlen(cwd)), 2);
	}
	if (current && !current->value)
	{
		print_error("malloc: ", NULL, NULL, ENOMEM);
		g_wstatus = EXIT_FAILURE;
	}
}

void	bi_cd(char **av, t_shell *shell)
{
	char	*path;

	g_wstatus = EXIT_SUCCESS;
	if (!av[1])
	{
		path = replace_by_home_path(shell);
		if (!path)
		{
			print_error("cd: ", NULL, "HOME not set\n", EXIT_FAILURE);
			return ;
		}
	}
	else
		path = ft_strldup(av[1], ft_strlen(av[1]));
	set_pwd(shell, "OLDPWD");
	if (path && chdir(path) == -1)
	{
		print_error("cd: ", path, NULL, errno);
		g_wstatus = EXIT_FAILURE;
	}
	else if (!path)
	{
		print_error("malloc: ", NULL, NULL, ENOMEM);
		g_wstatus = EXIT_FAILURE;
	}
	set_pwd(shell, "PWD");
}
