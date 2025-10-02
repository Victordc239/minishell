/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   2.c                                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sofernan <sofernan@student.42madrid.es>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/01 17:05:13 by sofernan          #+#    #+#             */
/*   Updated: 2025/10/01 17:16:18 by sofernan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "mini.h"

int	is_all_digits(const char *s)
{
	int	i;

	if (!s || s[0] == '\0')
		return (0);
	i = 0;
	while (s[i])
	{
		if (!ft_isdigit((unsigned char)s[i]))
			return (0);
		i++;
	}
	return (1);
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
