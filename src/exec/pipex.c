#include "minishell.h"

char **list_to_char(t_env_var *env)
{
	char		**envp;
	t_env_var	*current;
	size_t		i;

	i = 0;
	current = env;
	while (current)
	{
		current = current->next;
		i++;
	}
	envp = malloc(sizeof(char*) * i + 1);
	if (!envp)
		return (NULL);
	i = 0;
	current = env;
	while (current)
	{
		envp[i] = ft_strjoin(current->name, "=", 0);
		envp[i] = ft_strjoin(envp[i], current->value, 1);
		current = current->next;
		i++;
	}
	envp[i] = NULL;
	return (envp);
}

void	ft_child(t_cmd *const job, const int prev_in,
				char *exec_path, t_shell *shell)
{
	signal(SIGINT, NULL);
	signal(SIGQUIT, NULL);
	if (exec_path == NULL)
		ft_free_job_exit(job->av, exec_path, shell->locations, 1);
	if (job->flags & E_FILEIN)
	{
		if (job->fd_in < 0)
			ft_free_job_exit(job->av, exec_path, shell->locations, 1);
		ft_dup_close((int [2]){job->fd_in, STDIN_FILENO},
			job->av, exec_path, shell->locations);
	}
	else if (job->flags & E_PIPEIN)
		ft_dup_close((int [2]){prev_in, STDIN_FILENO},
			job->av, exec_path, shell->locations);
	if (job->flags & E_FILEOUT)
	{
		if (job->fd_out < 0)
			ft_free_job_exit(job->av, exec_path, shell->locations, 1);
		ft_dup_close((int [2]){job->fd_out, STDOUT_FILENO},
			job->av, exec_path, shell->locations);
	}
	else if (job->flags & E_PIPEOUT)
		ft_dup_close((int [2]){job->pipe_fd[1], STDOUT_FILENO},
			job->av, exec_path, shell->locations);
	if (job->valid)
	{
		execve(exec_path, job->av, list_to_char(shell->env));
		ft_putstr_fd("execve failed \n", STDERR_FILENO);
	}
	ft_free_job_exit(job->av, exec_path, shell->locations, 1);
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

void ft_pipex(t_cmd *cmd, t_shell *shell)
{
    const int	prev_in = cmd->pipe_fd[0];
	pid_t		pid;
	char		*exec_path;
	int			err;

	err = 0;
	exec_path = NULL;
    shell->locations = get_locations(list_to_char(shell->env));
	if (cmd->flags & E_PIPEOUT)
		pipe(cmd->pipe_fd);
	if (*cmd->av)
		exec_path = get_exec_path(*cmd->av, shell->locations);
	pid = fork();
	if (pid < 0)
	{
		ft_putstr_fd("fork failed", STDERR_FILENO);
		ft_free_job_exit(cmd->av, exec_path, shell->locations, 1);
	}
	if (pid == 0)
		ft_child(cmd, prev_in, exec_path, shell);
	else
		err = ft_parent(cmd, pid, prev_in, shell);
	ft_free_job_exit(cmd->av, exec_path, shell->locations, err);
}