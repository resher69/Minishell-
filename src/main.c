
#include "minishell.h"

int	split_usr_input(char *usr_input, t_shell *shell)
{
	t_cmd	**cmd_tmp;
	size_t	i;
	size_t	j;
	size_t	n_pipe;
	int		quote;

	i = 0;
	shell->n_cmd = 1;
	i += skip_ifs(&usr_input[i]);
	while (usr_input && usr_input[i])
	{
		if (usr_input[i] == '|' && i == 0 + (size_t)skip_ifs(usr_input))
			return (printf("Minishell : parse error near `|'\n"));
		quote = get_quote(usr_input[i]);
		if (usr_input[i] && usr_input[i] == '|' && quote == QUOTE_NONE)
			shell->n_cmd++;
		i++;
		i += skip_ifs(&usr_input[i]);
	}
	cmd_tmp = malloc(sizeof(t_cmd *) * (shell->n_cmd + 1));
	if (!cmd_tmp)
		return (1);
	i = 0;
	n_pipe = 0;
	while (usr_input[i] && n_pipe < shell->n_cmd)
	{
		j = 0;
		quote = get_quote(usr_input[i]);
		while (usr_input[i + j] && (usr_input[i + j] != '|' || quote != QUOTE_NONE))
		{
			j++;
			quote = get_quote(usr_input[i + j]);
		}
		cmd_tmp[n_pipe] = malloc(sizeof(t_cmd *));
		if (!cmd_tmp[n_pipe])
			return (1);
		cmd_tmp[n_pipe]->av = malloc(sizeof(char *));
		*cmd_tmp[n_pipe]->av = ft_strldup(usr_input + i, j);
		i += j + 1;
		i += skip_ifs(usr_input + i);
		n_pipe++;
	}
	cmd_tmp[n_pipe] = NULL;
	size_t k = 0;
	while (cmd_tmp && cmd_tmp[k])
	{
		if (cmd_tmp[k]->av[0])
			k++;
		else
			break ;
	}
	if (k < shell->n_cmd)
		return (printf("Minishell : parse error near `|'\n"));
	shell->usr_cmd = cmd_tmp;
	return (0);
}

int main(int ac, char **av, char **envp)
{
	t_shell	*shell;
	char	*usr_input;
	int		err;

	(void)av;
	if (ac != 1)
		return (printf("No arg needed\n") * 0 + 1);
	init_env(envp);
	shell = malloc(sizeof(t_shell));
	while (1)
	{
		usr_input = readline("Minishell$>");
		if (usr_input[0])
		{
			err = split_usr_input(usr_input, shell);
			if (!err)
			{
				size_t i = 0;
				free(usr_input);
				while (i < shell->n_cmd)
				{
					shell->usr_cmd[i] = expand(*shell->usr_cmd[i]->av);
					printf("printing argv\n");
					while (shell->usr_cmd[i] && shell->usr_cmd[i]->av && *shell->usr_cmd[i]->av)
					{
						if (strncmp(*shell->usr_cmd[i]->av, "<", 1) == 0)
							printf("redir in simple\n");
						else if (strncmp(*shell->usr_cmd[i]->av, "<<", 2) == 0)
							printf("redir in double\n");
						else if (strncmp(*shell->usr_cmd[i]->av, ">", 1) == 0)
							printf("redir out simple\n");
						else if (strncmp(*shell->usr_cmd[i]->av, ">>", 2) == 0)
							printf("redir out double\n");
						printf("|%s|\n", *shell->usr_cmd[i]->av);
						shell->usr_cmd[i]->av++;
					}
					i++;
				}
			}
		}
	}
	return (0);
}
