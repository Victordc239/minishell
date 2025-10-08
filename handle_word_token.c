/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_word_token.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sofernan <sofernan@student.42madrid.es>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/18 14:41:57 by sofernan          #+#    #+#             */
/*   Updated: 2025/10/07 14:19:27 by sofernan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "mini.h"

void	append_literal(char **res, char *src, int len)
{
	char	*lit;
	char	*tmp;

	lit = ft_substr(src, 0, len);
	tmp = *res;
	*res = ft_strjoin(tmp, lit);
	free(tmp);
	free(lit);
}

char	*expand_env_in_str(char *src, t_minishell *mini)
{
	char	*res;
	int		i;
	int		start;

	res = ft_strdup("");
	i = 0;
	while (src[i])
	{
		if (src[i] == '$')
			expand_dollar(&res, src, &i, mini);
		else
		{
			start = i;
			while (src[i] && src[i] != '$')
				i++;
			append_literal(&res, src + start, i - start);
		}
	}
	return (res);
}

void	remove_marker_inplace(char *s)
{
	size_t	i;
	size_t	j;

	if (!s)
		return ;
	i = 0;
	j = 0;
	while (s[i])
	{
		if (s[i] != '\x01')
			s[j++] = s[i];
		i++;
	}
	s[j] = '\0';
}

void	expand_token(t_token *token, t_minishell *mini)
{
	char	*expanded;
	char	*home;
	char	*tmp;

	if (token->quote == Q_SINGLE)
		return ;
	expanded = expand_env_in_str(token->value, mini);
	free(token->value);
	token->value = expanded;
	remove_marker_inplace(token->value);
	if (token->quote == Q_NONE && token->value && token->value[0] == '~'
		&& (token->value[1] == '/' || token->value[1] == '\0'))
	{
		home = copy_env_value("HOME", mini->env_list);
		if (home)
		{
			tmp = ft_strjoin(home, token->value + 1);
			free(home);
			if (tmp)
				(free(token->value), token->value = tmp);
		}
	}
	if (ft_strchr(token->value, '\x07'))
		replace_char_inplace(token->value, '\x07', '$');
}

void	handle_word_token(t_minishell *mini, t_token *token)
{
	if (token->quote != Q_SINGLE
		&& (ft_strchr(token->value, '$')
			|| ft_strchr(token->value, '\x07')
			|| token->value[0] == '~'))
		expand_token(token, mini);
	if (token->value[0] == '\0' && token->quote != Q_SINGLE)
		return ;
	if (token->quote == Q_NONE && (ft_strchr(token->value, '*')
			|| ft_strchr(token->value, '?') || ft_strchr(token->value, '[')))
		expand_and_add_glob(token->value, mini);
	else
		add_arg_to_command(mini, token->value);
}
