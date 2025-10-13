/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   extract_quoted_token.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sofernan <sofernan@student.42madrid.es>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/30 19:17:34 by sofernan          #+#    #+#             */
/*   Updated: 2025/10/13 19:46:24 by sofernan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "mini.h"

int	init_quoted_token(t_minishell *mini, t_extract *extr)
{
	extr->quote_char = mini->tokenizer->input[mini->tokenizer->pos];
	extr->j = mini->tokenizer->pos + 1;
	extr->len = ft_strlen(mini->tokenizer->input);
	extr->pos = extr->j;
	extr->bi = 0;
	if (extr->len > 16)
		extr->bufcap = extr->len + 1;
	else
		extr->bufcap = 64;
	extr->buf = malloc(extr->bufcap);
	if (!extr->buf)
	{
		mini->tokenizer->err = 1;
		return (0);
	}
	return (1);
}

int	reserve_buffer_capacity(t_extract *extr, t_minishell *mini)
{
	size_t		newcap;
	char		*newbuf;

	if (extr->bi + 1 < extr->bufcap)
		return (1);
	newcap = extr->bufcap * 2;
	newbuf = malloc(newcap);
	if (!newbuf)
	{
		mini->tokenizer->err = 1;
		return (0);
	}
	ft_memcpy(newbuf, extr->buf, extr->bi);
	free(extr->buf);
	extr->buf = newbuf;
	extr->bufcap = newcap;
	return (1);
}

int	handle_double_quote_escape(t_extract *extr, t_minishell *mini)
{
	extr->next = mini->tokenizer->input[extr->pos + 1];
	if (extr->next == '"' || extr->next == '\\'
		|| extr->next == '$' || extr->next == '`')
	{
		if (!reserve_buffer_capacity(extr, mini))
			return (0);
		extr->buf[extr->bi++] = extr->next;
		extr->pos += 2;
		return (1);
	}
	if (!reserve_buffer_capacity(extr, mini))
		return (0);
	extr->buf[extr->bi++] = '\\';
	extr->pos++;
	return (1);
}

char	*finalize_quoted_token(t_extract *extr, t_minishell *mini)
{
	extr->buf[extr->bi] = '\0';
	mini->tokenizer->prev_type = T_WORD;
	mini->tokenizer->pos = (int)(extr->pos + 1);
	if (mini->tokenizer->pos < (int)ft_strlen(mini->tokenizer->input)
		&& ft_strchr("<>", mini->tokenizer->input[mini->tokenizer->pos]))
		mini->tokenizer->last_adjacent = 1;
	else
		mini->tokenizer->last_adjacent = 0;
	extr->cdup = ft_strdup(extr->buf);
	free(extr->buf);
	return (extr->cdup);
}

char	*extract_quoted_token(t_minishell *mini)
{
	t_extract	extr;

	if (!init_quoted_token(mini, &extr))
		return (NULL);
	while (1)
	{
		if (extr.pos >= extr.len)
			return (free(extr.buf), ft_putstr("Quote expansion error\n", 2),
				g_status = 2, mini->tokenizer->err = 1, NULL);
		extr.c = mini->tokenizer->input[extr.pos];
		if (extr.c == extr.quote_char)
			return (finalize_quoted_token(&extr, mini));
		if (extr.quote_char == '"' && extr.c == '\\' && extr.pos + 1 < extr.len)
		{
			if (!handle_double_quote_escape(&extr, mini))
				return (NULL);
			continue ;
		}
		if (!reserve_buffer_capacity(&extr, mini))
			return (NULL);
		extr.buf[extr.bi++] = extr.c;
		extr.pos++;
	}
	return (NULL);
}
