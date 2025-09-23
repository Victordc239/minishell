/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   41.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sofernan <sofernan@student.42madrid.es>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/30 15:25:53 by sofernan          #+#    #+#             */
/*   Updated: 2025/09/23 14:16:13 by sofernan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "mini.h"

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
		if (!val)
			val = "";
		*res = ft_strjoin(tmp, val);
		free(var);
	}
	else
		*res = ft_strjoin(tmp, "$");
	free(tmp);
	*i = j;
}
