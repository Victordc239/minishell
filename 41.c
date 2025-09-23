/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   41.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sofernan <sofernan@student.42madrid.es>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/30 15:25:53 by sofernan          #+#    #+#             */
/*   Updated: 2025/09/23 17:29:33 by sofernan         ###   ########.fr       */
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
