/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   41.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sofernan <sofernan@student.42madrid.es>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/30 15:25:53 by sofernan          #+#    #+#             */
/*   Updated: 2025/09/30 17:04:19 by sofernan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "mini.h"

t_minishell	init_minishell(void)
{
	t_minishell	minishell;

	ft_bzero(&minishell, sizeof(t_minishell));
	minishell.env_list = NULL;
	minishell.t_list = NULL;
	minishell.command_list = NULL;
	minishell.pipex_data = NULL;
	minishell.head = NULL;
	minishell.tmp = NULL;
	minishell.tokenizer = NULL;
	minishell.curr_token = NULL;
	minishell.new_token = NULL;
	minishell.new_node = NULL;
	minishell.current = NULL;
	return (minishell);
}

int	env_len(char **env)
{
	int	i;

	i = 0;
	while (env && env[i])
		i++;
	return (i);
}

char	**copy_env(char **env)
{
	int		len;
	char	**copy;
	int		i;

	i = 0;
	len = env_len(env);
	copy = malloc(sizeof(char *) * (len + 1));
	if (!copy)
		return (NULL);
	while (i < len)
	{
		copy[i] = ft_strdup(env[i]);
		i++;
	}
	copy[i] = NULL;
	return (copy);
}

t_env	*create_env_list(char **envp, t_minishell *mini)
{
	int		i;
	char	*equal;
	char	*name;
	char	*value;

	mini->env_list = NULL;
	i = 0;
	while (envp[i])
	{
		equal = ft_strchr(envp[i], '=');
		if (equal)
		{
			name = ft_substr(envp[i], 0, equal - envp[i]);
			value = ft_strdup(equal + 1);
			add_env_node(mini, name, value, 1);
			free(name);
			free(value);
		}
		i++;
	}
	return (mini->env_list);
}

void	do_signal(void)
{
	signal(SIGINT, sighandler);
	signal(SIGQUIT, SIG_IGN);
}

char	*get_prompt(void)
{
	char	*cwd;
	char	*pront;

	cwd = getcwd(NULL, 0);
	if (!cwd)
		pront = ft_strdup("Minishell> ");
	else
	{
		pront = ft_strjoin(cwd, " Minishell> ");
		free(cwd);
	}
	return (pront);
}

int	handle_input_cycle(char *input, t_minishell *mini, int saved_stdin)
{
	char	*status_str;

	if (!input)
	{
		free_env_list(mini->env_list);
		free_tokenizer(mini->tokenizer);
		free_t_list(mini->t_list);
		return (close(saved_stdin), 0);
	}
	if (g_status == 128 + SIGINT)
	{
		status_str = ft_itoa(g_status);
		if (status_str)
		{
			if (!update_node(mini->env_list, "?", status_str, 0))
				add_env_node(mini, "?", status_str, 0);
			free(status_str);
		}
	}
	if (*input == '\0')
		return (free(input), dup2(saved_stdin, STDIN_FILENO),
			close(saved_stdin), 1);
	(process_input(input, mini), free(input));
	(dup2(saved_stdin, STDIN_FILENO), close(saved_stdin));
	return (1);
}

void	mini_loop(t_minishell *mini)
{
	int		saved_stdin;
	char	*prompt;
	char	*input;

	while (1)
	{
		saved_stdin = dup(STDIN_FILENO);
		prompt = get_prompt();
		input = readline(prompt);
		free(prompt);
		if (!handle_input_cycle(input, mini, saved_stdin))
			break ;
	}
}

volatile sig_atomic_t	g_status = 0;

int	main(int argc, char **argv, char **env)
{
	char		**my_env;
	t_minishell	minishell;
	char		*status_str;

	(void)argv;
	if (argc != 1)
		exit_with_error("Too many arguments\n", 1, 1);
	minishell = init_minishell();
	my_env = copy_env(env);
	minishell.env_list = create_env_list(my_env, &minishell);
	status_str = ft_itoa(g_status);
	add_env_node(&minishell, "?", status_str, 0);
	ft_freedoom(my_env);
	free(status_str);
	do_signal();
	mini_loop(&minishell);
	return (0);
}
