/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_commands.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sofernan <sofernan@student.42madrid.es>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/01 16:49:39 by sofernan          #+#    #+#             */
/*   Updated: 2025/10/01 16:50:47 by sofernan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "mini.h"

t_command	*init_new_command(void)
{
	t_command	*cmd;

	cmd = ft_calloc(1, sizeof(t_command));
	if (!cmd)
		return (NULL);
	cmd->argv = NULL;
	cmd->redirs = NULL;
	cmd->infile = NULL;
	cmd->outfile = NULL;
	cmd->append = 0;
	cmd->is_heredoc = 0;
	cmd->heredoc_file = NULL;
	cmd->next = NULL;
	return (cmd);
}

void	add_command_to_list(t_minishell *mini)
{
	if (!mini->head)
	{
		mini->head = mini->curr;
		return ;
	}
	mini->tmp = mini->head;
	while (mini->tmp->next)
		mini->tmp = mini->tmp->next;
	mini->tmp->next = mini->curr;
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

t_command	*parse_commands(t_minishell *mini)
{
	int	heredoc_index;

	heredoc_index = 0;
	mini->head = NULL;
	mini->curr = NULL;
	while (mini->t_list)
	{
		if (g_status == 2)
			break ;
		if (!mini->curr)
		{
			mini->curr = init_new_command();
			add_command_to_list(mini);
		}
		process_token(mini, &heredoc_index);
		mini->t_list = mini->t_list->next;
	}
	return (mini->head);
}
