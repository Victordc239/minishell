/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   35ok.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sofernan <sofernan@student.42madrid.es>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/19 14:44:02 by sofernan          #+#    #+#             */
/*   Updated: 2025/09/29 17:35:53 by sofernan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "mini.h"

int	split_ops(char *input, char ***segments_out, char ***ops_out, int *count_out)
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

int	handle_empty_last_segment(char **cur_input, char ***segments, char ***ops, int *seg_count)
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
			ft_putstr("minishell: internal split error\n", 2);
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
		ft_putstr("minishell: ", 2);
		ft_putstr("syntax error near unexpected token `newline'\n", 2);
		g_status = 2;
		return (free(cur_input), 0);
	}
	if (!handle_continuation(&cur_input))
		return (free(cur_input), 0);
	if (!split_ops(cur_input, segments, ops, seg_count))
		return (ft_putstr("minishell: internal split error\n", 2),
			g_status = 2, free(cur_input), 0);
	if (!handle_empty_last_segment(&cur_input, segments, ops, seg_count))
		return (0);
	return (free(cur_input), 1);
}

void	handle_segments(t_minishell *minishell, char **segments, char **ops, int seg_count)
{
	if (seg_count <= 0)
		return ;
	process_segment(minishell, *segments);
	update_env_status(minishell);
	if (seg_count > 1 && ops && *ops)
	{
		if (!ft_strcmp(*ops, "&&") && g_status != 0)
		{
			segments++;
			ops++;
			seg_count--;
		}
		else if (!ft_strcmp(*ops, "||") && g_status == 0)
		{
			segments++;
			ops++;
			seg_count--;
		}
	}
	handle_segments(minishell, segments + 1, ops + 1, seg_count - 1);
}

void	free_split_result(char **segments, char **ops, int count)
{
	int	i;

	if (segments)
	{
		i = 0;
		while (i < count)
		{
			free(segments[i]);
			i++;
		}
		free(segments);
	}
	if (ops)
	{
		i = 0;
		while (i < count - 1)
		{
			free(ops[i]);
			i++;
		}
		free(ops);
	}
}

void	process_input(char *input, t_minishell *minishell)
{
	char	**segments;
	char	**ops;
	int		seg_count;

	segments = NULL;
	ops = NULL;
	seg_count = 0;
	if (!prepare_segments(input, &segments, &ops, &seg_count))
	{
		update_env_status(minishell);
		return ;
	}
	handle_segments(minishell, segments, ops, seg_count);
	free_split_result(segments, ops, seg_count);
}
