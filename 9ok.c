/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   9ok.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sofernan <sofernan@student.42madrid.es>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/18 14:41:03 by sofernan          #+#    #+#             */
/*   Updated: 2025/09/18 17:09:03 by sofernan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "mini.h"

void	check_errno(int err, t_minishell *mini)
{
	if (err == EISDIR)
	{
		ft_putstr(": Is a directory\n", 2);
		(free_minishell(mini), exit(126));
	}
	else if (err == EACCES)
	{
		if (mini->command_list->argv[0]
			&& !ft_strchr(mini->command_list->argv[0], '/'))
			ft_putstr(": command not found\n", 2);
		else
			ft_putstr(": Permission denied\n", 2);
		(free_minishell(mini), exit(126));
	}
	else if (err == ENOENT)
	{
		ft_putstr(": command not found\n", 2);
		(free_minishell(mini), exit(127));
	}
	else
	{
		(ft_putstr(": ", 2), ft_putstr(strerror(err), 2),
			ft_putstr("\n", 2), free_minishell(mini), exit(1));
	}
}

char	*create_path(char *possible_path, char *command)
{
	char	*path;
	char	*temp;

	if (ft_strchr(command, '/'))
		return (ft_strdup(command));
	temp = ft_strjoin(possible_path, "/");
	if (!temp)
		return (NULL);
	path = ft_strjoin(temp, command);
	free(temp);
	return (path);
}

void	free_and_exit(char **args, char **paths, int exit_code)
{
	ft_freedoom(args);
	ft_freedoom(paths);
	exit(exit_code);
}

 void	exec_paths(char **paths, char *cmd, t_minishell *mini, char **envir)
{
	int		i;
	char	*path;

	i = 0;
	while (paths[i])
	{
		path = create_path(paths[i], cmd);
		if (!path)
			free_and_exit(mini->command_list->argv, paths, 0);
		if (access(path, F_OK) == 0 && access(path, X_OK) == 0)
		{
			execve(path, mini->command_list->argv, envir);
			free(path);
			check_errno(errno, mini);
		}
		free(path);
		i++;
	}
}

void	execute_command(t_minishell *mini, char **paths, char **envir)
{
	char	*cmd;

	if (!envir || !*envir)
		free_struct(mini->pipex_data, "Missing environment\n", 1, 2);
	cmd = mini->command_list->argv[0];
	if (ft_strchr(cmd, '/'))
	{
		if (access(cmd, F_OK) == -1)
			(perror(cmd), free_minishell(mini), exit(127));
		if (access(cmd, X_OK) == -1)
			(perror(cmd), free_minishell(mini), exit(126));
		execve(cmd, mini->command_list->argv, envir);
		check_errno(errno, mini);
	}
	exec_paths(paths, cmd, mini, envir);
	mini->paths_execve = paths;
	mini->envir_execve = envir;
	(check_errno(ENOENT, mini), exit(127));
}
