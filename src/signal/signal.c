#include "minishell.h"

void	sig_int(int sig)
{
	(void)sig;
	rl_replace_line("", 0);
	ft_putchar_fd('\n', STDERR_FILENO);
	rl_on_new_line();
	rl_redisplay();
	// g_error_number = 1;
}


void	heredoc_sig_int(int sig)
{
	(void)sig;
	ft_putchar_fd('\n', STDERR_FILENO);
	// g_error_number = 1;
	// exit(g_error_number);
    exit(1);
}

// void	sig_child(int sig)
// {
// 	if (sig == SIGINT)
// 	{
// 		ft_putchar_fd('\n', STDERR_FILENO);
// 		// g_error_number = 130;
// 	}
// 	else if (sig == SIGQUIT)
// 	{
// 		ft_putstr_fd("Quit: 3\n", STDERR_FILENO);
// 		// g_error_number = 131;
// 	}
// }