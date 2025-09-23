/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   41.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sofernan <sofernan@student.42madrid.es>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/30 15:25:53 by sofernan          #+#    #+#             */
/*   Updated: 2025/09/23 17:14:25 by sofernan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "mini.h"

 char	*get_cd_path(t_minishell *mini, int *path_allocated, int *print_new)
{
	char	*path;

	if (mini->command_list->argv[1] && mini->command_list->argv[2])
		return (ft_putstr("cd: too many arguments\n", 2), g_status = 1, NULL);
	if (!mini->command_list->argv[1])
	{
		path = get_env_value("HOME", mini->env_list);
		if (!path)
			return (ft_putstr("cd: HOME not set\n", 2), g_status = 1, NULL);
		return (*path_allocated = 1, path);
	}
	if (ft_strcmp(mini->command_list->argv[1], "-") == 0)
	{
		path = get_env_value("OLDPWD", mini->env_list);
		if (!path)
			return (ft_putstr("cd: OLDPWD not set\n", 2), g_status = 1, NULL);
		return (*path_allocated = 1, *print_new = 1, path);
	}
	return (mini->command_list->argv[1]);
}

 char	*perform_cd(t_minishell *mini, char *path,
				int path_allocated, char **prev_cwd)
{
	char	*new_cwd;

	*prev_cwd = getcwd(NULL, 0);
	if (!*prev_cwd)
		*prev_cwd = get_env_value("PWD", mini->env_list);
	if (chdir(path) == -1)
	{
		perror("cd");
		g_status = 1;
		if (path_allocated)
			free(path);
		if (*prev_cwd)
			free(*prev_cwd);
		return (NULL);
	}
	g_status = 0;
	new_cwd = getcwd(NULL, 0);
	if (!new_cwd)
		new_cwd = ft_strdup(path ? path : "");
	return (new_cwd);
}

void	ft_cd(t_minishell *mini, int path_allocated, int print_new)
{
	char	*path;
	char	*prev_cwd;
	char	*new_cwd;

	path = get_cd_path(mini, &path_allocated, &print_new);
	if (!path)
		return ;
	new_cwd = perform_cd(mini, path, path_allocated, &prev_cwd);
	if (!new_cwd)
		return ;
	if (prev_cwd)
	{
		if (!update_node(mini->env_list, "OLDPWD", prev_cwd, 1))
			add_env_node(mini, "OLDPWD", prev_cwd, 1);
		free(prev_cwd);
	}
	if (!update_node(mini->env_list, "PWD", new_cwd, 1))
		add_env_node(mini, "PWD", new_cwd, 1);
		if (print_new)
		{
			ft_putstr(new_cwd, 1);
			ft_putstr("\n", 1);
		}
	free(new_cwd);
	if (path_allocated)
		free(path);
}
