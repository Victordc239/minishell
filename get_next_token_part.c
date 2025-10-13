/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_token_part.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sofernan <sofernan@student.42madrid.es>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/19 14:24:25 by sofernan          #+#    #+#             */
/*   Updated: 2025/10/13 19:16:08 by sofernan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "mini.h"

int	is_word_char(char c)
{
	if (c == '\0')
		return (0);
	if (c == ' ' || c == '\t')
		return (0);
	if (c == '|' || c == '<' || c == '>')
		return (0);
	if (c == '\'' || c == '"')
		return (0);
	return (1);
}

char	*extract_word(t_minishell *mini)
{
	int		start;
	char	*word;

	start = mini->tokenizer->pos;
	while (is_word_char(mini->tokenizer->input[mini->tokenizer->pos]))
		mini->tokenizer->pos++;
	if (mini->tokenizer->pos == start)
		return (NULL);
	word = ft_substr(mini->tokenizer->input, start,
			mini->tokenizer->pos - start);
	mini->tokenizer->last_adjacent = 0;
	if (mini->tokenizer->pos < (int)ft_strlen(mini->tokenizer->input)
		&& ft_strchr("<>|", mini->tokenizer->input[mini->tokenizer->pos]))
		mini->tokenizer->last_adjacent = 1;
	else
		mini->tokenizer->last_adjacent = 0;
	return (word);
}

char	*get_next_token_part(t_minishell *mini)
{
	char	*tmp;

	if (mini->tokenizer->input[mini->tokenizer->pos] == '$'
		&& mini->tokenizer->input[mini->tokenizer->pos + 1] == '"')
	{
		mini->tokenizer->pos++;
		mini->tokenizer->quote = Q_DOUBLE;
		return (extract_quoted_token(mini));
	}
	if (mini->tokenizer->input[mini->tokenizer->pos] == '\'')
	{
		mini->tokenizer->quote = Q_SINGLE;
		tmp = extract_quoted_token(mini);
	}
	else if (mini->tokenizer->input[mini->tokenizer->pos] == '"')
	{
		mini->tokenizer->quote = Q_DOUBLE;
		tmp = extract_quoted_token(mini);
	}
	else
	{
		tmp = extract_word(mini);
		mini->tokenizer->quote = Q_NONE;
	}
	return (tmp);
}
