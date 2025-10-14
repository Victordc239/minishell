/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenize_input.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sofernan <sofernan@student.42madrid.es>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/19 14:30:35 by sofernan          #+#    #+#             */
/*   Updated: 2025/10/14 15:29:19 by sofernan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "mini.h"

t_token	*create_token_node(t_minishell *mini, char *value)
{
	t_token	*new_node;

	new_node = malloc(sizeof(t_token));
	if (!new_node)
		return (NULL);
	new_node->value = ft_strdup(value);
	if (mini && mini->tokenizer)
		new_node->type = mini->tokenizer->prev_type;
	else
		new_node->type = T_WORD;
	new_node->quote = Q_NONE;
	new_node->expansion_type = NO_EXPANSION;
	new_node->next = NULL;
	if (mini && mini->tokenizer)
	{
		new_node->adjacent = mini->tokenizer->last_adjacent;
		mini->tokenizer->last_adjacent = 0;
	}
	else
		new_node->adjacent = 0;
	return (new_node);
}

t_token	*add_token(t_minishell *mini, char *value)
{
	t_token	*new_node;
	t_token	*current;

	new_node = create_token_node(mini, value);
	if (!new_node)
		return (NULL);
	if (mini->t_list == NULL)
		mini->t_list = new_node;
	else
	{
		current = mini->t_list;
		while (current->next)
			current = current->next;
		current->next = new_node;
	}
	return (new_node);
}

void	replace_char_inplace(char *s, char find, char replace)
{
	int	i;

	i = 0;
	if (!s)
		return ;
	while (s[i])
	{
		if (s[i] == find)
			s[i] = replace;
		i++;
	}
}

t_token	*check_expansion(t_minishell *mini, char *val)
{
	t_token	*new_token;

	new_token = add_token(mini, val);
	if (!new_token)
		return (NULL);
	new_token->type = mini->tokenizer->prev_type;
	new_token->quote = mini->tokenizer->quote;
	if (ft_strchr(new_token->value, '\x01')
		&& !ft_strchr(new_token->value, '$'))
		delete_marker_inplace(new_token->value);
	if (new_token->type == T_WORD && new_token->value[0] == '$')
	{
		if (new_token->quote == Q_SINGLE)
			new_token->expansion_type = NO_EXPANSION;
		else if (new_token->value[1] == '?' && new_token->value[2] == '\0')
			new_token->expansion_type = EXIT_STATUS_EXPANSION;
		else
			new_token->expansion_type = VAR_EXPANSION;
	}
	else
		new_token->expansion_type = NO_EXPANSION;
	if (new_token->quote == Q_SINGLE && ft_strchr(new_token->value, '\x07'))
		replace_char_inplace(new_token->value, '\x07', '$');
	return (new_token);
}

int	tokenize_input(t_minishell *mini)
{
	char	*val;

	while (!mini->tokenizer->err)
	{
		val = extract_next_token(mini);
		if (mini->tokenizer->err || !val)
			break ;
		if (!check_expansion(mini, val))
		{
			free(val);
			return (0);
		}
		free(val);
	}
	return (1);
}
