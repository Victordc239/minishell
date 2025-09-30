/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   29.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sofernan <sofernan@student.42madrid.es>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/19 14:44:25 by sofernan          #+#    #+#             */
/*   Updated: 2025/09/30 17:14:05 by sofernan         ###   ########.fr       */
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
		ft_putstr("minishell: ", 2);
		ft_putstr("syntax error near unexpected token `newline'\n", 2);
	}
	else
	{
		m = ft_strjoin("minishell: syntax error near unexpected token `", tok);
		tmp = ft_strjoin(m, "'\n");
		free(m);
		ft_putstr(tmp, 2);
		free(tmp);
	}
	g_status = 2;
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

int	ft_isspace(int c)
{
	if (c == ' ' || c == '\t' || c == '\n'
		|| c == '\v' || c == '\f' || c == '\r')
		return (1);
	return (0);
}

int	ends_with_unquoted_redir(const char *s, int in_sq, int in_dq)
{
	int	i;

	in_sq = 0;
	in_dq = 0;
	i = 0;
	if (!s)
		return (0);
	while (s[i])
	{
		if (s[i] == '\'' && !in_dq)
			in_sq = !in_sq;
		else if (s[i] == '"' && !in_sq)
			in_dq = !in_dq;
		i++;
	}
	i--;
	while (i >= 0 && (s[i] == '\0' || ft_isspace((unsigned char)s[i])))
		i--;
	if (i < 0)
		return (0);
	if ((i - 1 >= 0 && s[i] == '>' && s[i - 1] == '>')
		|| (i - 1 >= 0 && s[i] == '<' && s[i - 1] == '<')
		|| (s[i] == '<') || (s[i] == '>'))
		return (1);
	return (0);
}

int	ends_with_unquoted_continuation_op(const char *s)
{
	int	i;
	int	in_sq;
	int	in_dq;

	in_dq = 0;
	in_sq = 0;
	i = 0;
	if (!s)
		return (0);
	while (s[i])
	{
		if (s[i] == '\'' && !in_dq)
			in_sq = !in_sq;
		else if (s[i] == '"' && !in_sq)
			in_dq = !in_dq;
		i++;
	}
	i--;
	while (i >= 0 && ft_isspace((unsigned char)s[i]))
		i--;
	if (i < 0)
		return (0);
	if ((s[i] == '|') || (s[i] == '&' && i - 1 >= 0 && s[i - 1] == '&'))
		return (1);
	return (0);
}
