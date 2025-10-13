/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   extract_metachar.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sofernan <sofernan@student.42madrid.es>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/19 14:16:43 by sofernan          #+#    #+#             */
/*   Updated: 2025/10/13 20:01:42 by sofernan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "mini.h"

int	process_heredoc_token(char *input, t_minishell *mini)
{
	int	count;

	count = 0;
	while (input[count] == '<')
		count++;
	if (count > 2)
	{
		ft_putstr("minishell: syntax error near unexpected token `<<<'\n",
			STDERR_FILENO);
		mini->tokenizer->err = 1;
		return (0);
	}
	if (count == 2)
	{
		mini->tokenizer->prev_type = T_HEREDOC;
		mini->tokenizer->pos += 2;
		return (1);
	}
	return (0);
}

int	process_redir_append(char *input, t_minishell *mini)
{
	if (input[0] == '>' && input[1] == '>')
	{
		mini->tokenizer->prev_type = T_RED_APPEND;
		mini->tokenizer->pos += 2;
		return (1);
	}
	return (0);
}

int	extract_double_metachar(t_minishell *mini)
{
	char	*input;

	input = mini->tokenizer->input + mini->tokenizer->pos;
	if (input[0] == '<' && input[1] == '>')
	{
		mini->tokenizer->prev_type = T_RED_INOUT;
		mini->tokenizer->pos += 2;
		return (1);
	}
	if (input[0] == '<')
		return (process_heredoc_token(input, mini));
	if (input[0] == '>' && input[1] == '>')
		return (process_redir_append(input, mini));
	return (0);
}

int	extract_single_metachar(t_minishell *mini)
{
	char	c;

	c = mini->tokenizer->input[mini->tokenizer->pos];
	if (c == '|')
		mini->tokenizer->prev_type = T_PIPE;
	else if (c == '>')
		mini->tokenizer->prev_type = T_RED_OUT;
	else if (c == '<')
		mini->tokenizer->prev_type = T_RED_IN;
	else
		return (0);
	mini->tokenizer->pos++;
	return (1);
}

char	*extract_metachar(t_minishell *mini)
{
	char	*symbol;

	symbol = NULL;
	mini->tokenizer->err = 0;
	if (extract_double_metachar(mini))
		symbol = ft_substr(mini->tokenizer->input,
				mini->tokenizer->pos - 2, 2);
	else if (extract_single_metachar(mini))
		symbol = ft_substr(mini->tokenizer->input,
				mini->tokenizer->pos - 1, 1);
	return (symbol);
}
