/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_cd.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sofernan <sofernan@student.42madrid.es>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/02 17:23:37 by sofernan          #+#    #+#             */
/*   Updated: 2025/10/14 15:10:15 by sofernan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "mini.h"

char	*get_cd_path(t_minishell *mini, int *free_path, int *print_new)
{
	char	*path;

	if (mini->cmd_list->argv[1] && mini->cmd_list->argv[2])
		return (ft_putstr("cd: too many arguments\n", 2), g_status = 1, NULL);
	if (!mini->cmd_list->argv[1])
	{
		path = copy_env_value("HOME", mini->env_list);
		if (!path)
			return (ft_putstr("cd: HOME not set\n", 2), g_status = 1, NULL);
		return (*free_path = 1, path);
	}
	if (ft_strcmp(mini->cmd_list->argv[1], "-") == 0)
	{
		path = copy_env_value("OLDPWD", mini->env_list);
		if (!path)
			return (ft_putstr("cd: OLDPWD not set\n", 2), g_status = 1, NULL);
		return (*free_path = 1, *print_new = 1, path);
	}
	return (mini->cmd_list->argv[1]);
}

char	*execute_cd(t_minishell *mini, char *path,
						int free_path, char **prev_cwd)
{
	char	*new_cwd;

	*prev_cwd = getcwd(NULL, 0);
	if (!*prev_cwd)
		*prev_cwd = copy_env_value("PWD", mini->env_list);
	if (chdir(path) == -1)
	{
		perror("cd");
		g_status = 1;
		if (free_path)
			free(path);
		if (*prev_cwd)
			free(*prev_cwd);
		return (NULL);
	}
	g_status = 0;
	new_cwd = getcwd(NULL, 0);
	if (!new_cwd)
	{
		if (path)
			new_cwd = ft_strdup(path);
		else
			new_cwd = ft_strdup("");
	}
	return (new_cwd);
}

void	ft_cd(t_minishell *mini, int free_path, int print_new)
{
	char	*path;
	char	*prev_cwd;
	char	*new_cwd;

	path = get_cd_path(mini, &free_path, &print_new);
	if (!path)
		return ;
	new_cwd = execute_cd(mini, path, free_path, &prev_cwd);
	if (!new_cwd)
		return ;
	if (prev_cwd)
	{
		if (!update_env_var(mini->env_list, "OLDPWD", prev_cwd, 1))
			set_env_var(mini, "OLDPWD", prev_cwd, 1);
		free(prev_cwd);
	}
	if (!update_env_var(mini->env_list, "PWD", new_cwd, 1))
		set_env_var(mini, "PWD", new_cwd, 1);
	if (print_new)
		(ft_putstr(new_cwd, 1), ft_putstr("\n", 1));
	free(new_cwd);
	if (free_path)
		free(path);
}
