/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   16.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sofernan <sofernan@student.42madrid.es>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/18 14:41:43 by sofernan          #+#    #+#             */
/*   Updated: 2025/09/23 16:02:08 by sofernan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "mini.h"

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
