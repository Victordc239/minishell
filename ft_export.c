/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_export.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sofernan <sofernan@student.42madrid.es>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/18 14:41:08 by sofernan          #+#    #+#             */
/*   Updated: 2025/10/06 18:48:56 by sofernan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "mini.h"

int	count_exported(t_minishell *mini)
{
	int		count;
	t_env	*tmp;
	t_env	*iter;

	tmp = mini->env_list;
	count = 0;
	iter = tmp;
	while (iter)
	{
		if (iter->exported)
			count++;
		iter = iter->next;
	}
	return (count);
}

void	sort_env_array(t_env **arr, int count)
{
	int		i;
	int		j;
	t_env	*tmp;

	i = 0;
	while (i < count - 1)
	{
		j = i + 1;
		while (j < count)
		{
			if (ft_strcmp(arr[i]->name, arr[j]->name) > 0)
			{
				tmp = arr[i];
				arr[i] = arr[j];
				arr[j] = tmp;
			}
			j++;
		}
		i++;
	}
}

void	print_env_array(t_env **arr, int count)
{
	int	i;

	i = 0;
	while (i < count)
	{
		printf("declare -x %s", arr[i]->name);
		if (arr[i]->value)
			printf("=\"%s\"", arr[i]->value);
		printf("\n");
		i++;
	}
}

void	print_sorted_env(t_minishell *mini)
{
	int		count;
	int		i;
	t_env	**arr;
	t_env	*tmp;

	count = count_exported(mini);
	arr = malloc(sizeof(t_env *) * count);
	if (!arr)
		return ;
	i = 0;
	tmp = mini->env_list;
	while (tmp)
	{
		if (tmp->exported)
			arr[i++] = tmp;
		tmp = tmp->next;
	}
	sort_env_array(arr, count);
	print_env_array(arr, count);
	free(arr);
}

void	ft_export(t_minishell *mini)
{
	int	i;
	int	error;

	i = 1;
	error = 0;
	if (!mini->cmd_list->argv[1])
	{
		print_sorted_env(mini);
		return ;
	}
	while (mini->cmd_list->argv[i])
	{
		error = process_export_arg(mini->cmd_list->argv[i], mini);
		i++;
	}
	g_status = error;
}
