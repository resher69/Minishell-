#include "minishell.h"

void	print_error(char *cmd, char *value, char *error, int status)
{
	errno = status;
	g_wstatus = status;
	ft_putstr_fd("MiniSHEEEESH: ", STDERR_FILENO);
	if (cmd)
		ft_putstr_fd(cmd, STDERR_FILENO);
	if (value)
	{
		ft_putstr_fd(value, STDERR_FILENO);
		ft_putstr_fd(": ", STDERR_FILENO);
	}
	if (errno != 258 && errno != EXIT_FAILURE
		&& errno != 127 && errno != 255 && errno != 3)
		perror(NULL);
	if (error)
		ft_putstr_fd(error, STDERR_FILENO);
}