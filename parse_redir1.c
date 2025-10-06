/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_redir1.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sofernan <sofernan@student.42madrid.es>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/18 14:42:01 by sofernan          #+#    #+#             */
/*   Updated: 2025/10/06 19:01:15 by sofernan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "mini.h"

void	syntax_error_unexpected(t_minishell *mini, const char *tok)
{
	char	*m;
	char	*tmp;

	if (mini && mini->curr)
		mini->curr->redirs = NULL;
	if (!tok || *tok == '\0')
	{
		ft_putstr("mini: ", 2);
		ft_putstr("syntax error near unexpected token `newline'\n", 2);
	}
	else
	{
		m = ft_strjoin("mini: syntax error near unexpected token `", tok);
		tmp = ft_strjoin(m, "'\n");
		free(m);
		ft_putstr(tmp, 2);
		free(tmp);
	}
	g_status = 2;
}

t_redir	*init_redir(int type, char const *filename)
{
	t_redir	*redir;

	redir = malloc(sizeof(t_redir));
	if (!redir)
		return (NULL);
	redir->type = type;
	redir->filename = ft_strdup(filename);
	if (!redir->filename)
		return (free(redir), NULL);
	redir->next = NULL;
	return (redir);
}

void	add_redir_to_cmd(t_minishell *mini, int type, char const *filename)
{
	t_redir	*new;

	if (!mini || !mini->curr)
		exit_with_error("mini or current command is NULL\n", 1, 2);
	new = init_redir(type, filename);
	if (!new)
		exit_with_error("init_redir failed\n", 1, 2);
	if (!mini->curr->redirs)
	{
		mini->curr->redirs = new;
		mini->curr->last_redir = new;
	}
	else
	{
		mini->curr->last_redir->next = new;
		mini->curr->last_redir = new;
	}
}

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

void	parse_red_inout(t_minishell *mini, t_token **token)
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
	add_redir_to_cmd(mini, T_RED_INOUT, next->value);
	*token = next;
}
