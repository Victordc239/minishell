/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   12.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sofernan <sofernan@student.42madrid.es>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/18 14:41:20 by sofernan          #+#    #+#             */
/*   Updated: 2025/09/18 16:31:27 by sofernan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "mini.h"

void	free_env_list(t_env *env)
{
	t_env	*tmp;

	while (env)
	{
		tmp = env->next;
		if (env->name)
			free(env->name);
		if (env->value)
			free(env->value);
		free(env);
		env = tmp;
	}
}

void	free_t_list(t_token *list)
{
	t_token	*tmp;

	while (list)
	{
		tmp = list->next;
		if (list->value)
			free(list->value);
		free(list);
		list = tmp;
	}
}

void	free_tokenizer(t_tokenizer *tokenizer)
{
	if (!tokenizer)
		return ;
	free(tokenizer);
}

void	ft_freedoom(char **str)
{
	int	i;

	if (!str)
		return ;
	i = 0;
	while (str[i])
	{
		free(str[i]);
		str[i] = NULL;
		i++;
	}
	free(str);
}

void	free_minishell(t_minishell *shell)
{
	if (!shell)
		return ;
	if (shell->env_list)
		(free_env_list(shell->env_list), shell->env_list = NULL);
	if (shell->t_list)
		(free_t_list(shell->t_list), shell->t_list = NULL);
	if (shell->pipex_data && shell->command_list)
		(free_pipex_data(shell->pipex_data), shell->pipex_data = NULL);
	if (shell->tokenizer)
	{
		free_tokenizer(shell->tokenizer);
		shell->tokenizer = NULL;
	}
	ft_freedoom(shell->envir_execve);
	ft_freedoom(shell->paths_execve);
	shell->curr_token = NULL;
	shell->new_token = NULL;
	shell->new_node = NULL;
	shell->current = NULL;
	shell->head = NULL;
	shell->tmp = NULL;
	shell->curr = NULL;
}
