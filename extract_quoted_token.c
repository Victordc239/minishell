/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   extract_quoted_token.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sofernan <sofernan@student.42madrid.es>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/30 19:17:34 by sofernan          #+#    #+#             */
/*   Updated: 2025/09/30 19:18:15 by sofernan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "mini.h"

/*char	*extract_quoted_token(t_minishell *shell)
{
	size_t j;
	char quote_char;
	char *buf;
	size_t len;
	size_t pos;
	size_t	bi;
	size_t	bufcap;
	char	*cdup;
	char	c;
	char next;

	quote_char = shell->tokenizer->input[shell->tokenizer->pos];
	j = shell->tokenizer->pos + 1;
	len = ft_strlen(shell->tokenizer->input);
	pos = j;
	bi = 0;
	if (len > 16)
		bufcap = len + 1;
	else
		bufcap = 64;
	buf = malloc(bufcap);
	if (!buf)
	{
		shell->tokenizer->err = 1;
		return (NULL);
	}
	while (1)
	{
		if (pos >= ft_strlen(shell->tokenizer->input))
		{
			free(buf);
			ft_putstr("Quote expansion error\n", 2);
			g_status = 2;
			shell->tokenizer->err = 1;
			return (NULL);
		}
		c = shell->tokenizer->input[pos];
		if (c == quote_char)
		{
			buf[bi] = '\0';
			shell->tokenizer->prev_type = T_WORD;
			shell->tokenizer->pos = pos + 1;
			if (shell->tokenizer->pos < (int)ft_strlen(shell->tokenizer->input)
				&& ft_strchr("<>|", shell->tokenizer->input[shell->tokenizer->pos]))
				shell->tokenizer->last_adjacent = 1;
			else
				shell->tokenizer->last_adjacent = 0;
			cdup = ft_strdup(buf);
			free(buf);
			return (cdup);
		}
		if (quote_char == '"' && c == '\\'
			&& pos + 1 < ft_strlen(shell->tokenizer->input))
		{
			next = shell->tokenizer->input[pos + 1];
			if (next == '"' || next == '\\' || next == '$' || next == '`')
			{
				if (bi + 1 >= bufcap)
				{
					bufcap *= 2;
					buf = realloc(buf, bufcap);
					if (!buf)
					{
						shell->tokenizer->err = 1;
						return (NULL);
					}
				}
				buf[bi++] = next;
				pos += 2;
				continue ;
			}
			if (bi + 1 >= bufcap)
			{
				bufcap *= 2;
				buf = realloc(buf, bufcap);
				if (!buf)
				{
					shell->tokenizer->err = 1;
					return (NULL);
				}
			}
			buf[bi++] = c;
			pos++;
			continue ;
		}
		if (bi + 1 >= bufcap)
		{
			bufcap *= 2;
			buf = realloc(buf, bufcap);
			if (!buf)
			{
				shell->tokenizer->err = 1;
				return (NULL);
			}
		}
		buf[bi++] = c;
		pos++;
	}
	return (NULL);
}*/

int	init_extract(t_minishell *shell, t_extract *e)
{
	e->quote_char = shell->tokenizer->input[shell->tokenizer->pos];
	e->j = shell->tokenizer->pos + 1;
	e->len = ft_strlen(shell->tokenizer->input);
	e->pos = e->j;
	e->bi = 0;
	if (e->len > 16)
		e->bufcap = e->len + 1;
	else
		e->bufcap = 64;
	e->buf = malloc(e->bufcap);
	if (!e->buf)
	{
		shell->tokenizer->err = 1;
		return (0);
	}
	return (1);
}

int	ensure_capacity_2(t_extract *e, t_minishell *shell)
{
	size_t		newcap;
	char		*newbuf;

	if (e->bi + 1 < e->bufcap)
		return (1);
	newcap = e->bufcap * 2;
	newbuf = malloc(newcap);
	if (!newbuf)
	{
		shell->tokenizer->err = 1;
		return (0);
	}
	ft_memcpy(newbuf, e->buf, e->bi);
	free(e->buf);
	e->buf = newbuf;
	e->bufcap = newcap;
	return (1);
}

int	handle_escape_double(t_extract *e, t_minishell *shell)
{
	e->next = shell->tokenizer->input[e->pos + 1];
	if (e->next == '"' || e->next == '\\' || e->next == '$' || e->next == '`')
	{
		if (!ensure_capacity_2(e, shell))
			return (0);
		e->buf[e->bi++] = e->next;
		e->pos += 2;
		return (1);
	}
	if (!ensure_capacity_2(e, shell))
		return (0);
	e->buf[e->bi++] = '\\';
	e->pos++;
	return (1);
}

char	*finish_and_return(t_extract *e, t_minishell *shell)
{
	e->buf[e->bi] = '\0';
	shell->tokenizer->prev_type = T_WORD;
	shell->tokenizer->pos = (int)(e->pos + 1);
	if (shell->tokenizer->pos < (int)ft_strlen(shell->tokenizer->input)
		&& ft_strchr("<>", shell->tokenizer->input[shell->tokenizer->pos]))
		shell->tokenizer->last_adjacent = 1;
	else
		shell->tokenizer->last_adjacent = 0;
	e->cdup = ft_strdup(e->buf);
	free(e->buf);
	return (e->cdup);
}

char	*extract_quoted_token(t_minishell *shell)
{
	t_extract	e;

	if (!init_extract(shell, &e))
		return (NULL);
	while (1)
	{
		if (e.pos >= e.len)
			return (free(e.buf), ft_putstr("Quote expansion error\n", 2),
				g_status = 2, shell->tokenizer->err = 1, NULL);
		e.c = shell->tokenizer->input[e.pos];
		if (e.c == e.quote_char)
			return (finish_and_return(&e, shell));
		if (e.quote_char == '"' && e.c == '\\' && e.pos + 1 < e.len)
		{
			if (!handle_escape_double(&e, shell))
				return (NULL);
			continue ;
		}
		if (!ensure_capacity_2(&e, shell))
			return (NULL);
		e.buf[e.bi++] = e.c;
		e.pos++;
	}
	return (NULL);
}
