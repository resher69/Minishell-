
#include "minishell.h"

int main(int ac, char **av, char **envp)
{
	(void)ac;
	(void)av;
	init_env(envp);
	printf("%s\n", get_env("HOME"));
	return (0);
}
