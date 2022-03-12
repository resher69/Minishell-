/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bi_export.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ebellon <ebellon@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/12 17:29:07 by ebellon           #+#    #+#             */
/*   Updated: 2022/03/12 21:00:16 by ebellon          ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	ft_isalpha(int c)
{
	return (('a' <= c && c <= 'z') || ('A' <= c && c <= 'Z'));
}

int	ft_isalnum(int c)
{
	return (ft_isalpha(c) || ft_isdigit(c));
}

int	is_word(char *str)
{
	size_t	i;

	i = 0;
	if (ft_isdigit(str[i]))
		return (0);
	while (str[i])
	{
		if (!ft_isalnum(str[i]) && str[i] != '_')
			return (0);
		i++;
	}
	return (1);
}

char	*ft_strchr_bis(const char *str, int c)
{
	while (*str)
	{
		if (*str == c)
			return ((char *)str);
		str++;
	}
	if (*str == c)
		return ((char *)str);
	return (NULL);
}

static void	index_list(t_shell *s)
{
	t_env_var	*elem;
	t_env_var	*next;

	elem = s->env;
	while (elem)
	{
		elem->index = 1;
		next = s->env;
		while (next)
		{
			if (ft_strcmp(elem->name, next->name) > 0)
				elem->index++;
			next = next->next;
		}
		elem = elem->next;
	}
}

char	*ft_substr(char *src, unsigned int start, size_t len)
{
	char	*new;
	size_t	new_max_len;

	if (start > ft_strlen(src))
		return (NULL);
	new_max_len = ft_strlen(&src[start]);
	if (len > new_max_len)
		len = new_max_len;
	new = calloc((len + 1), sizeof(char));
	if (!new)
		return (NULL);
	ft_strlcpy(new, &src[start], len + 1);
	return (new);
}

static void	print_export(t_shell *s, int fd)
{
	size_t		i;
	t_env_var	*export;
	size_t		env_size;

	i = 1;
	env_size = 0;
	export = s->env;
	while (export)
	{
		export = export->next;
		env_size++;
	}
	while (i <= env_size)
	{
		export = s->env;
		while (export)
		{
			if (i == export->index)
			{
				ft_putstr_fd("declare -x ", fd);
				ft_putstr_fd(export->name, fd);
				if (export->value)
				{
					ft_putstr_fd("=\"", fd);
					ft_putstr_fd(export->value + 1, fd);
					ft_putstr_fd("\"", fd);
				}
				ft_putstr_fd("\n", fd);
			}
			export = export->next;
		}
		i++;
	}
}

int	env_new(t_shell *s, char *var)
{
	t_env_var	*new;
	t_env_var	*current;

	new = calloc(1, sizeof(t_env_var));
	if (!new)
		return (-1);
	new->name = ft_substr(var, 0, ft_strchr(var, '=') - 1);
	if (!new->name)
		return (-1);
	new->value = NULL;
	if (ft_strchr(var, '='))
		new->value = ft_substr(ft_strchr_bis(var, '='), 0, ft_strlen(var));
	new->next = NULL;
	current = s->env;
	while (current->next)
		current = current->next;
	current->next = new;
	return (0);
}

void	env_del(t_shell *s, t_env_var *elem)
{
	t_env_var	*current;

	current = s->env;
	if (current == elem)
		s->env = current->next;
	else
	{
		while (current->next != elem)
			current = current->next;
		current->next = elem->next;
	}
	free(elem->name);
	free(elem->value);
	free(elem);
}
static int	create_variable(t_shell *s, char *cmd, char *name)
{
	t_env_var	*export;

	export = s->env;
	while (export && ft_strcmp(name, export->name))
		export = export->next;
	if (export)
		env_del(s, export);
	return (env_new(s, cmd));
}

static void	create_env_variable(t_shell *s, char **av)
{
	size_t	i;
	char	*name;

	i = 1;
	while (av[i])
	{
		if (!ft_strchr_bis(av[i], '='))
			name = ft_strldup(av[i], ft_strlen(av[i]));
		else
			name = ft_substr(av[i], 0, ft_strchr(av[i], '=') - 1);
		if (!name)
			print_error("malloc: ", NULL, NULL, ENOMEM);
		else if (!av[i][0] || av[i][0] == '='
			|| !is_word(name))
			print_error("export: ", av[i],
				"not a valid identifier\n", EXIT_FAILURE);
		else if (create_variable(s, av[i], name) == -1)
			print_error("malloc: ", NULL, NULL, ENOMEM);
		free(name);
		i++;
	}
}

void	bi_export(t_shell *s, char **av, int fd)
{
	g_wstatus = EXIT_SUCCESS;
	if (av[1])
		create_env_variable(s, av);
	else
	{
		index_list(s);
		print_export(s, fd);
	}
}
