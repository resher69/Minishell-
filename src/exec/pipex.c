/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ebellon <ebellon@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/12 17:01:25 by ebellon           #+#    #+#             */
/*   Updated: 2022/03/12 18:11:27 by ebellon          ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	**list_to_char(t_env_var *env)
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
	envp = malloc(sizeof(char *) * i + 1);
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

void	exec_builtins(char *exec_path, char **av, t_shell *shell)
{
	if (!ft_strcmp(exec_path, "echo"))
		bi_echo(av);
	else if (!ft_strcmp(exec_path, "cd"))
		bi_cd(av, shell);
	else if (!ft_strcmp(exec_path, "pwd"))
		bi_pwd();
	else if (!ft_strcmp(exec_path, "export"))
		bi_export(shell, av);
	else if (!ft_strcmp(exec_path, "unset"))
		bi_unset(shell);
	else if (!ft_strcmp(exec_path, "env"))
		bi_env(shell);
	else if (!ft_strcmp(exec_path, "exit"))
		bi_exit(shell, av);
	exit(g_wstatus);
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
		ft_dup_close((int [2]){shell->pipe_fd[1], STDOUT_FILENO},
			job->av, exec_path, shell->locations);
	if (is_builtin(exec_path, shell, job->av[1]))
		exec_builtins(exec_path, job->av, shell);
	execve(exec_path, job->av, list_to_char(shell->env));
	print_error("execve: ", NULL, NULL, errno);
	ft_free_job_exit(job->av, exec_path, shell->locations, 1);
}

int	ft_parent(t_cmd *const job, pid_t pid, const int prev_in, t_shell *shell)
{
	int	err;

	err = 0;
	if (job->flags & E_PIPEOUT)
		err += ft_safe_close(shell->pipe_fd[1]);
	else if (job->flags & E_FILEOUT && job->fd_out > 0)
		err += ft_safe_close(job->fd_out);
	if (job->flags & E_PIPEIN)
		err += ft_safe_close(prev_in);
	else if (job->flags & E_FILEIN && job->fd_in > 0)
		err += ft_safe_close(job->fd_in);
	shell->pid_ar[shell->i++] = pid;
	return (err);
}

void	ft_pipex(t_cmd *cmd, t_shell *shell)
{
	const int	prev_in = shell->pipe_fd[0];
	pid_t		pid;
	char		*exec_path;
	int			err;

	err = 0;
	exec_path = NULL;
	shell->locations = get_locations(list_to_char(shell->env));
	if (cmd->flags & E_PIPEOUT)
		pipe(shell->pipe_fd);
	if (*cmd->av)
		exec_path = get_exec_path(*cmd->av, shell->locations, shell, cmd->av[1]);
	pid = fork();
	if (pid < 0)
	{
		print_error("fork: ", NULL, NULL, errno);
		ft_free_job_exit(cmd->av, exec_path, shell->locations, 1);
	}
	if (pid == 0)
		ft_child(cmd, prev_in, exec_path, shell);
	else
		err = ft_parent(cmd, pid, prev_in, shell);
	ft_free_job_exit(cmd->av, exec_path, shell->locations, err);
}
