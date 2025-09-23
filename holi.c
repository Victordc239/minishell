/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   holi.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sofernan <sofernan@student.42madrid.es>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/30 15:25:53 by sofernan          #+#    #+#             */
/*   Updated: 2025/09/23 14:04:18 by sofernan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "mini.h"

int	match_glob(const char *p, const char *str)
{
	if (*p == '\0')
		return (*str == '\0');
	if (*p == '*')
	{
		while (*p == '*')
			p++;
		if (*p == '\0')
			return (1);
		while (*str != '\0' && !match_glob(p, str))
			str++;
		return (match_glob(p, str));
	}
	if (*p == '[')
	{
		if (*str == '\0')
			return (0);
		if (match_class(&p, *str) <= 0)
			return (0);
		return (match_glob(p, str + 1));
	}
	if (*str == '\0' || *p != *str)
		return (0);
	return (match_glob(p + 1, str + 1));
}

void	append_variable(char **res, char *src, int *i, t_minishell *mini)
{
	char	*var;
	char	*val;
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
		val = env_value(var, mini->env_list);
		*res = ft_strjoin(tmp, val ? val : "");
		free(var);
	}
	else
		*res = ft_strjoin(tmp, "$");
	free(tmp);
	*i = j;
}
	