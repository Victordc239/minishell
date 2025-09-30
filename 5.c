/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   5.c                                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sofernan <sofernan@student.42madrid.es>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/18 14:41:03 by sofernan          #+#    #+#             */
/*   Updated: 2025/09/30 17:14:55 by sofernan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "mini.h"

void	check_errno(int err, t_minishell *mini)
{
	const char	*cmd;

	cmd = NULL;
	if (mini && mini->command_list && mini->command_list->argv
		&& mini->command_list->argv[0])
		cmd = mini->command_list->argv[0];
	if (cmd)
		(ft_putstr((char *)cmd, 2), ft_putstr(": ", 2));
	if (err == EISDIR)
		(ft_putstr("Is a directory\n", 2), free_minishell(mini), exit(126));
	else if (err == EACCES)
	{
		if (cmd && !ft_strchr(cmd, '/'))
			ft_putstr("command not found\n", 2);
		else
			ft_putstr("Permission denied\n", 2);
		free_minishell(mini);
		exit(126);
	}
	else if (err == ENOENT)
		(ft_putstr("command not found\n", 2), free_minishell(mini), exit(127));
	else
		(ft_putstr(": ", 2), ft_putstr(strerror(err), 2),
			ft_putstr("\n", 2), free_minishell(mini), exit(1));
}

char	*create_path(char *possible_path, char *command)
{
	char	*path;
	char	*temp;

	if (ft_strchr(command, '/'))
		return (ft_strdup(command));
	if (possible_path && possible_path[0] == '\0')
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

int	try_exec_one_path(char *dir, char *cmd, t_minishell *mini, char **envir)
{
	char		*path;
	struct stat	st;
	int			saved_errno;

	path = create_path(dir, cmd);
	if (!path)
		return (-1);
	if (stat(path, &st) == 0 && S_ISDIR(st.st_mode))
		return (free(path), 0);
	if (stat(path, &st) == 0 && S_ISREG(st.st_mode)
		&& access(path, X_OK) == 0)
	{
		execve(path, mini->command_list->argv, envir);
		saved_errno = errno;
		free(path);
		if (saved_errno == EISDIR)
			return (0);
		check_errno(saved_errno, mini);
		return (0);
	}
	return (free(path), 0);
}

void	exec_paths(char **paths, char *cmd, t_minishell *mini, char **envir)
{
	int	i;
	int	ret;

	if (!paths)
		return ;
	i = 0;
	while (paths[i])
	{
		ret = try_exec_one_path(paths[i], cmd, mini, envir);
		if (ret == -1)
			free_and_exit(mini->command_list->argv, paths, 0);
		i++;
	}
}

void	execute_command(t_minishell *mini, char **paths, char **envir)
{
	struct stat	st;
	char		*cmd;

	if (!envir || !*envir)
		free_struct(mini->pipex_data, "Missing environment\n", 1, 2);
	cmd = mini->command_list->argv[0];
	if (ft_strchr(cmd, '/'))
	{
		if (stat(cmd, &st) == 0 && S_ISDIR(st.st_mode))
		{
			ft_putstr(cmd, 2);
			ft_putstr(": ", 2);
			ft_putstr("Is a directory\n", 2);
			free_minishell(mini);
			exit(126);
		}
		execve(cmd, mini->command_list->argv, envir);
		check_errno(errno, mini);
	}
	exec_paths(paths, cmd, mini, envir);
	mini->paths_execve = paths;
	mini->envir_execve = envir;
	(check_errno(ENOENT, mini), exit(127));
}
