/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   3.c                                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sofernan <sofernan@student.42madrid.es>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/02 17:44:09 by sofernan          #+#    #+#             */
/*   Updated: 2025/10/03 15:13:51 by sofernan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "mini.h"

int	ft_isspace(int c)
{
	if (c == ' ' || c == '\t' || c == '\n'
		|| c == '\v' || c == '\f' || c == '\r')
		return (1);
	return (0);
}

int	ends_with_unquoted_continuation_op(const char *s)
{
	int	i;
	int	in_sq;
	int	in_dq;

	in_dq = 0;
	in_sq = 0;
	i = 0;
	if (!s)
		return (0);
	while (s[i])
	{
		if (s[i] == '\'' && !in_dq)
			in_sq = !in_sq;
		else if (s[i] == '"' && !in_sq)
			in_dq = !in_dq;
		i++;
	}
	i--;
	while (i >= 0 && ft_isspace((unsigned char)s[i]))
		i--;
	if (i < 0)
		return (0);
	if ((s[i] == '|') || (s[i] == '&' && i - 1 >= 0 && s[i - 1] == '&'))
		return (1);
	return (0);
}

int	append_more_line(char **cur_input)
{
	char	*more;
	char	*tmp;

	more = readline("> ");
	if (!more)
	{
		ft_putstr("minishell: syntax error: unexpected end of file\n", 2);
		g_status = 2;
		return (0);
	}
	add_history(more);
	tmp = ft_strjoin(*cur_input, "\n");
	free(*cur_input);
	*cur_input = ft_strjoin(tmp, more);
	free(tmp);
	free(more);
	return (1);
}

int	handle_continuation(char **cur_input)
{
	if (!ends_with_unquoted_continuation_op(*cur_input))
		return (1);
	while (ends_with_unquoted_continuation_op(*cur_input))
	{
		if (!append_more_line(cur_input))
			return (0);
	}
	return (1);
}
