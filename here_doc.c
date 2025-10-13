/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   here_doc.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sofernan <sofernan@student.42madrid.es>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/01 17:14:17 by sofernan          #+#    #+#             */
/*   Updated: 2025/10/13 14:42:18 by sofernan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "mini.h"

char	*get_filename(int index)
{
	char	*number;
	char	*filename;
	char	*tmp;

	number = ft_itoa(index);
	if (!number)
		return (NULL);
	tmp = ft_strjoin("minishell_", number);
	free(number);
	if (!tmp)
		return (NULL);
	filename = ft_strjoin(tmp, ".tmp");
	free(tmp);
	return (filename);
}

int	here_doc(const char *limiter, const char *filename,
		t_minishell *mini, t_token_quote quote)
{
	int	fd;
	int	save_in;
	int	result;

	save_in = dup(STDIN_FILENO);
	signal(SIGINT, heredoc_signal);
	fd = open(filename, O_CREAT | O_WRONLY | O_TRUNC, 0644);
	if (fd == -1)
	{
		if (save_in != -1)
			(dup2(save_in, STDIN_FILENO), close(save_in));
		return (-1);
	}
	if (!limiter || !*limiter)
		exit_with_error(SYNTAX_ERROR, 1, 2);
	result = process_heredoc(fd, limiter, mini, quote);
	return (here_doc_finish(fd, save_in, result));
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
		g_status = 1;
		return (NULL);
	}
	if (ret == 130)
	{
		unlink(filename);
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
