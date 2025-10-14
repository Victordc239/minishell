/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free_minishell.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sofernan <sofernan@student.42madrid.es>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/18 14:41:20 by sofernan          #+#    #+#             */
/*   Updated: 2025/10/14 15:09:20 by sofernan         ###   ########.fr       */
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

void	free_token_list(t_token *list)
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
	if (tokenizer->input)
		free(tokenizer->input);
	free(tokenizer);
}

void	free_str_array(char **str)
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

void	free_minishell(t_minishell *mini)
{
	if (!mini)
		return ;
	if (mini->saved_stdin >= 0)
	{
		close(mini->saved_stdin);
		mini->saved_stdin = -1;
	}
	if (mini->env_list)
		(free_env_list(mini->env_list), mini->env_list = NULL);
	if (mini->t_list)
		(free_token_list(mini->t_list), mini->t_list = NULL);
	if (mini->pipex_data)
		(free_pipex_data(mini->pipex_data), mini->pipex_data = NULL);
	if (mini->tokenizer)
		(free_tokenizer(mini->tokenizer), mini->tokenizer = NULL);
	free_str_array(mini->envir_execve);
	free_str_array(mini->paths_execve);
	mini->curr_token = NULL;
	mini->new_token = NULL;
	mini->new_node = NULL;
	mini->current = NULL;
	mini->head = NULL;
	mini->tmp = NULL;
	mini->curr = NULL;
}
