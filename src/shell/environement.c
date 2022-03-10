/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   environement.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ebellon <ebellon@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/03 16:17:14 by agardet           #+#    #+#             */
/*   Updated: 2022/03/10 19:56:56 by ebellon          ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*get_env(char *var_name, t_shell *shell)
{
	t_env_var	*current;

	current = shell->env;
	while (current)
	{
		if (ft_strcmp(current->name, var_name) == 0)
			return (current->value);
		current = current->next;
	}
	return (NULL);
}

t_env_var	*new_env_var(char *name, char *value)
{
	t_env_var	*new;

	new = malloc(sizeof(t_env_var));
	if (!new)
		return (NULL);
	new->name = name;
	new->value = value;
	new->next = 0;
	return (new);
}

void	add_env(t_env_var *new, t_shell *shell)
{
	t_env_var	*current;

	if (shell->env)
	{
		current = shell->env;
		while (current && current->next)
			current = current->next;
		current->next = new;
	}
	else
		shell->env = new;
}

void	init_env(char **envp, t_shell *shell)
{
	t_env_var	*var;
	size_t		equal_pos;

	while (envp && *envp)
	{
		equal_pos = ft_strchr(*envp, '=');
		var = new_env_var(ft_strldup(*envp, equal_pos), \
				ft_strldup(*envp + equal_pos + 1, \
					ft_strlen(*envp) - equal_pos - 1));
		add_env(var, shell);
		envp++;
	}	
}
