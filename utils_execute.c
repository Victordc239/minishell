/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_execute.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sofernan <sofernan@student.42madrid.es>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/19 13:58:50 by sofernan          #+#    #+#             */
/*   Updated: 2025/10/14 20:52:38 by sofernan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "mini.h"

t_pipex	*init_pipex(void)
{
	t_pipex	*px;

	px = malloc(sizeof(t_pipex));
	if (!px)
		return (NULL);
	px->prev_fd = -1;
	px->count_heredoc = 0;
	px->n_cmds = 0;
	px->builtins = 0;
	px->pid = NULL;
	px->commands = NULL;
	return (px);
}

int	is_builtin(t_minishell *mini)
{
	t_token	*token;

	token = mini->t_list;
	if (!token || !token->value)
		return (-1);
	if (!ft_strcmp(token->value, "echo") || !ft_strcmp(token->value, "pwd")
		|| !ft_strcmp(token->value, "env"))
		return (0);
	else if (!ft_strcmp(token->value, "cd") || !ft_strcmp(token->value, "exit")
		|| !ft_strcmp(token->value, "export") || !ft_strcmp(token->value,
			"unset"))
		return (1);
	return (-1);
}

int	count_commands_list(t_minishell *mini)
{
	t_command	*cmd;
	int			count;

	count = 0;
	cmd = mini->cmd_list;
	while (cmd)
	{
		count++;
		cmd = cmd->next;
	}
	return (count);
}

void	delete_heredoc_files(int n)
{
	int		i;
	char	*filename;

	i = 0;
	while (i < n)
	{
		filename = create_heredoc_filename(i);
		if (filename)
		{
			unlink(filename);
			free(filename);
		}
		i++;
	}
}

void	run_pipeline(t_minishell *mini)
{
	int	i;

	i = 0;
	if (mini->pipex_data->n_cmds > 1
		&& mini->pipex_data->builtins == 1)
	{
		mini->pipex_data->builtins = -1;
	}
	mini->pipex_data->pid = malloc(sizeof(pid_t)
			* mini->pipex_data->n_cmds);
	if (!mini->pipex_data->pid)
		exit_with_error("Error malloc pid failed\n", 1, 2);
	while (i < mini->pipex_data->n_cmds)
		mini->pipex_data->pid[i++] = -1;
	execute_pipeline(mini);
	if (mini->pipex_data->prev_fd != -1)
	{
		close(mini->pipex_data->prev_fd);
		mini->pipex_data->prev_fd = -1;
	}
	delete_heredoc_files(mini->pipex_data->count_heredoc);
	if (mini->pipex_data)
		free_pipex_data(mini->pipex_data);
}
