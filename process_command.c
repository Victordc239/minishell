/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   process_command.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sofernan <sofernan@student.42madrid.es>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/19 14:35:56 by sofernan          #+#    #+#             */
/*   Updated: 2025/10/06 14:00:44 by sofernan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "mini.h"

int	init_tokenizer(t_minishell *minishell, char *input)
{
	if (minishell->t_list)
	{
		free_t_list(minishell->t_list);
		minishell->t_list = NULL;
	}
	if (minishell->tokenizer)
		(free_tokenizer(minishell->tokenizer), minishell->tokenizer = NULL);
	minishell->tokenizer = malloc(sizeof(t_tokenizer));
	if (!minishell->tokenizer)
		return (0);
	minishell->tokenizer->input = ft_strdup(input);
	if (!minishell->tokenizer->input)
	{
		free(minishell->tokenizer);
		minishell->tokenizer = NULL;
		return (0);
	}
	minishell->tokenizer->pos = 0;
	minishell->tokenizer->prev_type = T_WORD;
	minishell->tokenizer->quote = Q_NONE;
	minishell->tokenizer->err = 0;
	minishell->tokenizer->last_adjacent = 0;
	return (1);
}

int	fill_tokens(t_minishell *minishell, char *input)
{
	int	success;

	success = 1;
	if (!init_tokenizer(minishell, input))
		return (0);
	success = tokenize_input(minishell);
	if (minishell->tokenizer)
	{
		if (minishell->tokenizer->err)
			success = 0;
		if (minishell->tokenizer->input)
			free(minishell->tokenizer->input);
		free(minishell->tokenizer);
		minishell->tokenizer = NULL;
	}
	if (!success)
	{
		free_t_list(minishell->t_list);
		minishell->t_list = NULL;
	}
	return (success);
}

int	check_syntax_pipes(t_token *tokenizer)
{
	if (!tokenizer)
		return (1);
	if (tokenizer->type == T_PIPE)
	{
		ft_putstr("minishell: syntax error near unexpected token `|'\n", 2);
		g_status = 2;
		return (0);
	}
	while (tokenizer->next)
	{
		if (tokenizer->type == T_PIPE && tokenizer->next->type == T_PIPE)
		{
			ft_putstr("minishell: syntax error near unexpected token `|'\n", 2);
			g_status = 2;
			return (0);
		}
		tokenizer = tokenizer->next;
	}
	if (tokenizer->type == T_PIPE)
	{
		ft_putstr("minishell: syntax error near unexpected token `|'\n", 2);
		g_status = 2;
		return (0);
	}
	return (1);
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

void	process_command(t_minishell *minishell, char *seg, char *inner)
{
	if (!fill_tokens(minishell, seg))
	{
		update_env_status(minishell);
		free_t_list(minishell->t_list);
		minishell->t_list = NULL;
		if (inner)
			free(inner);
		return ;
	}
	if (!check_syntax_pipes(minishell->t_list))
	{
		update_env_status(minishell);
		free_t_list(minishell->t_list);
		minishell->t_list = NULL;
		if (inner)
			free(inner);
		return ;
	}
	ft_execute(minishell);
	free_t_list(minishell->t_list);
	minishell->t_list = NULL;
	if (inner)
		free(inner);
}
