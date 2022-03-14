/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ebellon <ebellon@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/13 17:53:00 by agardet           #+#    #+#             */
/*   Updated: 2022/03/14 20:33:44 by ebellon          ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	env_new(t_shell *s, char *var)
{
	t_env_var	*new;
	t_env_var	*current;

	new = ft_calloc(1, sizeof(t_env_var));
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

int	create_variable(t_shell *s, char *cmd, char *name)
{
	t_env_var	*export;

	export = s->env;
	while (export && ft_strcmp(name, export->name))
		export = export->next;
	if (export)
		env_del(s, export);
	return (env_new(s, cmd));
}

void	create_env_variable(t_shell *s, char **av)
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
