/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_heredoc.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sofernan <sofernan@student.42madrid.es>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/01 17:14:17 by sofernan          #+#    #+#             */
/*   Updated: 2025/10/06 19:05:04 by sofernan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "mini.h"

char	*get_filename(int index)
{
	char	*number;
	char	*filename;
	char	*temp;

	number = ft_itoa(index);
	if (!number)
		return (NULL);
	temp = ft_strjoin("minishell_", number);
	free(number);
	if (!temp)
		return (NULL);
	filename = ft_strjoin(temp, ".temp");
	free(temp);
	return (filename);
}

char	*handle_heredoc(t_minishell *mini, t_command *cmd,
						t_token *tok, int index)
{
	char	*filename;
	int		ret;

	filename = get_filename(index);
	if (!filename)
		exit_with_error("malloc filename failed\n", 1, 2);
	ret = here_doc(tok->value, filename, mini, tok->quote);
	if (ret == -1)
	{
		free(filename);
		ft_putstr("Error reading heredoc\n", 2);
		exit(1);
	}
	if (ret == 130)
	{
		free(filename);
		g_status = 130;
		return (NULL);
	}
	cmd->is_heredoc = 1;
	cmd->heredoc_file = filename;
	return (filename);
}

void	process_heredoc_2(t_minishell *mini, t_token **token, int *index)
{
	char	*filename;

	filename = handle_heredoc(mini, mini->curr,
			(*token)->next, *index);
	if (!filename)
	{
		g_status = 130;
		mini->curr->is_heredoc = 0;
		return ;
	}
	add_redir_to_cmd(mini, T_HEREDOC, filename);
	mini->curr->is_heredoc = 1;
	mini->pipex_data->count_heredoc++;
	(*index)++;
	*token = (*token)->next;
}

void	parse_heredoc(t_minishell *mini, t_token **token, int *index)
{
	if (!(*token)->next || !(*token)->next->value)
	{
		if (mini->pipex_data)
		{
			free_pipex_data(mini->pipex_data);
			mini->pipex_data = NULL;
		}
		mini->curr->redirs = NULL;
		exit_with_error("heredoc: missing limiter\n", 1, 2);
		return ;
	}
	if ((*token)->next->type != T_WORD || !(*token)->next->value
		|| ft_strchr("<>|", (*token)->next->value[0]))
	{
		syntax_error_unexpected(mini, (*token)->next->value);
		return ;
	}
	process_heredoc_2(mini, token, index);
}
