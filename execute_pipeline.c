/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_pipeline.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sofernan <sofernan@student.42madrid.es>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/19 14:07:18 by sofernan          #+#    #+#             */
/*   Updated: 2025/10/13 13:58:44 by sofernan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "mini.h"

void	apply_one_redirection(t_minishell *mini, t_redir *redir)
{
	int	fd;

	if (redir->type == T_RED_IN || redir->type == T_HEREDOC)
		fd = open(redir->filename, O_RDONLY);
	else if (redir->type == T_RED_INOUT)
		fd = open(redir->filename, O_RDWR | O_CREAT, 0644);
	else if (redir->type == T_RED_OUT)
		fd = open(redir->filename, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	else
		fd = open(redir->filename, O_WRONLY | O_CREAT | O_APPEND, 0644);
	if (fd == -1)
		(perror(redir->filename), free_minishell(mini), exit(1));
	if (redir->type == T_RED_IN || redir->type == T_HEREDOC
		|| redir->type == T_RED_INOUT)
	{
		if (dup2(fd, STDIN_FILENO) == -1)
			(perror("dup2"), close(fd), free_minishell(mini), exit(1));
	}
	else
	{
		if (dup2(fd, STDOUT_FILENO) == -1)
			(perror("dup2"), close(fd), free_minishell(mini), exit(1));
	}
	close(fd);
}

void	apply_redirections(t_minishell *mini)
{
	t_redir	*redir;

	redir = mini->cmd_list->redirs;
	while (redir)
	{
		if ((redir->type == T_RED_IN || redir->type == T_RED_OUT
				|| redir->type == T_HEREDOC || redir->type == T_RED_APPEND
				|| redir->type == T_RED_INOUT)
			&& (!redir->filename || ft_strchr("|'\"<>", redir->filename[0])))
		{
			free_minishell(mini);
			exit_with_error(SYNTAX_ERROR, 2, 2);
		}
		apply_one_redirection(mini, redir);
		redir = redir->next;
	}
}

void	execute_last_command(t_minishell *mini, int i)
{
	(signal(SIGINT, SIG_IGN), mini->pipex_data->pid[i] = fork());
	if (mini->pipex_data->pid[i] == -1)
		free_struct(mini->pipex_data, ERR_FORK, 1, 2);
	if (mini->pipex_data->pid[i] == 0)
	{
		(signal(SIGINT, SIG_DFL), signal(SIGQUIT, SIG_DFL));
		if (mini->saved_stdin != -1)
			(close(mini->saved_stdin), mini->saved_stdin = -1);
		if (mini->pipex_data->prev_fd != -1)
		{
			if (dup2(mini->pipex_data->prev_fd, STDIN_FILENO) == -1)
				exit_with_error("dup2 final prev_fd failed\n", 1, 2);
			close(mini->pipex_data->prev_fd);
			mini->pipex_data->prev_fd = -1;
		}
		apply_redirections(mini);
		if (!mini->cmd_list->argv || !mini->cmd_list->argv[0])
			exit(0);
		ft_cmd(mini);
	}
	else
		if (mini->pipex_data->prev_fd != -1)
			(close(mini->pipex_data->prev_fd), mini->pipex_data->prev_fd = -1);
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
	while (mini->cmd_list && i < mini->pipex_data->n_cmds - 1)
	{
		process_and_exec(mini, i);
		mini->cmd_list = mini->cmd_list->next;
		i++;
	}
	if (mini->pipex_data->builtins == 1)
	{
		apply_redirections(mini);
		exec_builtin_parent(mini);
	}
	else
		execute_last_command(mini, i);
	wait_status(mini->pipex_data);
	if (mini->pipex_data->prev_fd != -1)
	{
		close(mini->pipex_data->prev_fd);
		mini->pipex_data->prev_fd = -1;
	}
	signal(SIGINT, sighandler);
}
