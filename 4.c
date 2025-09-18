/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   5.c                                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sofernan <sofernan@student.42madrid.es>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/18 14:40:22 by sofernan          #+#    #+#             */
/*   Updated: 2025/09/18 16:01:57 by sofernan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "mini.h"

void	remove_env_var(char const *name, t_minishell *mini)
{
	t_env	*curr;
	t_env	*prev;

	curr = mini->env_list;
	prev = NULL;
	while (curr)
	{
		if (!ft_strcmp(curr->name, name))
		{
			if (prev)
				prev->next = curr->next;
			else
				mini->env_list = curr->next;
			free(curr->name);
			free(curr->value);
			free(curr);
			return ;
		}
		prev = curr;
		curr = curr->next;
	}
}

void	ft_unset(t_minishell *mini)
{
	int		i;

	i = 1;
	while (mini->command_list->argv[i])
	{
		remove_env_var(mini->command_list->argv[i], mini);
		i++;
	}
}
