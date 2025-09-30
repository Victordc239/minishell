/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   20ok.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sofernan <sofernan@student.42madrid.es>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/18 14:42:01 by sofernan          #+#    #+#             */
/*   Updated: 2025/09/29 17:33:21 by sofernan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "mini.h"

void	parse_red_in(t_minishell *mini, t_token **token)
{
	t_token	*next;

	if (!(*token)->next)
	{
		if (mini->pipex_data)
		{
			free_pipex_data(mini->pipex_data);
			mini->pipex_data = NULL;
		}
		mini->curr->redirs = NULL;
		exit_with_error("syntax error: no infile\n", 1, 2);
		return ;
	}
	next = (*token)->next;
	if (next->type != T_WORD || !next->value
		|| ft_strchr("<>|", next->value[0]))
	{
		syntax_error_unexpected(mini, next->value);
		return ;
	}
	add_redir_to_cmd(mini, T_RED_IN, next->value);
	*token = next;
}

t_token	*get_next_valid_outfile(t_minishell *mini, t_token *token)
{
	t_token	*next;

	next = token->next;
	if (!next)
	{
		if (mini->pipex_data)
		{
			free_pipex_data(mini->pipex_data);
			mini->pipex_data = NULL;
		}
		mini->curr->redirs = NULL;
		exit_with_error("syntax error: no outfile\n", 1, 2);
		return (NULL);
	}
	if (next->type != T_WORD || !next->value
		|| ft_strchr("<>|", next->value[0]))
		return (syntax_error_unexpected(mini, next->value), NULL);
	if (is_all_digits(next->value) && next->adjacent && next->next
		&& (next->next->type == T_RED_IN || next->next->type == T_RED_OUT
			|| next->next->type == T_RED_APPEND || next->next->type == T_HEREDOC
			|| next->next->type == T_RED_INOUT))
		return (syntax_error_unexpected(mini, next->value), NULL);
	return (next);
}

void	parse_red_out(t_minishell *mini, t_token **token)
{
	t_token	*next;

	next = get_next_valid_outfile(mini, *token);
	if (!next)
		return ;
	add_redir_to_cmd(mini, T_RED_OUT, next->value);
	*token = next;
}

t_token	*get_next_token_for_append(t_minishell *mini, t_token **token)
{
	t_token	*next;

	if (!(*token)->next)
	{
		if (mini->pipex_data)
		{
			free_pipex_data(mini->pipex_data);
			mini->pipex_data = NULL;
		}
		mini->curr->redirs = NULL;
		exit_with_error("syntax error: no outfile\n", 1, 2);
		return (NULL);
	}
	next = (*token)->next;
	if (next->type != T_WORD || !next->value
		|| ft_strchr("<>|", next->value[0]))
		return (syntax_error_unexpected(mini, next->value), NULL);
	if (is_all_digits(next->value) && next->adjacent && next->next
		&& (next->next->type == T_RED_IN || next->next->type == T_RED_OUT
			|| next->next->type == T_RED_APPEND || next->next->type == T_HEREDOC
			|| next->next->type == T_RED_INOUT))
	{
		return (syntax_error_unexpected(mini, next->value), NULL);
	}
	return (next);
}

void	parse_red_append(t_minishell *mini, t_token **token)
{
	t_token	*next;

	next = get_next_token_for_append(mini, token);
	if (!next)
		return ;
	add_redir_to_cmd(mini, T_RED_APPEND, next->value);
	mini->curr->append = 1;
	*token = next;
}

void	process_heredoc_2(t_minishell *mini, t_token **token, int *index)
{
	char	*filename;

	filename = handle_heredoc(mini->curr, (*token)->next->value, *index);
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

void	process_token(t_minishell *mini, int *index)
{
	t_token	*token;

	token = mini->t_list;
	if (token->type == T_WORD)
		handle_word_token(mini, token);
	else if (token->type == T_RED_IN && token->next)
		parse_red_in(mini, &mini->t_list);
	else if (token->type == T_RED_INOUT && token->next)
		parse_red_inout(mini, &mini->t_list);
	else if (token->type == T_RED_OUT && token->next)
		parse_red_out(mini, &mini->t_list);
	else if (token->type == T_RED_APPEND && token->next)
		parse_red_append(mini, &mini->t_list);
	else if (token->type == T_HEREDOC && token->next)
		parse_heredoc(mini, &mini->t_list, index);
	else if (token->type == T_PIPE)
		mini->curr = NULL;
}
