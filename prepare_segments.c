/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   prepare_segments.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sofernan <sofernan@student.42madrid.es>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/02 17:42:00 by sofernan          #+#    #+#             */
/*   Updated: 2025/10/06 19:12:20 by sofernan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "mini.h"

int	ends_with_unquoted_redir(const char *s, int in_sq, int in_dq)
{
	int	i;

	in_sq = 0;
	in_dq = 0;
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
	while (i >= 0 && (s[i] == '\0' || ft_isspace((unsigned char)s[i])))
		i--;
	if (i < 0)
		return (0);
	if ((i - 1 >= 0 && s[i] == '>' && s[i - 1] == '>')
		|| (i - 1 >= 0 && s[i] == '<' && s[i - 1] == '<')
		|| (s[i] == '<') || (s[i] == '>'))
		return (1);
	return (0);
}

int	split_ops(char *input, char ***segments_out,
				char ***ops_out, int *count_out)
{
	t_split_state	st;

	if (!input)
		return (0);
	st.pos = 0;
	st.len = ft_strlen(input);
	st.start = 0;
	st.quote = 0;
	st.paren_depth = 0;
	st.seg_count = 0;
	st.segments = NULL;
	st.ops = NULL;
	split_loop_and_append(input, &st);
	*segments_out = st.segments;
	*ops_out = st.ops;
	*count_out = st.seg_count;
	return (1);
}

int	complete_last_seg(char **cur_input, char ***segments,
						char ***ops, int *seg_count)
{
	while (*seg_count >= 2
		&& (*segments)[*seg_count - 1]
		&& (*segments)[*seg_count - 1][0] == '\0')
	{
		if (!append_more_line(cur_input))
		{
			free_split_result(*segments, *ops, *seg_count);
			free(*cur_input);
			return (0);
		}
		free_split_result(*segments, *ops, *seg_count);
		*segments = NULL;
		*ops = NULL;
		*seg_count = 0;
		if (!split_ops(*cur_input, segments, ops, seg_count))
		{
			ft_putstr("mini: internal split error\n", 2);
			free(*cur_input);
			return (0);
		}
	}
	return (1);
}

int	prepare_segments(char *input, char ***segments, char ***ops, int *seg_count)
{
	char	*cur_input;

	add_history(input);
	g_status = 0;
	*seg_count = 0;
	*ops = NULL;
	*segments = NULL;
	cur_input = ft_strdup(input);
	if (!cur_input)
		return (0);
	if (ends_with_unquoted_redir(cur_input, 0, 0))
	{
		ft_putstr("mini: ", 2);
		ft_putstr("syntax error near unexpected token `newline'\n", 2);
		g_status = 2;
		return (free(cur_input), 0);
	}
	if (!handle_continuation(&cur_input))
		return (free(cur_input), 0);
	if (!split_ops(cur_input, segments, ops, seg_count))
		return (ft_putstr("mini: internal split error\n", 2),
			g_status = 2, free(cur_input), 0);
	if (!complete_last_seg(&cur_input, segments, ops, seg_count))
		return (0);
	return (free(cur_input), 1);
}
