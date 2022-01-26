
#include "minishell.h"

int main(int ac, char **av, char **envp)
{
	(void)ac;
	(void)av;
	init_env(envp);
	expand("je       suis gay           'homosexuel         et'           \"transgenre puis\" $HOME homophobe");
	return (0);
}
