/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vdiez-cu <vdiez-cu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/30 15:25:53 by sofernan          #+#    #+#             */
/*   Updated: 2025/10/08 14:04:09 by vdiez-cu         ###   ########.fr       */
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

void	do_signal(void)
{
	signal(SIGINT, sighandler);
	signal(SIGQUIT, SIG_IGN);
}

/*int	handle_input_cycle(char *input, t_minishell *mini, int saved_stdin)
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
}*/

int handle_input_cycle(char *input, t_minishell *mini)
{
    char *status_str;

    if (!input)
    {
        free_env_list(mini->env_list);
        free_tokenizer(mini->tokenizer);
        free_t_list(mini->t_list);
        if (mini->saved_stdin != -1)
        {
            close(mini->saved_stdin);
            mini->saved_stdin = -1;
        }
        return (0);
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
    {
        free(input);
        if (dup2(mini->saved_stdin, STDIN_FILENO) == -1)
            ft_putstr("dup2 failed\n", 2);
        if (mini->saved_stdin != -1)
        {
            close(mini->saved_stdin);
            mini->saved_stdin = -1;
        }
        return (1);
    }
    process_input(input, mini);
    free(input);
    if (dup2(mini->saved_stdin, STDIN_FILENO) == -1)
        ft_putstr("dup2 failed\n", 2);
    if (mini->saved_stdin != -1)
    {
        close(mini->saved_stdin);
        mini->saved_stdin = -1;
    }
    return (1);
}

/*void	mini_loop(t_minishell *mini)
{
	int		saved_stdin;
	char	*prompt;
	char	*input;
	char	*cwd;

	while (1)
	{
		saved_stdin = dup(STDIN_FILENO);
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
		if (!handle_input_cycle(input, mini, saved_stdin))
			break ;
	}
}*/

void mini_loop(t_minishell *mini)
{
    char *prompt;
    char *input;
    char *cwd;

    while (1)
    {
        mini->saved_stdin = dup(STDIN_FILENO);
	  if (mini->saved_stdin != -1)
		{
		int flags;

		flags = fcntl(mini->saved_stdin, F_GETFD);
		if (flags != -1)
			fcntl(mini->saved_stdin, F_SETFD, flags | FD_CLOEXEC);
		}
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
        if (!handle_input_cycle(input, mini))
            break ;
    }
    if (mini->saved_stdin != -1)
    {
        close(mini->saved_stdin);
        mini->saved_stdin = -1;
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
	add_env_node(&mini, "?", status_str, 0);
	ft_freedoom(my_env);
	free(status_str);
	do_signal();
	mini_loop(&mini);
	return (0);
}
