#include "minishell.h"

void	ft_child(t_cmd *const job, const int prev_in,
				char *exec_path, char **locations, char **envp)
{
	if (exec_path == NULL)
		ft_free_job_exit(job->av, exec_path, locations, 1);
	if (job->flags & E_FILEIN)
	{
		if (job->fd_in < 0)
			ft_free_job_exit(job->av, exec_path, locations, 1);
		ft_dup_close((int [2]){job->fd_in, STDIN_FILENO},
			job->av, exec_path, locations);
	}
	else if (job->flags & E_PIPEIN)
		ft_dup_close((int [2]){prev_in, STDIN_FILENO},
			job->av, exec_path, locations);
	if (job->flags & E_FILEOUT)
	{
		if (job->fd_out < 0)
			ft_free_job_exit(job->av, exec_path, locations, 1);
		ft_dup_close((int [2]){job->fd_out, STDOUT_FILENO},
			job->av, exec_path, locations);
	}
	else if (job->flags & E_PIPEOUT)
		ft_dup_close((int [2]){job->pipe_fd[1], STDOUT_FILENO},
			job->av, exec_path, locations);
	signal(SIGINT, NULL);
	signal(SIGQUIT, NULL);
	if (job->valid)
	{
		execve(exec_path, job->av, envp);
		ft_putstr_fd("execve failed \n", STDERR_FILENO);
	}
	ft_free_job_exit(job->av, exec_path, locations, 1);
}

int	ft_parent(t_cmd *const job, pid_t pid, const int prev_in, t_shell *shell)
{
	int	err;

	err = 0;
	if (job->flags & E_PIPEOUT)
		err += ft_safe_close(job->pipe_fd[1]);
	else if (job->flags & E_FILEOUT && job->fd_out > 0)
		err += ft_safe_close(job->fd_out);
	if (job->flags & E_PIPEIN)
		err += ft_safe_close(prev_in);
	else if (job->flags & E_FILEIN && job->fd_in > 0)
		err += ft_safe_close(job->fd_in);
	shell->pid_ar[shell->i++] = pid;
	return (err);
}

void ft_pipex(t_cmd *cmd, char **envp, t_shell *shell)
{
    const int	prev_in = cmd->pipe_fd[0];
	pid_t		pid;
	char		*exec_path;
	int			err;
    char        **locations;

	err = 0;
	exec_path = NULL;
    //modifie get_location to work with chained list
    locations = get_locations(envp);
	if (cmd->flags & E_PIPEOUT)
		pipe(cmd->pipe_fd);
	if (*cmd->av)
		exec_path = get_exec_path(*cmd->av, locations);
	pid = fork();
	if (pid < 0)
	{
		ft_putstr_fd("fork failed", STDERR_FILENO);
		ft_free_job_exit(cmd->av, exec_path, locations, 1);
	}
	if (pid == 0)
		ft_child(cmd, prev_in, exec_path, locations, envp);
	else
		err = ft_parent(cmd, pid, prev_in, shell);
	ft_free_job_exit(cmd->av, exec_path, locations, err);
}