/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   process_and_exec.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sofernan <sofernan@student.42madrid.es>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/19 14:16:00 by sofernan          #+#    #+#             */
/*   Updated: 2025/10/13 13:31:06 by sofernan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "mini.h"

void	free_struct(t_pipex *data, char *message, int exit_code, int std)
{
	if (data)
	{
		if (data->pid)
		{
			free(data->pid);
			data->pid = NULL;
		}
		free(data);
	}
	ft_putstr(message, std);
	exit(exit_code);
}

int	has_redir_type(t_command *cmd, int type)
{
	t_redir	*r;

	r = cmd->redirs;
	while (r)
	{
		if (r->type == type)
			return (1);
		r = r->next;
	}
	return (0);
}

void	child_process(t_minishell *mini, int fd[2])
{
	signal(SIGINT, SIG_DFL);
	signal(SIGQUIT, SIG_DFL);
	if (mini->saved_stdin != -1)
		(close(mini->saved_stdin), mini->saved_stdin = -1);
	apply_redirections(mini);
	if (!has_redir_type(mini->cmd_list, T_RED_IN)
		&& !has_redir_type(mini->cmd_list, T_HEREDOC)
		&& mini->pipex_data->prev_fd != -1)
	{
		if (dup2(mini->pipex_data->prev_fd, STDIN_FILENO) == -1)
			exit_with_error("dup2 prev_fd failed\n", 1, 2);
		close(mini->pipex_data->prev_fd);
		mini->pipex_data->prev_fd = -1;
	}
	if (!has_redir_type(mini->cmd_list, T_RED_OUT)
		&& !has_redir_type(mini->cmd_list, T_RED_APPEND))
	{
		if (dup2(fd[1], STDOUT_FILENO) == -1)
			exit_with_error("dup2 pipe write failed\n", 1, 2);
	}
	(close(fd[0]), close(fd[1]));
	if (!mini->cmd_list->argv || !mini->cmd_list->argv[0])
		exit(0);
	ft_cmd(mini);
}

void	process_and_exec(t_minishell *mini, int i)
{
	int	fd[2];

	if (pipe(fd) == -1)
		free_struct(mini->pipex_data, ERR_PIPE, 1, 2);
	signal(SIGINT, SIG_IGN);
	mini->pipex_data->pid[i] = fork();
	if (mini->pipex_data->pid[i] == -1)
		free_struct(mini->pipex_data, ERR_FORK, 1, 2);
	if (mini->pipex_data->pid[i] == 0)
	{
		signal(SIGINT, SIG_DFL);
		signal(SIGQUIT, SIG_DFL);
		child_process(mini, fd);
	}
	else
	{
		if (mini->pipex_data->prev_fd != -1)
			close(mini->pipex_data->prev_fd);
		mini->pipex_data->prev_fd = fd[0];
		close(fd[1]);
	}
}
