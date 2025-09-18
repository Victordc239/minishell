/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   6.c                                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sofernan <sofernan@student.42madrid.es>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/18 14:43:09 by sofernan          #+#    #+#             */
/*   Updated: 2025/09/18 15:29:16 by sofernan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "mini.h"

void	update_node_value(t_env *tmp, char *value, int exported)
{
	if (tmp->value)
		free(tmp->value);
	if (value)
		tmp->value = ft_strdup(value);
	else
		tmp->value = NULL;
	tmp->exported = exported;
}

int	update_node(t_env *tmp, char *name, char *value, int exported)
{
	while (tmp)
	{
		if (!ft_strcmp(tmp->name, name))
		{
			update_node_value(tmp, value, exported);
			return (1);
		}
		tmp = tmp->next;
	}
	return (0);
}

t_env	*create_new_node(char *name, char *value, int exported)
{
	t_env	*new;

	new = malloc(sizeof(t_env));
	if (!new)
		return (NULL);
	new->name = ft_strdup(name);
	if (!new->name)
	{
		free(new);
		return (NULL);
	}
	if (value)
	{
		new->value = ft_strdup(value);
	}
	else
	{
		new->value = NULL;
	}
	new->exported = exported;
	new->signal = 0;
	new->next = NULL;
	return (new);
}

void	append_node_to_list(t_minishell *mini, t_env *new)
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

void	add_env_node(t_minishell *mini, char *name, char *value, int exported)
{
	t_env	*tmp;
	t_env	*new;

	tmp = mini->env_list;
	if (update_node(tmp, name, value, exported))
		return ;
	new = create_new_node(name, value, exported);
	if (!new)
		return ;
	append_node_to_list(mini, new);
}
