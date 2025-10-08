/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   set_env_var.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sofernan <sofernan@student.42madrid.es>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/18 14:43:09 by sofernan          #+#    #+#             */
/*   Updated: 2025/10/07 15:04:19 by sofernan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "mini.h"

int	update_env_var(t_env *tmp, char *name, char *value, int exported)
{
	while (tmp)
	{
		if (!ft_strcmp(tmp->name, name))
		{
			if (tmp->value)
				free(tmp->value);
			if (value)
				tmp->value = ft_strdup(value);
			else
				tmp->value = NULL;
			tmp->exported = exported;
			return (1);
		}
		tmp = tmp->next;
	}
	return (0);
}

t_env	*create_env_var(char *name, char *value, int exported)
{
	t_env	*new;

	new = malloc(sizeof(t_env));
	if (!new)
		return (NULL);
	new->name = ft_strdup(name);
	if (!new->name)
		return (free(new), NULL);
	if (value)
	{
		new->value = ft_strdup(value);
		if (!new->value)
		{
			free(new->name);
			free(new);
			return (NULL);
		}
	}
	else
		new->value = NULL;
	new->exported = exported;
	new->signal = 0;
	new->next = NULL;
	return (new);
}

void	add_node_env_list(t_minishell *mini, t_env *new)
{
	t_env	*tmp;

	if (!mini->env_list)
	{
		mini->env_list = new;
	}
	else
	{
		tmp = mini->env_list;
		while (tmp->next)
			tmp = tmp->next;
		tmp->next = new;
	}
}

void	set_env_var(t_minishell *mini, char *name, char *value, int exported)
{
	t_env	*tmp;
	t_env	*new;

	tmp = mini->env_list;
	if (update_env_var(tmp, name, value, exported))
		return ;
	new = create_env_var(name, value, exported);
	if (!new)
		return ;
	add_node_env_list(mini, new);
}
