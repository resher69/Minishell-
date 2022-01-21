
#include "minishell.h"

t_env_var	*g_env = NULL;

char	*get_env(char *var_name)
{
	t_env_var *current;
	
	current = g_env;
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

void	add_env(t_env_var *new)
{
	t_env_var	*current;

	if (g_env)
	{
		current = g_env;
		while (current && current->next)
			current = current->next;
		current->next = new;
	}
	else
		g_env = new;
}

void	init_env(char **envp)
{
	t_env_var	*var;
	size_t		i;
	size_t		equal_pos;

	i = 0;
//	assertfalse(g_env);
	while (envp && *envp)
	{
		equal_pos = ft_strchr(*envp, '=');
		var = new_env_var(
			ft_strldup(*envp, equal_pos),
			ft_strldup(*envp + equal_pos + 1, ft_strlen(*envp) - equal_pos - 1)
		);
//		assertfalse(var);
		add_env(var);
		envp++;
	}	
}



