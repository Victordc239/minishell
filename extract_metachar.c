/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   extract_metachar.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sofernan <sofernan@student.42madrid.es>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/19 14:16:43 by sofernan          #+#    #+#             */
/*   Updated: 2025/09/29 17:36:55 by sofernan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "mini.h"

int	handle_heredoc_and_error(char *input, t_minishell *shell)
{
	int	count;

	count = 0;
	while (input[count] == '<')
		count++;
	if (count > 2)
	{
		ft_putstr("minishell: syntax error near unexpected token `<<<'\n",
			STDERR_FILENO);
		shell->tokenizer->err = 1;
		return (0);
	}
	if (count == 2)
	{
		shell->tokenizer->prev_type = T_HEREDOC;
		shell->tokenizer->pos += 2;
		return (1);
	}
	return (0);
}

int	handle_redirection_append(char *input, t_minishell *shell)
{
	if (input[0] == '>' && input[1] == '>')
	{
		shell->tokenizer->prev_type = T_RED_APPEND;
		shell->tokenizer->pos += 2;
		return (1);
	}
	return (0);
}

int	extract_double_metachar(t_minishell *shell)
{
	char	*input;

	input = shell->tokenizer->input + shell->tokenizer->pos;
	if (input[0] == '<' && input[1] == '>')
	{
		shell->tokenizer->prev_type = T_RED_INOUT;
		shell->tokenizer->pos += 2;
		return (1);
	}
	if (input[0] == '<')
		return (handle_heredoc_and_error(input, shell));
	if (input[0] == '>' && input[1] == '>')
		return (handle_redirection_append(input, shell));
	return (0);
}

int	extract_single_metachar(t_minishell *shell)
{
	char	c;

	c = shell->tokenizer->input[shell->tokenizer->pos];
	if (c == '|')
		shell->tokenizer->prev_type = T_PIPE;
	else if (c == '>')
		shell->tokenizer->prev_type = T_RED_OUT;
	else if (c == '<')
		shell->tokenizer->prev_type = T_RED_IN;
	else
		return (0);
	shell->tokenizer->pos++;
	return (1);
}

char	*extract_metachar(t_minishell *shell)
{
	char	*symbol;

	symbol = NULL;
	shell->tokenizer->err = 0;
	if (extract_double_metachar(shell))
		symbol = ft_substr(shell->tokenizer->input,
				shell->tokenizer->pos - 2, 2);
	else if (extract_single_metachar(shell))
		symbol = ft_substr(shell->tokenizer->input,
				shell->tokenizer->pos - 1, 1);
	return (symbol);
}
