/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   41.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sofernan <sofernan@student.42madrid.es>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/30 15:25:53 by sofernan          #+#    #+#             */
/*   Updated: 2025/09/23 17:34:04 by sofernan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "mini.h"

void	exec_paths(char **paths, char *cmd, t_minishell *mini, char **envir)
{
	int			i;
	char		*path;
	struct stat	st;

	if (!paths)
		return ;
	i = 0;
	while (paths[i])
	{
		path = create_path(paths[i], cmd);
		if (!path)
			free_and_exit(mini->command_list->argv, paths, 0);
		if (stat(path, &st) == 0 && !S_ISDIR(st.st_mode)
			&& S_ISREG(st.st_mode) && access(path, X_OK) == 0)
		{
			execve(path, mini->command_list->argv, envir);
			free(path);
			if (errno != EISDIR)
				check_errno(errno, mini);
		}
		free(path);
		i++;
	}
}

//

 void	handle_builtin_or_path(t_minishell *mini, char **envir)
{
	char		*cmd;
	struct stat	st;

	cmd = mini->command_list->argv[0];
	if (is_builtin_str(cmd))
	{
		if (cmd && ft_strcmp(cmd, "exit") == 0)
			(ft_freedoom(envir), ft_exit(mini));
		execute_buitin_args(mini->command_list->argv, &envir, mini);
		(ft_freedoom(envir), free_minishell(mini), exit(0));
	}
	if (cmd && ft_strchr(cmd, '/'))
	{
		if (stat(cmd, &st) == 0 && S_ISDIR(st.st_mode))
			(ft_putstr(cmd, 2), ft_putstr(": Is a directory\n", 2),
				free_minishell(mini), ft_freedoom(envir), exit(126));
		if (access(cmd, F_OK) == -1)
			(perror(cmd), free_minishell(mini), ft_freedoom(envir), exit(127));
		if (access(cmd, X_OK) == -1)
			(perror(cmd), free_minishell(mini), ft_freedoom(envir), exit(126));
		execve(cmd, mini->command_list->argv, envir);
		check_errno(errno, mini);
	}
}

void	ft_cmd(t_minishell *mini)
{
	char	**possible_paths;
	char	*path_line;
	char	**envir;
	char	*cmd;

	envir = env_to_array(mini->env_list);
	handle_builtin_or_path(mini, envir);
	cmd = mini->command_list->argv[0];
	if (!envir || !*envir)
		(exit_with_error("Missing environment\n", 1, 2),
			free_minishell(mini), ft_freedoom(envir));
	path_line = find_execpath(envir);
	if (path_line && path_line[0] == '\0')
		(execve(cmd, mini->command_list->argv, envir), perror(cmd),
			free_minishell(mini), ft_freedoom(envir), exit(127));
	if (!path_line)
		(execve(cmd, mini->command_list->argv, envir), perror(cmd),
			free_minishell(mini), ft_freedoom(envir), exit(127));
	possible_paths = ft_split(path_line, ':');
	if (!possible_paths)
		(exit_with_error("Error with possible path\n", 1, 2),
			free_minishell(mini), ft_freedoom(envir));
	execute_command(mini, possible_paths, envir);
}
