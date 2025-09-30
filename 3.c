/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   3.c                                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sofernan <sofernan@student.42madrid.es>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/18 14:36:45 by sofernan          #+#    #+#             */
/*   Updated: 2025/09/30 14:48:03 by sofernan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "mini.h"

int	ft_exit(t_minishell *mini)
{
	unsigned char	code;

	if (!mini->command_list->argv[1])
	{
		free_minishell(mini);
		exit((unsigned char)g_status);
	}
	if (mini->command_list->argv[2])
	{
		ft_putstr("exit: too many arguments\n", 2);
		g_status = 1;
		return (1);
	}
	if (!parse_exit_code(mini->command_list->argv[1], &code, 0, 0))
	{
		ft_putstr("exit: ", 2);
		ft_putstr(mini->command_list->argv[1], 2);
		ft_putstr(": numeric argument required\n", 2);
		free_minishell(mini);
		exit(2);
	}
	free_minishell(mini);
	exit((int)code);
}

char	*get_cd_path(t_minishell *mini, int *free_path, int *print_new)
{
	char	*path;

	if (mini->command_list->argv[1] && mini->command_list->argv[2])
		return (ft_putstr("cd: too many arguments\n", 2), g_status = 1, NULL);
	if (!mini->command_list->argv[1])
	{
		path = get_env_value("HOME", mini->env_list);
		if (!path)
			return (ft_putstr("cd: HOME not set\n", 2), g_status = 1, NULL);
		return (*free_path = 1, path);
	}
	if (ft_strcmp(mini->command_list->argv[1], "-") == 0)
	{
		path = get_env_value("OLDPWD", mini->env_list);
		if (!path)
			return (ft_putstr("cd: OLDPWD not set\n", 2), g_status = 1, NULL);
		return (*free_path = 1, *print_new = 1, path);
	}
	return (mini->command_list->argv[1]);
}

char	*builtin_cd(t_minishell *mini, char *path, int free_path, char **prev_cwd)
{
	char	*new_cwd;

	*prev_cwd = getcwd(NULL, 0);
	if (!*prev_cwd)
		*prev_cwd = get_env_value("PWD", mini->env_list);
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
	new_cwd = builtin_cd(mini, path, free_path, &prev_cwd);
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
		(ft_putstr(new_cwd, 1), ft_putstr("\n", 1));
	free(new_cwd);
	if (free_path)
		free(path);
}

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

/*void	execute_buitin(t_minishell *minishell)
{
	if (!ft_strcmp(minishell->command_list->argv[0], "exit"))
		ft_exit(minishell);
	else if (!ft_strcmp(minishell->command_list->argv[0], "cd"))
		ft_cd(minishell);
	else if (!ft_strcmp(minishell->command_list->argv[0], "unset"))
		ft_unset(minishell);
	else if (!ft_strcmp(minishell->command_list->argv[0], "export"))
		ft_export(minishell);
}*/

void	execute_buitin(t_minishell *minishell)
{
	if (!ft_strcmp(minishell->command_list->argv[0], "exit"))
		ft_exit(minishell);
	else if (!ft_strcmp(minishell->command_list->argv[0], "cd"))
		ft_cd(minishell, 0, 0);
	else if (!ft_strcmp(minishell->command_list->argv[0], "unset"))
		ft_unset(minishell);
	else if (!ft_strcmp(minishell->command_list->argv[0], "export"))
		ft_export(minishell);
}
