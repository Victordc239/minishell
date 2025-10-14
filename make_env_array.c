/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   make_env_array.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sofernan <sofernan@student.42madrid.es>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/18 14:40:58 by sofernan          #+#    #+#             */
/*   Updated: 2025/10/14 15:09:20 by sofernan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "mini.h"

int	count_env_var(t_env	*env)
{
	int	count;

	count = 0;
	while (env)
	{
		if (env->exported && env->name && env->value)
			count++;
		env = env->next;
	}
	return (count);
}

char	*make_env_entry(t_env *node)
{
	char	*entry;
	char	*result;

	if (!node || !node->name || node->value == NULL)
		return (NULL);
	entry = ft_strjoin(node->name, "=");
	if (!entry)
		return (NULL);
	result = ft_strjoin(entry, node->value);
	free(entry);
	return (result);
}

char	**make_env_array(t_env *env_list)
{
	char	**env_array;
	int		i;
	int		size;

	size = count_env_var(env_list);
	i = 0;
	env_array = malloc(sizeof(char *) * (size + 1));
	if (!env_array)
		return (NULL);
	while (env_list)
	{
		if (env_list->exported && env_list->name && env_list->value)
		{
			env_array[i] = make_env_entry(env_list);
			if (!env_array[i])
			{
				env_array[i] = NULL;
				return (free_str_array(env_array), NULL);
			}
			i++;
		}
		env_list = env_list->next;
	}
	env_array[i] = NULL;
	return (env_array);
}
