/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   process_command.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sofernan <sofernan@student.42madrid.es>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/19 14:35:56 by sofernan          #+#    #+#             */
/*   Updated: 2025/10/03 15:26:24 by sofernan         ###   ########.fr       */
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

void	update_env_status(t_minishell *minishell)
{
	char	*status_str;
	int		status;

	if (g_status < 0)
	{
		status = -g_status;
		g_status = SIGINT;
	}
	else
		status = (int)g_status;
	status_str = ft_itoa(status);
	if (status_str)
	{
		add_env_node(minishell, "?", status_str, 0);
		free(status_str);
	}
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
