/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_execute.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sofernan <sofernan@student.42madrid.es>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/19 13:58:50 by sofernan          #+#    #+#             */
/*   Updated: 2025/10/03 15:29:37 by sofernan         ###   ########.fr       */
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
		return (NO_BUITIN);
	if (!ft_strcmp(token->value, "echo") || !ft_strcmp(token->value, "pwd")
		|| !ft_strcmp(token->value, "env"))
		return (BUILTIN_CHILD);
	else if (!ft_strcmp(token->value, "cd") || !ft_strcmp(token->value, "exit")
		|| !ft_strcmp(token->value, "export") || !ft_strcmp(token->value,
			"unset"))
		return (BUILTIN_PARENT);
	return (NO_BUITIN);
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
		filename = get_filename(i);
		if (filename)
		{
			unlink(filename);
			free(filename);
		}
		i++;
	}
}

void	ft_execute_helper(t_minishell *mini)
{
	int	i;

	i = 0;
	if (mini->pipex_data->n_cmds > 1
		&& mini->pipex_data->builtins == BUILTIN_PARENT)
	{
		mini->pipex_data->builtins = NO_BUITIN;
	}
	mini->pipex_data->pid = malloc(sizeof(pid_t) * mini->pipex_data->n_cmds);
	if (!mini->pipex_data->pid)
		exit_with_error("Error malloc pid failed\n", 1, 2);
	while (i < mini->pipex_data->n_cmds)
		mini->pipex_data->pid[i++] = -1;
	execute_pipeline(mini);
	delete_heredoc_files(mini->pipex_data->count_heredoc);
	if (mini->pipex_data)
		free_pipex_data(mini->pipex_data);
}

void	ft_execute(t_minishell *mini)
{
	t_token	*tokken;

	tokken = mini->t_list;
	mini->pipex_data = init_pipex();
	if (!mini->pipex_data)
		exit_with_error("Error init_pipex\n", 1, 2);
	mini->pipex_data->builtins = is_builtin(mini);
	mini->cmd_list = parse_commands(mini);
	if (g_status == 2 || g_status == 130)
	{
		if (g_status == 130)
			g_status = -g_status;
		delete_heredoc_files(mini->pipex_data->count_heredoc);
		if (mini->pipex_data)
			free_pipex_data(mini->pipex_data);
		mini->pipex_data = NULL;
		mini->t_list = tokken;
		return ;
	}
	mini->t_list = tokken;
	mini->pipex_data->commands = mini->cmd_list;
	mini->pipex_data->n_cmds = count_commands_list(mini);
	ft_execute_helper(mini);
}

// count_commands_list + ft_execute
/*void	ft_execute(t_minishell *mini)
{
	t_token		*tokken;
	t_command	*cmd;

	tokken = mini->t_list;
	mini->pipex_data = init_pipex();
	if (!mini->pipex_data)
		exit_with_error("Error init_pipex\n", 1, 2);
	mini->pipex_data->builtins = is_builtin(mini);
	mini->cmd_list = parse_commands(mini);
	if (g_status == 2 || g_status == 130)
	{
		if (g_status == 130)
			g_status = -g_status;
		delete_heredoc_files(mini->pipex_data->count_heredoc);
		if (mini->pipex_data)
			free_pipex_data(mini->pipex_data);
		mini->pipex_data = NULL;
		mini->t_list = tokken;
		return ;
	}
	mini->t_list = tokken;
	mini->pipex_data->commands = mini->cmd_list;
	cmd = mini->cmd_list;
	while (cmd)
	{
		mini->pipex_data->n_cmds++;
		cmd = cmd->next;
	}
	ft_execute_helper(mini);
}*/

//victor
/*t_pipex	*init_pipex(t_minishell *mini)
{
	t_pipex	*px;

	px = malloc(sizeof(t_pipex));
	if (!px)
		return (NULL);
	px->prev_fd = -1;
	px->count_heredoc = 0;
	px->n_cmds = 0;
	px->builtins = is_builtin(mini);
	px->pid = NULL;
	px->commands = NULL;
	return (px);
}

void	ft_execute(t_minishell *mini)
{
	t_token		*tokken;
	t_command	*cmd;

	tokken = mini->t_list;
	mini->pipex_data = init_pipex(mini);
	if (!mini->pipex_data)
		exit_with_error("Error init_pipex\n", 1, 2);
	mini->cmd_list = parse_commands(mini);
	if (g_status == 2 || g_status == 130)
	{
		if (g_status == 130)
			g_status = -g_status;
		delete_heredoc_files(mini->pipex_data->count_heredoc);
		if (mini->pipex_data)
			free_pipex_data(mini->pipex_data);
		mini->pipex_data = NULL;
		mini->t_list = tokken;
		return ;
	}
	mini->t_list = tokken;
	mini->pipex_data->commands = mini->cmd_list;
	cmd = mini->cmd_list;
	while (cmd)
	{
		mini->pipex_data->n_cmds++;
		cmd = cmd->next;
	}
	ft_execute_helper(mini);
}*/

//is_builtin + count_commands_list
/*void	analyze_commands(t_minishell *mini)
{
	t_token		*token;
	t_command	*cmd;
	int			count;

	count = 0;
	cmd = mini->cmd_list;
	while (cmd)
	{
		count++;
		cmd = cmd->next;
	}
	mini->pipex_data->n_cmds = count;
	token = mini->t_list;
	if (!token || !token->value)
		mini->pipex_data->builtins = NO_BUITIN;
	else if (!ft_strcmp(token->value, "echo") || !ft_strcmp(token->value, "pwd")
		|| !ft_strcmp(token->value, "env"))
		mini->pipex_data->builtins = BUILTIN_CHILD;
	else if (!ft_strcmp(token->value, "cd") || !ft_strcmp(token->value, "exit")
		|| !ft_strcmp(token->value, "export") || !ft_strcmp(token->value, "unset"))
		mini->pipex_data->builtins = BUILTIN_PARENT;
	else
		mini->pipex_data->builtins = NO_BUITIN;
}

void	ft_execute(t_minishell *mini)
{
	t_token	*tokken;

	tokken = mini->t_list;
	mini->pipex_data = init_pipex();
	if (!mini->pipex_data)
		exit_with_error("Error init_pipex\n", 1, 2);
	mini->cmd_list = parse_commands(mini);
	if (g_status == 2 || g_status == 130)
	{
		if (g_status == 130)
			g_status = -g_status;
		delete_heredoc_files(mini->pipex_data->count_heredoc);
		if (mini->pipex_data)
			free_pipex_data(mini->pipex_data);
		mini->pipex_data = NULL;
		mini->t_list = tokken;
		return ;
	}
	mini->t_list = tokken;
	mini->pipex_data->commands = mini->cmd_list;
	analyze_commands(mini);
	ft_execute_helper(mini);
}*/
