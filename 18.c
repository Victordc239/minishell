/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   18.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sofernan <sofernan@student.42madrid.es>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/19 14:11:08 by sofernan          #+#    #+#             */
/*   Updated: 2025/09/30 19:13:16 by sofernan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "mini.h"

void	execute_last_command(t_minishell *mini, int i)
{
	(signal(SIGINT, SIG_IGN), mini->pipex_data->pid[i] = fork());
	if (mini->pipex_data->pid[i] == -1)
		free_struct(mini->pipex_data, ERR_FORK, 1, 2);
	if (mini->pipex_data->pid[i] == 0)
	{
		(signal(SIGINT, SIG_DFL), signal(SIGQUIT, SIG_DFL));
		if (mini->pipex_data->prev_fd != -1)
		{
			if (dup2(mini->pipex_data->prev_fd, STDIN_FILENO) == -1)
				exit_with_error("dup2 final prev_fd failed\n", 1, 2);
		}
		apply_redirections(mini);
		if (!mini->command_list->argv || !mini->command_list->argv[0])
			exit(0);
		ft_cmd(mini);
	}
}

void	wait_status(t_pipex *data)
{
	int		status;
	pid_t	pid;
	int		count;

	count = 0;
	while (count < data->n_cmds && data->pid[count] != -1)
	{
		pid = waitpid(data->pid[count], &status, 0);
		if (pid == -1)
			perror("waitpid");
		if (pid == -1)
			exit_with_error("Waitpid failed\n", 1, 2);
		if (pid == data->pid[data->n_cmds - 1])
		{
			if (WIFEXITED(status))
				g_status = WEXITSTATUS(status);
			else if (WIFSIGNALED(status))
			{
				g_status = 128 + WTERMSIG(status);
				if (WTERMSIG(status) == SIGINT)
					write(STDOUT_FILENO, "\n", 1);
			}
		}
		count++;
	}
}

void	execute_pipeline(t_minishell *mini)
{
	int	i;

	i = 0;
	while (mini->command_list && i < mini->pipex_data->n_cmds - 1)
	{
		process_and_exec(mini, i);
		mini->command_list = mini->command_list->next;
		i++;
	}
	if (mini->pipex_data->builtins == 1)
		(apply_redirections(mini), execute_buitin(mini));
	else
		execute_last_command(mini, i);
	if (mini->pipex_data->prev_fd != -1)
		close(mini->pipex_data->prev_fd);
	wait_status(mini->pipex_data);
	signal(SIGINT, sighandler);
}
