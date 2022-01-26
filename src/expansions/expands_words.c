
#include "minishell.h"

int expand_words(t_cmd *cmd)
{
	int 	i;
	int		j;
	size_t  n_words;
	char 	**argv;
	int		quote;

	i = 0;
	n_words = 1;
	while (*cmd->av && (*cmd->av)[i])
	{
		quote = get_quote((*cmd->av)[i]);
		if (!is_ifs((*cmd->av)[i]) || quote != QUOTE_NONE)
			i++;
		else
		{
			n_words++;
			i += skip_ifs(*cmd->av + i);
		}
	}


	argv = malloc(sizeof(char *) * (n_words + 1));
	if (!argv)
		return (1);

	i = 0;
	while (*cmd->av && (*cmd->av)[i])
	{
		quote = get_quote((*cmd->av)[i]);
		j = 0;
		while ((*cmd->av)[i + j] && (!is_ifs((*cmd->av)[i + j]) || quote != QUOTE_NONE))
		{
			j++;
			quote = get_quote((*cmd->av)[i + j]);
		}
		*argv = malloc((j + 1) * sizeof(char));
		if (!argv)
			return (1);
		ft_strlcpy(*argv, *cmd->av + i, j);
		(*argv++)[j] = 0;
		i += j;
		i += skip_ifs(*cmd->av + i);
	}
	*argv = NULL;
	argv -= n_words;
	free(*cmd->av);
	free(cmd->av);
	cmd->av = argv;



	printf("printing argv\n");
	while (cmd->av && *cmd->av)
	{
		printf("|%s|\n", *cmd->av);
		cmd->av++;
	}
	printf ("\n");




	return (0); 
}