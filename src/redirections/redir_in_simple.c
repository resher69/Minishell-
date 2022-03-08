#include "minishell.h"

int redir_in_simple(t_cmd *cmd, size_t id_redir)
{
	size_t  i;
	size_t  j;
	char	**av;

	i = 0;
	av = malloc(sizeof(char*) * cmd->ac);
	while (i < id_redir)
	{
		av[i] = cmd->av[i];
		i++;
	}
	j = i;
	if (cmd->av[i] && strlen(cmd->av[i]) == 1)
	{
		free(cmd->av[i]);
		i++;
		cmd->fd_in = open(cmd->av[i], O_RDONLY, 0644);
		if (cmd->fd_in < 0)
			printf("Minishell : no such file or directory: %s\n", cmd->av[i]);
		else
		{
			cmd->flags |= E_FILEIN;
			cmd->flags &= ~E_PIPEIN;
		}
		// printf("redir in simple done\n");
	}
	else if (cmd->av[i])
	{
		cmd->fd_in = open(cmd->av[i] + 1, O_RDONLY, 0644);
		if (cmd->fd_in < 0)
			printf("Minishell : no such file or directory: %s\n", cmd->av[i] + 1);
		else
		{
			cmd->flags |= E_FILEIN;
			cmd->flags &= ~E_PIPEIN;
		}
		// printf("redir in simple done\n");
	}
	if (cmd->av[i])
	{
		free(cmd->av[i]);
		i++;
		while (i < cmd->ac)
		{
			av[j] = cmd->av[i];
			i++;
			j++;
		}
		av[j] = NULL;
		free(cmd->av);
	}
	cmd->av = av;
	cmd->ac = j;
	return (0);
}