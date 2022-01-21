
#include "minishell.h"

static int	is_ifs(char c)
{
	return (c == ' ' || c == '\t' || c == '\n');
}

static int	is_valid_var_char(char c)
{
	return (!is_ifs(c) && c != 0 && c != 61 && c != 36 && c != 32 && ((c >= 97 && <= 126) || (c >= 65 c <= 91));
}

size_t		get_next_var_len(char *cmd)
{
	size_t	i;

	i = 0;
	while (cmd && is_valid_var_char(cmd[i]))
		i++;
	return (i);
}

int expand_variables(t_cmd *cmd)
{
    char    *expanded;
	size_t	expanded_size;
	size_t	i;

	i = 0;
	while (*cmd->av && *cmd->av[i])
	{
		i = ft_strchr(*cmd->av, '$');
		cmd->av++;
		size_t var_size = get_next_var_len(cmd + i);

		expanded_size += ft_strlen(get_env(;
		//add len parsed $ to expanded_size
		//i += var len ($HOME = 4)
		expanded_size++;
	}
	return (0);  	  
}
