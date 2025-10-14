/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_cmd.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sofernan <sofernan@student.42madrid.es>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/18 14:33:09 by sofernan          #+#    #+#             */
/*   Updated: 2025/10/14 19:20:32 by sofernan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "mini.h"

int	is_builtin_cmd(char *cmd)
{
	return (!ft_strcmp(cmd, "echo") || !ft_strcmp(cmd, "cd") || !ft_strcmp(cmd,
			"pwd") || !ft_strcmp(cmd, "export") || !ft_strcmp(cmd, "unset")
		|| !ft_strcmp(cmd, "env") || !ft_strcmp(cmd, "exit"));
}

char	*get_path_env(char **env)
{
	int	i;

	i = 0;
	if (!env)
		return (NULL);
	while (env[i])
	{
		if (ft_strncmp(env[i], "PATH=", 5) == 0)
			return (env[i] + 5);
		i++;
	}
	return (NULL);
}

void	try_exec_in_path(t_minishell *mini, char **env, char *cmd)
{
	char	*path_line;
	char	**possible_paths;

	if (!env || !*env)
		(exit_with_error("Missing environment\n", 1, 2),
			free_minishell(mini), free_str_array(env));
	path_line = get_path_env(env);
	if ((path_line && path_line[0] == '\0') || !path_line)
	{
		execve(cmd, mini->cmd_list->argv, env);
		perror(cmd);
		free_minishell(mini);
		free_str_array(env);
		exit(127);
	}
	possible_paths = ft_split(path_line, ':');
	if (!possible_paths)
		(exit_with_error("Error with possible path\n", 1, 2),
			free_minishell(mini), free_str_array(env));
	execute_command(mini, possible_paths, env);
}

void	ft_cmd(t_minishell *mini)
{
	char		**env;
	char		*cmd;
	struct stat	st;

	env = make_env_array(mini->env_list);
	cmd = mini->cmd_list->argv[0];
	if (is_builtin_cmd(cmd))
	{
		if (cmd && ft_strcmp(cmd, "exit") == 0)
			(free_str_array(env), ft_exit(mini));
		(exec_builtin_child(mini->cmd_list->argv, &env, mini),
			free_str_array(env), free_minishell(mini), exit(0));
	}
	if (cmd && ft_strchr(cmd, '/'))
	{
		if (stat(cmd, &st) == 0 && S_ISDIR(st.st_mode))
			(ft_putstr(cmd, 2), ft_putstr(": Is a directory\n", 2),
				free_minishell(mini), free_str_array(env), exit(126));
		if (access(cmd, F_OK) == -1)
			(perror(cmd), free_minishell(mini), free_str_array(env), exit(127));
		if (access(cmd, X_OK) == -1)
			(perror(cmd), free_minishell(mini), free_str_array(env), exit(126));
		(execve(cmd, mini->cmd_list->argv, env), check_errno(errno, mini));
	}
	try_exec_in_path(mini, env, cmd);
}
