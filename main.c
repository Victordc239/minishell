/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sofernan <sofernan@student.42madrid.es>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/30 15:25:53 by sofernan          #+#    #+#             */
/*   Updated: 2025/10/14 19:36:36 by sofernan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "mini.h"

char	**copy_env(char **env)
{
	int		len;
	char	**copy;
	int		i;

	len = 0;
	while (env && env[len])
		len++;
	copy = malloc(sizeof(char *) * (len + 1));
	if (!copy)
		return (NULL);
	i = 0;
	while (i < len)
	{
		copy[i] = ft_strdup(env[i]);
		i++;
	}
	copy[i] = NULL;
	return (copy);
}

void	update_status_sigint(t_minishell *mini)
{
	char	*status_str;

	if (g_status == 128 + SIGINT)
	{
		status_str = ft_itoa(g_status);
		if (status_str)
		{
			if (!update_env_var(mini->env_list, "?", status_str, 0))
				set_env_var(mini, "?", status_str, 0);
			free(status_str);
		}
	}
}

int	process_input_line(char *input, t_minishell *mini)
{
	if (!input)
	{
		(free_env_list(mini->env_list), free_tokenizer(mini->tokenizer));
		free_token_list(mini->t_list);
		if (mini->saved_stdin != -1)
			(close(mini->saved_stdin), mini->saved_stdin = -1);
		return (0);
	}
	update_status_sigint(mini);
	if (*input == '\0')
	{
		free(input);
		if (dup2(mini->saved_stdin, STDIN_FILENO) == -1)
			ft_putstr("dup2 failed\n", 2);
		if (mini->saved_stdin != -1)
			(close(mini->saved_stdin), mini->saved_stdin = -1);
		return (1);
	}
	(process_input(input, mini), free(input));
	if (dup2(mini->saved_stdin, STDIN_FILENO) == -1)
		ft_putstr("dup2 failed\n", 2);
	if (mini->saved_stdin != -1)
		(close(mini->saved_stdin), mini->saved_stdin = -1);
	return (1);
}

void	minishell_loop(t_minishell *mini)
{
	char	*prompt;
	char	*input;
	char	*cwd;

	while (1)
	{
		if (mini->saved_stdin != -1)
			(close(mini->saved_stdin), mini->saved_stdin = -1);
		mini->saved_stdin = dup(STDIN_FILENO);
		if (mini->saved_stdin == -1)
			exit_with_error("dup failed\n", 1, 2);
		cwd = getcwd(NULL, 0);
		if (!cwd)
			prompt = ft_strdup("Minishell> ");
		else
		{
			prompt = ft_strjoin(cwd, " Minishell> ");
			free(cwd);
		}
		input = readline(prompt);
		free(prompt);
		if (!process_input_line(input, mini))
			break ;
	}
}

volatile sig_atomic_t	g_status = 0;

int	main(int argc, char **argv, char **env)
{
	char		**my_env;
	t_minishell	mini;
	char		*status_str;

	(void)argv;
	if (argc != 1)
		exit_with_error("Too many arguments\n", 1, 1);
	mini = init_minishell();
	my_env = copy_env(env);
	mini.env_list = create_env_list(my_env, &mini);
	status_str = ft_itoa(g_status);
	set_env_var(&mini, "?", status_str, 0);
	free_str_array(my_env);
	free(status_str);
	signal(SIGINT, handle_signal);
	signal(SIGQUIT, SIG_IGN);
	minishell_loop(&mini);
	if (mini.saved_stdin != -1)
		(close(mini.saved_stdin), mini.saved_stdin = -1);
	return (0);
}
