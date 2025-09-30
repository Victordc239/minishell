/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   20.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sofernan <sofernan@student.42madrid.es>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/19 14:24:25 by sofernan          #+#    #+#             */
/*   Updated: 2025/09/30 19:18:26 by sofernan         ###   ########.fr       */
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

char	*extract_word(t_minishell *shell)
{
	int		start;
	char	*word;

	start = shell->tokenizer->pos;
	while (is_word_char(shell->tokenizer->input[shell->tokenizer->pos]))
		shell->tokenizer->pos++;
	if (shell->tokenizer->pos == start)
		return (NULL);
	word = ft_substr(shell->tokenizer->input, start,
			shell->tokenizer->pos - start);
	shell->tokenizer->last_adjacent = 0;
	if (shell->tokenizer->pos < (int)ft_strlen(shell->tokenizer->input)
		&& ft_strchr("<>|", shell->tokenizer->input[shell->tokenizer->pos]))
		shell->tokenizer->last_adjacent = 1;
	else
		shell->tokenizer->last_adjacent = 0;
	return (word);
}

char	*get_next_token_part(t_minishell *shell)
{
	char	*temp;

	if (shell->tokenizer->input[shell->tokenizer->pos] == '$'
		&& shell->tokenizer->input[shell->tokenizer->pos + 1] == '"')
	{
		shell->tokenizer->pos++;
		shell->tokenizer->quote = Q_DOUBLE;
		return (extract_quoted_token(shell));
	}
	if (shell->tokenizer->input[shell->tokenizer->pos] == '\'')
	{
		shell->tokenizer->quote = Q_SINGLE;
		temp = extract_quoted_token(shell);
	}
	else if (shell->tokenizer->input[shell->tokenizer->pos] == '"')
	{
		shell->tokenizer->quote = Q_DOUBLE;
		temp = extract_quoted_token(shell);
	}
	else
	{
		temp = extract_word(shell);
		shell->tokenizer->quote = Q_NONE;
	}
	return (temp);
}
