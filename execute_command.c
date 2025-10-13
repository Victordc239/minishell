/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_command.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sofernan <sofernan@student.42madrid.es>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/18 14:41:03 by sofernan          #+#    #+#             */
/*   Updated: 2025/10/13 15:08:01 by sofernan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "mini.h"

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

void	check_errno(int err, t_minishell *mini)
{
	const char	*cmd;

	cmd = NULL;
	if (mini && mini->cmd_list && mini->cmd_list->argv
		&& mini->cmd_list->argv[0])
		cmd = mini->cmd_list->argv[0];
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

int	try_exec_path(char *dir, char *cmd, t_minishell *mini, char **env)
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
		execve(path, mini->cmd_list->argv, env);
		saved_errno = errno;
		free(path);
		if (saved_errno == EISDIR)
			return (0);
		check_errno(saved_errno, mini);
		return (0);
	}
	return (free(path), 0);
}

void	search_exec_cmds(char **paths, char *cmd, t_minishell *mini, char **env)
{
	int	i;
	int	ret;

	if (!paths)
		return ;
	i = 0;
	while (paths[i])
	{
		ret = try_exec_path(paths[i], cmd, mini, env);
		if (ret == -1)
		{
			ft_freedoom(mini->cmd_list->argv);
			ft_freedoom(paths);
			exit(0);
		}
		i++;
	}
}

void	execute_command(t_minishell *mini, char **paths, char **env)
{
	struct stat	st;
	char		*cmd;

	if (!env || !*env)
		free_struct(mini->pipex_data, "Missing environment\n", 1, 2);
	cmd = mini->cmd_list->argv[0];
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
		execve(cmd, mini->cmd_list->argv, env);
		check_errno(errno, mini);
	}
	search_exec_cmds(paths, cmd, mini, env);
	mini->paths_execve = paths;
	mini->envir_execve = env;
	(check_errno(ENOENT, mini), exit(127));
}
