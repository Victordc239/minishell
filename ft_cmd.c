/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_cmd.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sofernan <sofernan@student.42madrid.es>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/18 14:33:09 by sofernan          #+#    #+#             */
/*   Updated: 2025/10/03 15:29:37 by sofernan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "mini.h"

int	is_builtin_str(char *str)
{
	return (!ft_strcmp(str, "echo") || !ft_strcmp(str, "cd") || !ft_strcmp(str,
			"pwd") || !ft_strcmp(str, "export") || !ft_strcmp(str, "unset")
		|| !ft_strcmp(str, "env") || !ft_strcmp(str, "exit"));
}

char	*find_execpath(char **envir)
{
	int	i;

	i = 0;
	if (!envir)
		return (NULL);
	while (envir[i])
	{
		if (ft_strncmp(envir[i], "PATH=", 5) == 0)
			return (envir[i] + 5);
		i++;
	}
	return (NULL);
}

static void	ft_cmd_rest(t_minishell *mini, char **envir, char *cmd)
{
	char	*path_line;
	char	**possible_paths;

	if (!envir || !*envir)
		(exit_with_error("Missing environment\n", 1, 2), free_minishell(mini),
			ft_freedoom(envir));
	path_line = find_execpath(envir);
	if ((path_line && path_line[0] == '\0') || !path_line)
	{
		execve(cmd, mini->cmd_list->argv, envir);
		perror(cmd);
		free_minishell(mini);
		ft_freedoom(envir);
		exit(127);
	}
	possible_paths = ft_split(path_line, ':');
	if (!possible_paths)
		(exit_with_error("Error with possible path\n", 1, 2),
			free_minishell(mini), ft_freedoom(envir));
	execute_command(mini, possible_paths, envir);
}

void	ft_cmd(t_minishell *mini)
{
	char		**envir;
	char		*cmd;
	struct stat	st;

	envir = env_to_array(mini->env_list);
	cmd = mini->cmd_list->argv[0];
	if (is_builtin_str(cmd))
	{
		if (cmd && ft_strcmp(cmd, "exit") == 0)
			(ft_freedoom(envir), ft_exit(mini));
		(execute_buitin_args(mini->cmd_list->argv, &envir, mini),
			ft_freedoom(envir), free_minishell(mini), exit(0));
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
		(execve(cmd, mini->cmd_list->argv, envir), check_errno(errno, mini));
	}
	ft_cmd_rest(mini, envir, cmd);
}
