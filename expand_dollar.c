/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_dollar.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sofernan <sofernan@student.42madrid.es>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/18 14:41:32 by sofernan          #+#    #+#             */
/*   Updated: 2025/10/07 15:03:57 by sofernan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "mini.h"

char	*copy_env_value(char *name, t_env *env)
{
	while (env)
	{
		if (!ft_strcmp(env->name, name))
		{
			if (env->value)
				return (ft_strdup(env->value));
			else
				return (NULL);
		}
		env = env->next;
	}
	return (NULL);
}

void	expand_exit_code(char **res, int *i, t_minishell *mini)
{
	char	*val;
	char	*tmp;

	val = copy_env_value("?", mini->env_list);
	tmp = *res;
	if (val)
		*res = ft_strjoin(*res, val);
	else
		*res = ft_strjoin(*res, "");
	free(tmp);
	free(val);
	*i += 2;
}

char	*get_env_value(char const *name, t_env *env)
{
	while (env)
	{
		if (ft_strcmp(env->name, name) == 0)
			return (env->value);
		env = env->next;
	}
	return (NULL);
}

void	expand_env_var(char **res, char *src, int *i, t_minishell *mini)
{
	char	*var;
	char	*value;
	int		j;
	int		len;
	char	*tmp;

	j = *i + 1;
	while (src[j] && (ft_isalnum(src[j]) || src[j] == '_'))
		j++;
	len = j - (*i + 1);
	tmp = *res;
	if (len > 0)
	{
		var = ft_substr(src, *i + 1, len);
		value = get_env_value(var, mini->env_list);
		if (!value)
			value = "";
		*res = ft_strjoin(tmp, value);
		free(var);
	}
	else
		*res = ft_strjoin(tmp, "$");
	free(tmp);
	*i = j;
}

void	expand_dollar(char **res, char *src, int *i, t_minishell *mini)
{
	if (src[*i + 1] == '?')
		expand_exit_code(res, i, mini);
	else
		expand_env_var(res, src, i, mini);
}
