/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   32.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sofernan <sofernan@student.42madrid.es>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/19 14:35:56 by sofernan          #+#    #+#             */
/*   Updated: 2025/09/26 20:24:02 by sofernan         ###   ########.fr       */
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

void	cleanup_tokenizer(t_minishell *minishell, int success)
{
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
}

int	fill_tokens(t_minishell *minishell, char *input)
{
	int	success;

	success = 1;
	if (!init_tokenizer(minishell, input))
		return (0);
	success = tokenize_input(minishell);
	cleanup_tokenizer(minishell, success);
	return (success);
}
