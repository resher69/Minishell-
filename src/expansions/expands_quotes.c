
#include "minishell.h"

int expand_quotes(t_cmd *cmd)
{
	size_t	i;
	char	*expand;

	i = 0;
	while (i < cmd->ac)
	{
		if (*cmd->av[i] && (*cmd->av[i] == '\'' || *cmd->av[i] == '\"'))
		{
			expand = ft_strldup(cmd->av[i] + 1, ft_strlen(cmd->av[i]) - 2);
			free(cmd->av[i]);
			cmd->av[i] = expand;
		}
		i++;
	}
	printf("printing argv\n");
	while (cmd->av && *cmd->av)
	{
		printf("|%s|\n", *cmd->av);
		cmd->av++;
	}
	printf ("\n");
	return (0); 
}
