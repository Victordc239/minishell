/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_exit.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sofernan <sofernan@student.42madrid.es>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/18 14:36:45 by sofernan          #+#    #+#             */
/*   Updated: 2025/09/18 16:41:51 by sofernan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "mini.h"

int	is_numeric(char const *str)
{
	int	i;

	i = 0;
	if (!str)
		return (0);
	if (str[i] == '-' || str[i] == '+')
		i++;
	while (str[i])
	{
		if (!ft_isdigit(str[i]))
			return (0);
		i++;
	}
	return (1);
}

int	ft_exit(t_minishell *mini)
{
	int	code;

	if (!mini->command_list->argv[1])
	{
		free_minishell(mini);
		exit(g_status);
	}
	if (mini->command_list->argv[2])
	{
		ft_putstr("exit: too many arguments\n", 2);
		g_status = 1;
		return (1);
	}
	if (!is_numeric(mini->command_list->argv[1]))
	{
		ft_putstr("exit: ", 2);
		ft_putstr(mini->command_list->argv[1], 2);
		ft_putstr(": numeric argument required\n", 2);
		free_minishell(mini);
		exit(2);
	}
	code = ft_atoi(mini->command_list->argv[1]);
	free_minishell(mini);
	exit(code % 256);
}

void	ft_cd(t_minishell *mini)
{
	char	*path;

	if (mini->command_list->argv[1] && mini->command_list->argv[2])
	{
		ft_putstr("cd: too many arguments\n", 2);
		g_status = 1;
		return ;
	}
	if (!mini->command_list->argv[1])
	{
		path = get_env_value("HOME", mini->env_list);
		if (!path)
		{
			(ft_putstr("cd: HOME not set\n", 2), g_status = 1);
			return ;
		}
	}
	else
		path = mini->command_list->argv[1];
	if (chdir(path) == -1)
		(perror("cd"), g_status = 1);
	else
		g_status = 0;
	if (!mini->command_list->argv[1])
		free(path);
}
