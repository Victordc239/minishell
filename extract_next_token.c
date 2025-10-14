/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   extract_metachar.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sofernan <sofernan@student.42madrid.es>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/19 14:27:30 by sofernan          #+#    #+#             */
/*   Updated: 2025/10/13 18:44:00 by sofernan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "mini.h"

char	*join_free(char *s1, char *s2)
{
	char	*res;

	res = ft_strjoin(s1, s2);
	free(s1);
	free(s2);
	return (res);
}

char	*join_with_marker(char *s1, char *s2)
{
	size_t	l1;
	size_t	l2;
	char	*res;

	if (!s1)
		s1 = ft_strdup("");
	if (!s1)
	{
		free(s2);
		return (NULL);
	}
	if (s1[0] == '\0')
		return (join_free(s1, s2));
	l1 = ft_strlen(s1);
	l2 = ft_strlen(s2);
	res = malloc(l1 + 1 + l2 + 1);
	if (!res)
		return (free(s1), free(s2), NULL);
	ft_memcpy(res, s1, l1);
	res[l1] = '\x01';
	ft_memcpy(res + l1 + 1, s2, l2 + 1);
	free(s1);
	free(s2);
	return (res);
}

int	process_token_part(t_minishell *mini, char **token,
	t_token_quote *first_quote, int *mixed)
{
	char	*part;

	while (mini->tokenizer->input[mini->tokenizer->pos]
		&& mini->tokenizer->input[mini->tokenizer->pos] != ' '
		&& mini->tokenizer->input[mini->tokenizer->pos] != '\t'
		&& !ft_strchr("|<>", mini->tokenizer->input[mini->tokenizer->pos]))
	{
		part = get_next_token_part(mini);
		if (!part)
			return (0);
		if (*first_quote == (t_token_quote) - 1)
			*first_quote = mini->tokenizer->quote;
		else if (mini->tokenizer->quote != *first_quote)
			*mixed = 1;
		if (mini->tokenizer->quote == Q_SINGLE)
			replace_char_inplace(part, '$', '\x07');
		*token = join_with_marker(*token, part);
	}
	return (1);
}

char	*extract_complex_token(t_minishell *mini)
{
	char			*token;
	t_token_quote	first_quote;
	int				mixed;

	mixed = 0;
	first_quote = (t_token_quote)-1;
	token = ft_strdup("");
	if (!token)
	{
		mini->tokenizer->err = 1;
		return (NULL);
	}
	if (!process_token_part(mini, &token, &first_quote, &mixed))
		return (free(token), NULL);
	mini->tokenizer->prev_type = T_WORD;
	if (!mixed && first_quote != (t_token_quote)-1)
		mini->tokenizer->quote = first_quote;
	else
		mini->tokenizer->quote = Q_NONE;
	if (mini->tokenizer->pos < (int)ft_strlen(mini->tokenizer->input)
		&& ft_strchr("<>|", mini->tokenizer->input[mini->tokenizer->pos]))
		mini->tokenizer->last_adjacent = 1;
	else
		mini->tokenizer->last_adjacent = 0;
	return (token);
}

char	*extract_next_token(t_minishell *mini)
{
	char	*val;

	while (mini->tokenizer->input[mini->tokenizer->pos] == ' '
		|| mini->tokenizer->input[mini->tokenizer->pos] == '\t'
		|| mini->tokenizer->input[mini->tokenizer->pos] == '\n')
		mini->tokenizer->pos++;
	if (mini->tokenizer->input[mini->tokenizer->pos] == '\0')
		return (NULL);
	val = extract_metachar (mini);
	if (val)
		return (val);
	return (extract_complex_token(mini));
}
