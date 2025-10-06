/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   process_input.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sofernan <sofernan@student.42madrid.es>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/19 14:44:02 by sofernan          #+#    #+#             */
/*   Updated: 2025/10/06 19:14:48 by sofernan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "mini.h"

void	update_env_status(t_minishell *mini)
{
	char	*status_str;
	int		status;

	if (g_status < 0)
	{
		status = -g_status;
		g_status = SIGINT;
	}
	else
		status = (int)g_status;
	status_str = ft_itoa(status);
	if (status_str)
	{
		add_env_node(mini, "?", status_str, 0);
		free(status_str);
	}
}

void	exec_segments(t_minishell *mini, char **segments,
						char **ops, int seg_count)
{
	if (seg_count <= 0)
		return ;
	process_segment(mini, *segments);
	update_env_status(mini);
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
	exec_segments(mini, segments + 1, ops + 1, seg_count - 1);
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

void	process_input(char *input, t_minishell *mini)
{
	char	**segments;
	char	**ops;
	int		seg_count;

	segments = NULL;
	ops = NULL;
	seg_count = 0;
	if (!prepare_segments(input, &segments, &ops, &seg_count))
	{
		update_env_status(mini);
		return ;
	}
	exec_segments(mini, segments, ops, seg_count);
	free_split_result(segments, ops, seg_count);
}
