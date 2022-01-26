
#include "minishell.h"

ssize_t		get_next_var_len(char *cmd, char **str)
{
	size_t	i;
	size_t	j;

	i = 0;
	j = 0;
	while (cmd && is_valid_var_char(cmd[i]))
		i++;
	*str = malloc((i + 1) * sizeof(char));
	if (!str && *str)
		return (-1);
	while (j < i)
	{
		(*str)[j] = cmd[j];
		j++;
	}
	(*str)[j] = 0;
	return (i);
}

char	*cpy_expand(char *str, size_t size, int quote)
{
	size_t	i;
	size_t	j;
	char *expand;
	char *buffer;
	size_t	off;

	expand = malloc((size + 1) * sizeof(char));
	i = 0;
	j = 0;
	while (str && str[i])
	{
		quote = get_quote(str[i]);
		if (str[i] != '$' || quote == QUOTE_SINGLE)
		{
			expand[j] = str[i];
			i++;
			j++;
		}
		else
		{
			off = get_next_var_len(str + i + 1, &buffer) + 1;
			i += off;
			ft_strlcpy(expand + j, get_env(buffer), ft_strlen(get_env(buffer)));
			j += ft_strlen(get_env(buffer));
			free(buffer);
		}
	}
	expand[j] = 0;
	return (expand);
}

int expand_variables(t_cmd *cmd)
{
	size_t	expanded_size;
	size_t	i;
	char 	*buffer;
	int		quote;

	i = 0;
	expanded_size = 0;
	while (*cmd->av && (*cmd->av)[i])
	{
		quote = get_quote((*cmd->av)[i]);
		if ((*cmd->av)[i] != '$' || quote == QUOTE_SINGLE)
		{
			i++;
			expanded_size++;
		}
		else
		{
			int off = get_next_var_len(*cmd->av + i + 1, &buffer) + 1;
			i += off;
			expanded_size += ft_strlen(get_env(buffer));
			free(buffer);
		}
	}
	*cmd->av = cpy_expand(*cmd->av, expanded_size, 0);
	return (0);  	  
}
