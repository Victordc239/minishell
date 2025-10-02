/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   21.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sofernan <sofernan@student.42madrid.es>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/19 14:27:30 by sofernan          #+#    #+#             */
/*   Updated: 2025/10/02 14:15:25 by sofernan         ###   ########.fr       */
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

int	process_token_part(t_minishell *shell, char **token,
	t_token_quote *first_quote, int *mixed)
{
	char	*part;

	while (shell->tokenizer->input[shell->tokenizer->pos]
		&& shell->tokenizer->input[shell->tokenizer->pos] != ' '
		&& shell->tokenizer->input[shell->tokenizer->pos] != '\t'
		&& !ft_strchr("|<>", shell->tokenizer->input[shell->tokenizer->pos]))
	{
		part = get_next_token_part(shell);
		if (!part)
			return (0);
		if (*first_quote == (t_token_quote) - 1)
			*first_quote = shell->tokenizer->quote;
		else if (shell->tokenizer->quote != *first_quote)
			*mixed = 1;
		if (shell->tokenizer->quote == Q_SINGLE)
			replace_char_inplace(part, '$', '\x07');
		*token = join_with_marker(*token, part);
	}
	return (1);
}

char	*extract_complex_token(t_minishell *shell)
{
	char			*token;
	t_token_quote	first_quote;
	int				mixed;

	mixed = 0;
	first_quote = (t_token_quote)-1;
	token = ft_strdup("");
	if (!token)
	{
		shell->tokenizer->err = 1;
		return (NULL);
	}
	if (!process_token_part(shell, &token, &first_quote, &mixed))
		return (free(token), NULL);
	shell->tokenizer->prev_type = T_WORD;
	if (!mixed && first_quote != (t_token_quote)-1)
		shell->tokenizer->quote = first_quote;
	else
		shell->tokenizer->quote = Q_NONE;
	if (shell->tokenizer->pos < (int)ft_strlen(shell->tokenizer->input)
		&& ft_strchr("<>|", shell->tokenizer->input[shell->tokenizer->pos]))
		shell->tokenizer->last_adjacent = 1;
	else
		shell->tokenizer->last_adjacent = 0;
	return (token);
}

char	*extract_token(t_minishell *shell)
{
	char	*val;

	while (shell->tokenizer->input[shell->tokenizer->pos] == ' '
		|| shell->tokenizer->input[shell->tokenizer->pos] == '\t'
		|| shell->tokenizer->input[shell->tokenizer->pos] == '\n')
		shell->tokenizer->pos++;
	if (shell->tokenizer->input[shell->tokenizer->pos] == '\0')
		return (NULL);
	val = extract_metachar(shell);
	if (val)
		return (val);
	return (extract_complex_token(shell));
}
