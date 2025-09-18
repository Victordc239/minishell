/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   todo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vdiez-cu <vdiez-cu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/30 15:25:53 by sofernan          #+#    #+#             */
/*   Updated: 2025/09/18 17:15:03 by vdiez-cu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "mini.h"

static int	append_ptr(char ***arr, int new_size, int copy_count, char *value)
{
	char	**newarr;
	int		i;

	newarr = malloc(sizeof(char *) * new_size);
	if (!newarr)
		return (0);
	i = 0;
	if (*arr)
	{
		while (i < copy_count)
		{
			newarr[i] = (*arr)[i];
			i++;
		}
		free(*arr);
	}
	newarr[copy_count] = value;
	*arr = newarr;
	return (1);
}

static int	pattern_has_slash(const char *s)
{
	while (s && *s)
	{
		if (*s == '/')
			return (1);
		s++;
	}
	return (0);
}

static void	split_path(const char *pattern, char **dir_out, char **base_out)
{
	int	i;

	*dir_out = NULL;
	*base_out = NULL;
	if (!pattern)
		return ;
	i = (int)ft_strlen(pattern) - 1;
	while (i >= 0 && pattern[i] != '/')
		i--;
	if (i < 0)
	{
		*dir_out = ft_strdup(".");
		*base_out = ft_strdup(pattern);
		return ;
	}
	if (i == 0)
		*dir_out = ft_strdup("/");
	else
		*dir_out = ft_substr(pattern, 0, i);
	*base_out = ft_strdup(pattern + i + 1);
}

static const char	*init_class(const char *p, char c, int *neg, int *matched)
{
	*neg = 0;
	*matched = 0;
	if (!p || *p != '[')
		return (NULL);
	p++;
	if (*p == '!' || *p == '^')
	{
		*neg = 1;
		p++;
	}
	if (*p == ']')
	{
		if (c == ']')
			*matched = 1;
		p++;
	}
	return (p);
}

static const char	*process_class_content(const char *p, char c, int *matched)
{
	char	a;
	char	b;
	char	tmp;

	if (!p || *p == '\0' || *p == ']')
		return (p);
	if (p[0] && p[1] == '-' && p[2] && p[2] != ']')
	{
		a = p[0];
		b = p[2];
		if (a > b)
		{
			tmp = a;
			a = b;
			b = tmp;
		}
		if ((unsigned char)c >= (unsigned char)a
			&& (unsigned char)c <= (unsigned char)b)
			*matched = 1;
		return (process_class_content(p + 3, c, matched));
	}
	if (c == *p)
		*matched = 1;
	return (process_class_content(p + 1, c, matched));
}

static int	match_class(const char **pp, char c)
{
	const char	*p;
	int			negate;
	int			matched;

	p = init_class(*pp, c, &negate, &matched);
	if (!p)
		return (-1);
	p = process_class_content(p, c, &matched);
	if (*p != ']')
		return (-1);
	p++;
	*pp = p;
	if (negate)
		return (!matched);
	return (matched);
}

static int	match_glob_star(const char *p, const char *str)
{
	if (*p == '\0')
		return (1);
	if (*str == '\0')
		return (0);
	if (match_glob(p, str))
		return (1);
	return (match_glob_star(p, str + 1));
}

static int	match_glob(const char *p, const char *str)
{
	int	rc;

	if (*p == '\0')
		return (*str == '\0');
	if (*p == '*')
	{
		while (*p == '*')
			p++;
		if (*p == '\0')
			return (1);
		return (match_glob_star(p, str));
	}
	if (*p == '[')
	{
		if (*str == '\0')
			return (0);
		rc = match_class(&p, *str);
		if (rc == -1 || rc == 0)
			return (0);
		return (match_glob(p, str + 1));
	}
	if (*str == '\0' || *p != *str)
		return (0);
	return (match_glob(p + 1, str + 1));
}

static int	ensure_capacity(char ***arr, size_t *count, size_t *cap)
{
	size_t		newcap;
	char		**newarr;
	size_t		i;

	if (*cap == 0)
	{
		*cap = 8;
		*arr = malloc(sizeof(char *) * (*cap));
		if (!*arr)
			return (0);
		return (1);
	}
	if (*count + 1 > *cap)
	{
		newcap = (*cap * 2);
		newarr = malloc(sizeof(char *) * newcap);
		if (!newarr)
			return (0);
		i = 0;
		while (i++ < *count)
			newarr[i - 1] = (*arr)[i - 1];
		(free(*arr), *arr = newarr);
		*cap = newcap;
	}
	return (1);
}

static int	insert_sorted(char ***arr, size_t *count, size_t *cap, char *s)
{
	size_t	pos;
	size_t	j;

	pos = 0;
	if (!ensure_capacity(arr, count, cap))
		return (0);
	while (pos < *count && ft_strcmp((*arr)[pos], s) < 0)
		pos++;
	j = *count;
	while (j > pos)
	{
		(*arr)[j] = (*arr)[j - 1];
		j--;
	}
	(*arr)[pos] = s;
	(*count)++;
	return (1);
}

///////////////////////////////////

void	free_env_list(t_env *env)
{
	t_env	*tmp;

	while (env)
	{
		tmp = env->next;
		if (env->name)
			free(env->name);
		if (env->value)
			free(env->value);
		free(env);
		env = tmp;
	}
}

void	free_t_list(t_token *list)
{
	t_token	*tmp;

	while (list)
	{
		tmp = list->next;
		if (list->value)
			free(list->value);
		free(list);
		list = tmp;
	}
}

//////////////

void	free_redir_list(t_redir *redir)
{
	t_redir	*tmp;

	while (redir)
	{
		tmp = redir->next;
		if (redir->filename)
			free(redir->filename);
		free(redir);
		redir = tmp;
	}
}

void	free_command_list(t_command *cmd)
{
	t_command	*tmp;
	int			i;

	while (cmd)
	{
		tmp = cmd->next;
		if (cmd->redirs)
			free_redir_list(cmd->redirs);
		if (cmd->argv)
		{
			i = 0;
			while (cmd->argv[i])
				(free(cmd->argv[i]), i++);
			free(cmd->argv);
		}
		if (cmd->infile)
			free(cmd->infile);
		if (cmd->outfile)
			free(cmd->outfile);
		if (cmd->heredoc_file)
			free(cmd->heredoc_file);
		free(cmd);
		cmd = tmp;
	}
}

void	free_pipex_data(t_pipex *data)
{
	if (!data)
		return ;
	if (data->pid)
		free(data->pid);
	if (data->commands)
		free_command_list(data->commands);
	free(data);
}

////////////////////////

void	free_tokenizer(t_tokenizer *tokenizer)
{
	if (!tokenizer)
		return ;
	free(tokenizer);
}

void	ft_freedoom(char **str)
{
	int	i;

	if (!str)
		return ;
	i = 0;
	while (str[i])
	{
		free(str[i]);
		str[i] = NULL;
		i++;
	}
	free(str);
}

////////////////////

void	free_minishell(t_minishell *shell)
{
	if (!shell)
		return ;
	if (shell->env_list)
		(free_env_list(shell->env_list), shell->env_list = NULL);
	if (shell->t_list)
		(free_t_list(shell->t_list), shell->t_list = NULL);
	if (shell->pipex_data && shell->command_list)
		(free_pipex_data(shell->pipex_data), shell->pipex_data = NULL);
	if (shell->tokenizer)
	{
		free_tokenizer(shell->tokenizer);
		shell->tokenizer = NULL;
	}
	ft_freedoom(shell->envir_execve);
	ft_freedoom(shell->paths_execve);
	shell->curr_token = NULL;
	shell->new_token = NULL;
	shell->new_node = NULL;
	shell->current = NULL;
	shell->head = NULL;
	shell->tmp = NULL;
	shell->curr = NULL;
}

////////////////////////////////

int	is_numeric(char const *str)
{
	int	i;

	i = 0;
	if (!str)
		return (0);
	if (str[i] == '-' || str[i] == '+')
		i++;
	while (str[i])
	{
		if (!ft_isdigit(str[i]))
			return (0);
		i++;
	}
	return (1);
}

int	ft_exit(t_minishell *mini)
{
	int	code;

	if (!mini->command_list->argv[1])
	{
		free_minishell(mini);
		exit(g_status);
	}
	if (mini->command_list->argv[2])
	{
		ft_putstr("exit: too many arguments\n", 2);
		g_status = 1;
		return (1);
	}
	if (!is_numeric(mini->command_list->argv[1]))
	{
		ft_putstr("exit: ", 2);
		ft_putstr(mini->command_list->argv[1], 2);
		ft_putstr(": numeric argument required\n", 2);
		free_minishell(mini);
		exit(2);
	}
	code = ft_atoi(mini->command_list->argv[1]);
	free_minishell(mini);
	exit(code % 256);
}

//////////////////////////

char	*get_env_value(char *name, t_env *env)
{
	while (env)
	{
		if (!ft_strcmp(env->name, name))
		{
			if (env->value)
				return (ft_strdup(env->value));
			else
				return (NULL);
		}
		env = env->next;
	}
	return (NULL);
}

void	ft_cd(t_minishell *mini)
{
	char	*path;

	if (mini->command_list->argv[1] && mini->command_list->argv[2])
	{
		ft_putstr("cd: too many arguments\n", 2);
		g_status = 1;
		return ;
	}
	if (!mini->command_list->argv[1])
	{
		path = get_env_value("HOME", mini->env_list);
		if (!path)
		{
			(ft_putstr("cd: HOME not set\n", 2), g_status = 1);
			return ;
		}
	}
	else
		path = mini->command_list->argv[1];
	if (chdir(path) == -1)
		(perror("cd"), g_status = 1);
	else
		g_status = 0;
	if (!mini->command_list->argv[1])
		free(path);
}

//////////////////////

void	remove_env_var(char const *name, t_minishell *mini)
{
	t_env	*curr;
	t_env	*prev;

	curr = mini->env_list;
	prev = NULL;
	while (curr)
	{
		if (!ft_strcmp(curr->name, name))
		{
			if (prev)
				prev->next = curr->next;
			else
				mini->env_list = curr->next;
			free(curr->name);
			free(curr->value);
			free(curr);
			return ;
		}
		prev = curr;
		curr = curr->next;
	}
}

void	ft_unset(t_minishell *mini)
{
	int		i;

	i = 1;
	while (mini->command_list->argv[i])
	{
		remove_env_var(mini->command_list->argv[i], mini);
		i++;
	}
}

///////////////////////

int	count_exported(t_minishell *mini)
{
	int		count;
	t_env	*tmp;

	tmp = mini->env_list;
	count = 0;
	while (mini->env_list)
	{
		if (mini->env_list->exported)
			count++;
		mini->env_list = mini->env_list->next;
	}
	mini->env_list = tmp;
	return (count);
}

void	sort_env_array(t_env **arr, int count)
{
	int		i;
	int		j;
	t_env	*tmp;

	i = 0;
	while (i < count - 1)
	{
		j = i + 1;
		while (j < count)
		{
			if (ft_strcmp(arr[i]->name, arr[j]->name) > 0)
			{
				tmp = arr[i];
				arr[i] = arr[j];
				arr[j] = tmp;
			}
			j++;
		}
		i++;
	}
}

void	print_env_array(t_env **arr, int count)
{
	int	i;

	i = 0;
	while (i < count)
	{
		printf("declare -x %s", arr[i]->name);
		if (arr[i]->value)
			printf("=\"%s\"", arr[i]->value);
		printf("\n");
		i++;
	}
}

void	print_sorted_env(t_minishell *mini)
{
	int		count;
	int		i;
	t_env	**arr;
	t_env	*tmp;

	count = count_exported(mini);
	arr = malloc(sizeof(t_env *) * count);
	if (!arr)
		return ;
	i = 0;
	tmp = mini->env_list;
	while (tmp)
	{
		if (tmp->exported)
			arr[i++] = tmp;
		tmp = tmp->next;
	}
	sort_env_array(arr, count);
	print_env_array(arr, count);
	free(arr);
}

/////////////

int	is_valid_identifier(char const *str)
{
	int	i;

	i = 1;
	if (!str || !(ft_isalpha(str[0]) || str[0] == '_'))
		return (0);
	while (str[i] && str[i] != '=')
	{
		if (!(ft_isalnum(str[i]) || str[i] == '_'))
			return (0);
		i++;
	}
	return (1);
}

//////////////////

t_env	*find_env(t_env *env_list, char const *name)
{
	while (env_list)
	{
		if (!ft_strcmp(env_list->name, name))
			return (env_list);
		env_list = env_list->next;
	}
	return (NULL);
}

//////////////////

void	update_node_value(t_env *tmp, char *value, int exported)
{
	if (tmp->value)
		free(tmp->value);
	if (value)
		tmp->value = ft_strdup(value);
	else
		tmp->value = NULL;
	tmp->exported = exported;
}

int	update_node(t_env *tmp, char *name, char *value, int exported)
{
	while (tmp)
	{
		if (!ft_strcmp(tmp->name, name))
		{
			update_node_value(tmp, value, exported);
			return (1);
		}
		tmp = tmp->next;
	}
	return (0);
}

t_env	*create_new_node(char *name, char *value, int exported)
{
	t_env	*new;

	new = malloc(sizeof(t_env));
	if (!new)
		return (NULL);
	new->name = ft_strdup(name);
	if (!new->name)
	{
		free(new);
		return (NULL);
	}
	if (value)
	{
		new->value = ft_strdup(value);
	}
	else
	{
		new->value = NULL;
	}
	new->exported = exported;
	new->signal = 0;
	new->next = NULL;
	return (new);
}

void	append_node_to_list(t_minishell *mini, t_env *new)
{
	t_env	*tmp;

	if (!mini->env_list)
	{
		mini->env_list = new;
	}
	else
	{
		tmp = mini->env_list;
		while (tmp->next)
			tmp = tmp->next;
		tmp->next = new;
	}
}

void	add_env_node(t_minishell *mini, char *name, char *value, int exported)
{
	t_env	*tmp;
	t_env	*new;

	tmp = mini->env_list;
	if (update_node(tmp, name, value, exported))
		return ;
	new = create_new_node(name, value, exported);
	if (!new)
		return ;
	append_node_to_list(mini, new);
}

//////////////////

void	add_or_update_env(char *arg, t_minishell *mini)
{
	t_env	*var;
	char	*equal;
	char	*name;
	char	*value;

	equal = ft_strchr(arg, '=');
	if (!equal)
		return ;
	name = ft_substr(arg, 0, equal - arg);
	value = ft_strdup(equal + 1);
	var = find_env(mini->env_list, name);
	if (var)
	{
		free(var->value);
		var->value = value;
	}
	else
		add_env_node(mini, name, value, 1);
	free(name);
}

//////////////////

void	mark_as_exported(char *name, t_minishell *mini)
{
	t_env	*var;

	var = find_env(mini->env_list, name);
	if (var)
		var->exported = 1;
	else
		add_env_node(mini, name, NULL, 1);
}

//////////////////

int	process_export_argument(char *arg, t_minishell *mini)
{
	char	*equal;
	char	*name;
	int		error;

	error = 0;
	equal = ft_strchr(arg, '=');
	if (equal)
		name = ft_substr(arg, 0, equal - arg);
	else
		name = ft_strdup(arg);
	if (!is_valid_identifier(name))
	{
		ft_putstr("export: `", 2);
		ft_putstr(arg, 2);
		ft_putstr("': not a valid identifier\n", 2);
		error = 1;
	}
	else if (equal)
		add_or_update_env(arg, mini);
	else
		mark_as_exported(name, mini);
	free(name);
	return (error);
}

//////////////////////////

void	ft_export(t_minishell *mini)
{
	int	i;
	int	error;

	i = 1;
	error = 0;
	if (!mini->command_list->argv[1])
	{
		print_sorted_env(mini);
		return ;
	}
	while (mini->command_list->argv[i])
	{
		error = process_export_argument(mini->command_list->argv[i], mini);
		i++;
	}
	g_status = error;
}

void	execute_buitin(t_minishell *minishell)
{
	if (!ft_strcmp(minishell->command_list->argv[0], "exit"))
		ft_exit(minishell);
	else if (!ft_strcmp(minishell->command_list->argv[0], "cd"))
		ft_cd(minishell);
	else if (!ft_strcmp(minishell->command_list->argv[0], "unset"))
		ft_unset(minishell);
	else if (!ft_strcmp(minishell->command_list->argv[0], "export"))
		ft_export(minishell);
}

///////////////////

void	free_and_exit(char **args, char **paths, int exit_code)
{
	ft_freedoom(args);
	ft_freedoom(paths);
	exit(exit_code);
}

void	free_and_error(char *str, char *message, int exit_code, int std)
{
	free(str);
	ft_putstr(message, std);
	exit(exit_code);
}

char	*get_filename(int index)
{
	char	*number;
	char	*filename;
	char	*temp;

	number = ft_itoa(index);
	if (!number)
		return (NULL);
	temp = ft_strjoin("minishell_", number);
	free(number);
	if (!temp)
		return (NULL);
	filename = ft_strjoin(temp, ".temp");
	free(temp);
	return (filename);
}

int	is_limiter(char *line, char *limiter)
{
	if (!limiter)
		return (0);
	if (ft_strncmp(line, limiter, ft_strlen(limiter)) == 0
		&& line[ft_strlen(limiter)] == '\n')
		return (1);
	return (0);
}

/*int	process_heredoc(int fd, char *limiter)
{
	char	*line;

	while (1)
	{
		if (g_status == 130)
			return (130);
		write(1, "> ", 2);
		line = get_next_line(0);
		if (!line || ft_strcmp(line, limiter) == 0 || is_limiter(line, limiter))
		{
			if (!line)
				ft_putstr("heredoc delimited by EOF\n", 2);
			free(line);
			break ;
		}
		write(fd, line, ft_strlen(line));
		write(fd, "\n", 1);
		free(line);
	}
	return (0);
}*/

int	process_heredoc(int fd, char *limiter)
{
	char	*line;

	while (1)
	{
		if (g_status == 130)
			return (130);
		write(1, "> ", 2);
		line = get_next_line(0);
		if (g_status == 130)
		{
			free(line);
			return (130);
		}
		if (!line || ft_strcmp(line, limiter) == 0 || is_limiter(line, limiter))
		{
			if (!line)
				ft_putstr("heredoc delimited by EOF\n", 2);
			free(line);
			break ;
		}
		write(fd, line, ft_strlen(line));
		write(fd, "\n", 1);
		free(line);
	}
	return (0);
}

void	heredoc_signal(int sing)
{
	(void)sing;
	write(1, "\n", 1);
	rl_on_new_line();
	rl_replace_line("", 0);
	g_status = 130;
	close(0);
}

int	here_doc(char *limiter, char const *filename)
{
	int	fd;
	int	save_in;
	int	result;

	save_in = dup(STDIN_FILENO);
	signal(SIGINT, heredoc_signal);
	fd = open(filename, O_CREAT | O_WRONLY | O_TRUNC, 0644);
	if (fd == -1)
	{
		close(save_in);
		return (-1);
	}
	if (!limiter || !*limiter)
		exit_with_error(SYNTAX_ERROR, 1, 2);
	result = process_heredoc(fd, limiter);
	close(fd);
	if (dup2(save_in, STDIN_FILENO) == -1)
		ft_putstr("error stdin heredoc\n", 2);
	close(save_in);
	signal(SIGINT, sighandler);
	return (result);
}

int	env_list_size(t_env	*env)
{
	int	count;

	count = 0;
	while (env)
	{
		if (env->exported && env->name)
			count++;
		env = env->next;
	}
	return (count);
}

char	*env_entry(t_env *node)
{
	char	*entry;
	char	*result;

	entry = ft_strjoin(node->name, "=");
	if (!entry)
		return (NULL);
	if (node->value)
		result = ft_strjoin(entry, node->value);
	else
		result = ft_strdup(entry);
	free(entry);
	return (result);
}

char	**env_to_array(t_env *env_list)
{
	char	**env_array;
	int		i;
	int		size;

	size = env_list_size(env_list);
	i = 0;
	env_array = malloc(sizeof(char *) * (size + 1));
	if (!env_array)
		return (NULL);
	while (env_list)
	{
		if (env_list->exported && env_list->name)
		{
			env_array[i] = env_entry(env_list);
			if (!env_array)
				return (ft_freedoom(env_array), NULL);
			i++;
		}
		env_list = env_list->next;
	}
	env_array[i] = NULL;
	return (env_array);
}

int	is_builtin_str(char *str)
{
	return (!ft_strcmp(str, "echo") || !ft_strcmp(str, "cd") || !ft_strcmp(str,
			"pwd") || !ft_strcmp(str, "export") || !ft_strcmp(str, "unset")
		|| !ft_strcmp(str, "env") || !ft_strcmp(str, "exit"));
}

////////////////////////

void	ft_echo_arg(char **argv)
{
	int	new_line;
	int	i;
	int	j;

	i = 1;
	new_line = 1;
	while (argv[i] && !ft_strncmp(argv[i], "-n", 2))
	{
		j = 2;
		while (argv[i][j] == 'n')
			j++;
		if (argv[i][j] != '\0')
			break ;
		new_line = 0;
		i++;
	}
	while (argv[i])
	{
		ft_putstr(argv[i], 1);
		if (argv[i + 1])
			ft_putstr(" ", 1);
		i++;
	}
	if (new_line)
		ft_putstr("\n", 1);
}

void	ft_pwd(char **argv, char **env)
{
	char	*cwd;

	(void)argv;
	(void)env;
	cwd = getcwd(NULL, 0);
	if (!cwd)
	{
		ft_putstr("Error pwd\n", 2);
		return ;
	}
	ft_putstr(cwd, 1);
	ft_putstr("\n", 1);
	free(cwd);
}

void	ft_env(char **argv, char **env)
{
	int	i;

	(void)argv;
	i = 0;
	while (env[i])
	{
		write(1, env[i], ft_strlen(env[i]));
		write(1, "\n", 1);
		i++;
	}
}

void	execute_buitin_args(char **argv, char ***env, t_minishell *mini)
{
	if (!ft_strcmp(argv[0], "echo"))
		ft_echo_arg(argv);
	else if (!ft_strcmp(argv[0], "pwd"))
		ft_pwd(argv, *env);
	else if (!ft_strcmp(argv[0], "export"))
		ft_export(mini);
	else if (!ft_strcmp(argv[0], "env"))
		ft_env(argv, *env);
}

////////////////////////

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

////////////////////////

void	free_struct(t_pipex *data, char *message, int exit_code, int std)
{
	if (data)
	{
		if (data->pid)
		{
			free(data->pid);
			data->pid = NULL;
		}
		free(data);
	}
	ft_putstr(message, std);
	exit(exit_code);
}

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

static void	exec_paths(char **paths, char *cmd, t_minishell *mini, char **envir)
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

////////////////////////

void	ft_cmd(t_minishell *mini)
{
	char	**possible_paths;
	char	*path_line;
	char	**envir;

	envir = env_to_array(mini->env_list);
	if (is_builtin_str(mini->command_list->argv[0]))
	{
		execute_buitin_args(mini->command_list->argv, &envir, mini);
		ft_freedoom(envir);
		free_minishell(mini);
		exit(0);
	}
	if (!envir || !*envir)
		(exit_with_error("Missing environment\n", 1, 2), free_minishell(mini),
			ft_freedoom(envir));
	path_line = find_execpath(envir);
	if (!path_line)
		(exit_with_error("Error with path\n", 1, 2), free_minishell(mini),
			ft_freedoom(envir));
	possible_paths = ft_split(path_line, ':');
	if (!possible_paths)
		(exit_with_error("Error with possible path\n", 1, 2),
			free_minishell(mini), ft_freedoom(envir));
	execute_command(mini, possible_paths, envir);
}

////////////////////////

t_redir	*init_redir(int type, char const *filename)
{
	t_redir	*redir;

	redir = malloc(sizeof(t_redir));
	if (!redir)
		return (NULL);
	redir->type = type;
	redir->filename = ft_strdup(filename);
	if (!redir->filename)
		return (free(redir), NULL);
	redir->next = NULL;
	return (redir);
}

void	add_redir_to_cmd(t_minishell *mini, int type, char const *filename)
{
	t_redir	*new;

	if (!mini || !mini->curr)
		exit_with_error("mini or current command is NULL\n", 1, 2);
	new = init_redir(type, filename);
	if (!new)
		exit_with_error("init_redir failed\n", 1, 2);
	if (!mini->curr->redirs)
	{
		mini->curr->redirs = new;
		mini->curr->last_redir = new;
	}
	else
	{
		mini->curr->last_redir->next = new;
		mini->curr->last_redir = new;
	}
}

///////////////////

void	exit_with_error(char *message, int exit_code, int std)
{
	ft_putstr(message, std);
	exit(exit_code);
}

///////////////////

void	add_arg_to_command(t_minishell *mini, char *arg)
{
	int		count;
	int		i;
	char	**new_argv;

	count = 0;
	i = 0;
	if (mini->curr->argv)
	{
		while (mini->curr->argv[count])
			count++;
	}
	new_argv = malloc(sizeof(char *) * (count + 2));
	if (!new_argv)
		exit_with_error("Error malloc argv\n", 1, 2);
	while (i < count)
	{
		new_argv[i] = mini->curr->argv[i];
		i++;
	}
	new_argv[count] = ft_strdup(arg);
	new_argv[count + 1] = NULL;
	free(mini->curr->argv);
	mini->curr->argv = new_argv;
}

///////////////////

static int	glob_init(const char *pattern, t_minishell *mini, t_glob_ctx *ctx)
{
	ctx->dir = NULL;
	ctx->pat = NULL;
	ctx->matches = NULL;
	ctx->mcount = 0;
	ctx->mcap = 0;
	ctx->matched_any = 0;
	ctx->d = NULL;
	ctx->allow_dot = 0;
	if (!pattern || !mini)
		return (0);
	if (!ft_strchr(pattern, '*') && !ft_strchr(pattern, '?')
		&& !ft_strchr(pattern, '['))
		return (add_arg_to_command(mini, (char *)pattern), 0);
	if (pattern_has_slash(pattern))
		split_path(pattern, &ctx->dir, &ctx->pat);
	else
	{
		ctx->dir = ft_strdup(".");
		ctx->pat = ft_strdup(pattern);
	}
	if (!ctx->dir || !ctx->pat)
		return (free(ctx->dir), free(ctx->pat),
			add_arg_to_command(mini, (char *)pattern), 0);
	ctx->allow_dot = (ctx->pat[0] == '.');
	return (1);
}

static int	open_dir_or_error(t_glob_ctx *ctx, char *pattern, t_minishell *mini)
{
	ctx->d = opendir(ctx->dir);
	if (!ctx->d)
	{
		free(ctx->dir);
		free(ctx->pat);
		add_arg_to_command(mini, pattern);
		return (0);
	}
	return (1);
}

static void	free_matches_recursive(t_glob_ctx *ctx, size_t idx)
{
	if (!ctx->matches)
		return ;
	if (idx >= ctx->mcount)
	{
		free(ctx->matches);
		ctx->matches = NULL;
		return ;
	}
	free(ctx->matches[idx]);
	free_matches_recursive(ctx, idx + 1);
}

static int	process_and_insert(t_glob_ctx *ctx, const char *name)
{
	char	*tmp;
	char	*full;

	tmp = NULL;
	full = NULL;
	if (ft_strcmp(ctx->dir, ".") == 0)
		full = ft_strdup(name);
	else if (ft_strcmp(ctx->dir, "/") == 0)
		full = ft_strjoin("/", name);
	else
	{
		tmp = ft_strjoin(ctx->dir, "/");
		if (!tmp)
			return (0);
		full = ft_strjoin(tmp, name);
		free(tmp);
	}
	if (!full)
		return (0);
	if (!insert_sorted(&ctx->matches, &ctx->mcount, &ctx->mcap, full))
		return (free(full), 0);
	ctx->matched_any = 1;
	return (1);
}

static int	process_dir(t_glob_ctx *ctx, char *pattern, t_minishell *mini)
{
	struct dirent	*entry;
	const char		*name;

	entry = readdir(ctx->d);
	if (!entry)
		return (1);
	name = entry->d_name;
	if (!name || name[0] == '\0')
		return (process_dir(ctx, pattern, mini));
	if (!ctx->allow_dot && name[0] == '.')
		return (process_dir(ctx, pattern, mini));
	if (match_glob(ctx->pat, name))
	{
		if (!process_and_insert(ctx, name))
		{
			free_matches_recursive(ctx, 0);
			closedir(ctx->d);
			free(ctx->dir);
			free(ctx->pat);
			add_arg_to_command(mini, pattern);
			return (0);
		}
	}
	return (process_dir(ctx, pattern, mini));
}

static void	add_results(t_minishell *mini, t_glob_ctx *ctx, size_t idx)
{
	if (idx >= ctx->mcount)
		return ;
	add_arg_to_command(mini, ctx->matches[idx]);
	free(ctx->matches[idx]);
	add_results(mini, ctx, idx + 1);
}

int	expand_and_add_glob(char *pattern, t_minishell *mini)
{
	t_glob_ctx	ctx;
	int			ok;

	if (!glob_init(pattern, mini, &ctx))
		return (1);
	if (!open_dir_or_error(&ctx, pattern, mini))
		return (1);
	ok = process_dir(&ctx, pattern, mini);
	if (!ok)
		return (1);
	closedir(ctx.d);
	if (!ctx.matched_any)
	{
		free(ctx.dir);
		free(ctx.pat);
		if (ctx.matches)
			free_matches_recursive(&ctx, 0);
		add_arg_to_command(mini, pattern);
		return (1);
	}
	add_results(mini, &ctx, 0);
	free(ctx.matches);
	free(ctx.dir);
	free(ctx.pat);
	return (1);
}

////////////////////////

void	parse_red_in(t_minishell *mini, t_token **token)
{
	if (!(*token)->next || (*token)->type != T_RED_IN)
	{
		if (mini->pipex_data)
		{
			free_pipex_data(mini->pipex_data);
			mini->pipex_data = NULL;
		}
		mini->curr->redirs = NULL;
		exit_with_error("syntax error: no infile\n", 1, 2);
	}
	add_redir_to_cmd(mini, T_RED_IN, (*token)->next->value);
	*token = (*token)->next;
}
////////////////////////

void	parse_red_out(t_minishell *mini, t_token **token)
{
	if (!(*token)->next)
	{
		if (mini->pipex_data)
		{
			free_pipex_data(mini->pipex_data);
			mini->pipex_data = NULL;
		}
		mini->curr->redirs = NULL;
		exit_with_error("syntax error: no outfile\n", 1, 2);
	}
	add_redir_to_cmd(mini, T_RED_OUT, (*token)->next->value);
	*token = (*token)->next;
}
////////////////////////

char	*handle_heredoc(t_command *cmd, char *limiter, int index)
{
	char	*filename;
	int		ret;

	filename = get_filename(index);
	if (!filename)
		exit_with_error("malloc filename failed\n", 1, 2);
	ret = here_doc(limiter, filename);
	if (ret == -1)
		free_and_error(filename, "Error reading heredoc\n", 1, 2);
	if (ret == 130)
	{
		free(filename);
		g_status = 130;
		return (NULL);
	}
	cmd->is_heredoc = 1;
	cmd->heredoc_file = filename;
	return (filename);
}

void	parse_heredoc(t_minishell *mini, t_token **token, int *index)
{
	char	*filename;

	if (!(*token)->next || !(*token)->next->value)
	{
		if (mini->pipex_data)
		{
			free_pipex_data(mini->pipex_data);
			mini->pipex_data = NULL;
		}
		mini->curr->redirs = NULL;
		exit_with_error("heredoc: missing limiter\n", 1, 2);
	}
	filename = handle_heredoc(mini->curr, (*token)->next->value,
			*index);
	if (!filename)
	{
		g_status = 130;
		mini->curr->is_heredoc = 0;
		return ;
	}
	add_redir_to_cmd(mini, T_HEREDOC, filename);
	mini->curr->is_heredoc = 1;
	mini->pipex_data->count_heredoc++;
	(*index)++;
	*token = (*token)->next;
}

t_command	*init_new_command(void)
{
	t_command	*cmd;

	cmd = ft_calloc(1, sizeof(t_command));
	if (!cmd)
		return (NULL);
	cmd->argv = NULL;
	cmd->redirs = NULL;
	cmd->infile = NULL;
	cmd->outfile = NULL;
	cmd->append = 0;
	cmd->is_heredoc = 0;
	cmd->heredoc_file = NULL;
	cmd->next = NULL;
	return (cmd);
}

void	add_command_to_list(t_minishell *mini)
{
	if (!mini->head)
	{
		mini->head = mini->curr;
		return ;
	}
	mini->tmp = mini->head;
	while (mini->tmp->next)
		mini->tmp = mini->tmp->next;
	mini->tmp->next = mini->curr;
}

static void	append_exit_code(char **res, int *i, t_minishell *mini)
{
	char	*val;
	char	*tmp;

	val = get_env_value("?", mini->env_list);
	tmp = *res;
	if (val)
		*res = ft_strjoin(*res, val);
	else
		*res = ft_strjoin(*res, "");
	free(tmp);
	free(val);
	*i += 2;
}

char	*env_value(char const *name, t_env *env)
{
	while (env)
	{
		if (ft_strcmp(env->name, name) == 0)
			return (env->value);
		env = env->next;
	}
	return (NULL);
}

static void	append_variable(char **res, char *src, int *i, t_minishell *mini)
{
	char	*var;
	char	*val;
	int		j;
	int		len;

	j = *i + 1;
	while (src[j] && (ft_isalnum(src[j]) || src[j] == '_'))
		j++;
	len = j - (*i + 1);
	if (len > 0)
	{
		var = ft_substr(src, *i + 1, len);
		val = env_value(var, mini->env_list);
		free(var);
		if (val)
			*res = ft_strjoin(*res, val);
		else
			*res = ft_strjoin(*res, "");
	}
	else
		*res = ft_strjoin(*res, "$");
	*i = j;
}

void	append_var(char **res, char *src, int *i, t_minishell *mini)
{
	if (src[*i + 1] == '?')
		append_exit_code(res, i, mini);
	else
		append_variable(res, src, i, mini);
}

void	append_literal(char **res, char *src, int len)
{
	char	*lit;

	lit = ft_substr(src, 0, len);
	*res = ft_strjoin(*res, lit);
	free(lit);
}

char	*expand_env_in_str(char *src, t_minishell *mini)
{
	char	*res;
	int		i;
	int		start;

	res = ft_strdup("");
	i = 0;
	while (src[i])
	{
		if (src[i] == '$')
			append_var(&res, src, &i, mini);
		else
		{
			start = i;
			while (src[i] && src[i] != '$')
				i++;
			append_literal(&res, src + start, i - start);
		}
	}
	return (res);
}

static void	replace_char_inplace(char *s, char find, char replace)
{
	int	i;

	i = 0;
	if (!s)
		return ;
	while (s[i])
	{
		if (s[i] == find)
			s[i] = replace;
		i++;
	}
}

void	expand_token(t_token *token, t_minishell *mini)
{
	char	*expanded;

	if (token->quote == Q_SINGLE)
		return ;
	expanded = expand_env_in_str(token->value, mini);
	free(token->value);
	token->value = expanded;
	if (ft_strchr(token->value, '\x07'))
		replace_char_inplace(token->value, '\x07', '$');
}

void	parse_red_append(t_minishell *mini, t_token **token)
{
	if (!(*token)->next)
	{
		if (mini->pipex_data)
		{
			free_pipex_data(mini->pipex_data);
			mini->pipex_data = NULL;
		}
		mini->curr->redirs = NULL;
		exit_with_error("syntax error: no outfile\n", 1, 2);
	}
	add_redir_to_cmd(mini, T_RED_APPEND, (*token)->next->value);
	mini->curr->append = 1;
	*token = (*token)->next;
}

/*void	process_token(t_minishell *mini, int *index)
{
	t_token	*token;

	token = mini->t_list;
	if (token->type == T_WORD)
	{
		if (token->quote != Q_SINGLE && (ft_strchr(token->value, '$')
				|| ft_strchr(token->value, '\x07')))
			expand_token(token, mini);
		if (token->value[0] == '\0' && token->quote != Q_SINGLE)
			return ;
		if (token->quote == Q_NONE && (ft_strchr(token->value, '*')
				|| ft_strchr(token->value, '?')
				|| ft_strchr(token->value, '[')))
			expand_and_add_glob(token->value, mini);
		else
			add_arg_to_command(mini, token->value);
	}
	else if (token->type == T_RED_IN && token->next)
		parse_red_in(mini, &mini->t_list);
	else if (token->type == T_RED_OUT && token->next)
		parse_red_out(mini, &mini->t_list);
	else if (token->type == T_RED_APPEND && token->next)
		parse_red_append(mini, &mini->t_list);
	else if (token->type == T_HEREDOC && token->next)
		parse_heredoc(mini, &mini->t_list, index);
	else if (token->type == T_PIPE)
		mini->curr = NULL;
}*/

static void	handle_word_token(t_minishell *mini, t_token *token)
{
	if (token->quote != Q_SINGLE && (ft_strchr(token->value, '$')
			|| ft_strchr(token->value, '\x07')))
		expand_token(token, mini);
	if (token->value[0] == '\0' && token->quote != Q_SINGLE)
		return ;
	if (token->quote == Q_NONE && (ft_strchr(token->value, '*')
			|| ft_strchr(token->value, '?') || ft_strchr(token->value, '[')))
		expand_and_add_glob(token->value, mini);
	else
		add_arg_to_command(mini, token->value);
}

void	process_token(t_minishell *mini, int *index)
{
	t_token	*token;

	token = mini->t_list;
	if (token->type == T_WORD)
		handle_word_token(mini, token);
	else if (token->type == T_RED_IN && token->next)
		parse_red_in(mini, &mini->t_list);
	else if (token->type == T_RED_OUT && token->next)
		parse_red_out(mini, &mini->t_list);
	else if (token->type == T_RED_APPEND && token->next)
		parse_red_append(mini, &mini->t_list);
	else if (token->type == T_HEREDOC && token->next)
		parse_heredoc(mini, &mini->t_list, index);
	else if (token->type == T_PIPE)
		mini->curr = NULL;
}

t_command	*parse_commands(t_minishell *mini)
{
	int	heredoc_index;

	heredoc_index = 0;
	mini->head = NULL;
	mini->curr = NULL;
	while (mini->t_list)
	{
		if (!mini->curr)
		{
			mini->curr = init_new_command();
			add_command_to_list(mini);
		}
		process_token(mini, &heredoc_index);
		mini->t_list = mini->t_list->next;
	}
	return (mini->head);
}

int	is_redir(t_redir *redir)
{
	return (redir->type == T_RED_IN || redir->type == T_RED_OUT
		|| redir->type == T_HEREDOC || redir->type == T_RED_APPEND);
}

void	apply_one_redirection(t_minishell *mini, t_redir *redir)
{
	int	fd;

	if (redir->type == T_RED_IN || redir->type == T_HEREDOC)
		fd = open(redir->filename, O_RDONLY);
	else if (redir->type == T_RED_OUT)
		fd = open(redir->filename, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	else
		fd = open(redir->filename, O_WRONLY | O_CREAT | O_APPEND, 0644);
	if (fd == -1)
		(perror(redir->filename), free_minishell(mini), exit(1));
	if (redir->type == T_RED_IN || redir->type == T_HEREDOC)
	{
		if (dup2(fd, STDIN_FILENO) == -1)
			(perror("dup2"), close(fd), free_minishell(mini), exit(1));
	}
	else
	{
		if (dup2(fd, STDOUT_FILENO) == -1)
			(perror("dup2"), close(fd), free_minishell(mini), exit(1));
	}
	close(fd);
}

void	apply_redirections(t_minishell *mini)
{
	t_redir	*redir;

	redir = mini->command_list->redirs;
	while (redir)
	{
		if (is_redir(redir) && ft_strchr("|'\"", redir->filename[0]))
		{
			free_minishell(mini);
			exit_with_error(SYNTAX_ERROR, 1, 2);
		}
		apply_one_redirection(mini, redir);
		redir = redir->next;
	}
}

int	has_redir_type(t_command *cmd, int type)
{
	t_redir	*r;

	r = cmd->redirs;
	while (r)
	{
		if (r->type == type)
			return (1);
		r = r->next;
	}
	return (0);
}

void	child_process(t_minishell *mini, int fd[2])
{
	apply_redirections(mini);
	if (!has_redir_type(mini->command_list, T_RED_IN)
		&& !has_redir_type(mini->command_list, T_HEREDOC)
		&& mini->pipex_data->prev_fd != -1)
	{
		if (dup2(mini->pipex_data->prev_fd, STDIN_FILENO) == -1)
			exit_with_error("dup2 prev_fd failed\n", 1, 2);
	}
	if (mini->pipex_data->prev_fd != -1)
		close(mini->pipex_data->prev_fd);
	if (!has_redir_type(mini->command_list, T_RED_OUT)
		&& !has_redir_type(mini->command_list, T_RED_APPEND))
	{
		if (dup2(fd[1], STDOUT_FILENO) == -1)
			exit_with_error("dup2 pipe write failed\n", 1, 2);
	}
	close(fd[0]);
	close(fd[1]);
	if (!mini->command_list->argv || !mini->command_list->argv[0])
		exit(0);
	ft_cmd(mini);
}

void	process_and_exec(t_minishell *mini, int i)
{
	int	fd[2];

	if (pipe(fd) == -1)
		free_struct(mini->pipex_data, ERR_PIPE, 1, 2);
	mini->pipex_data->pid[i] = fork();
	if (mini->pipex_data->pid[i] == -1)
		free_struct(mini->pipex_data, ERR_FORK, 1, 2);
	if (mini->pipex_data->pid[i] == 0)
		child_process(mini, fd);
	else
	{
		if (mini->pipex_data->prev_fd != -1)
			close(mini->pipex_data->prev_fd);
		mini->pipex_data->prev_fd = fd[0];
		close(fd[1]);
	}
}

void	execute_last_command(t_minishell *mini, int i)
{
	if (mini->pipex_data->builtins == 1)
	{
		(apply_redirections(mini), execute_buitin(mini));
		if (mini->pipex_data->prev_fd != -1)
			close(mini->pipex_data->prev_fd);
	}
	else
	{
		mini->pipex_data->pid[i] = fork();
		if (mini->pipex_data->pid[i] == -1)
			free_struct(mini->pipex_data, ERR_FORK, 1, 2);
		if (mini->pipex_data->pid[i] == 0)
		{
			if (mini->pipex_data->prev_fd != -1)
			{
				if (dup2(mini->pipex_data->prev_fd, STDIN_FILENO) == -1)
					exit_with_error("dup2 final prev_fd failed\n", 1, 2);
				close(mini->pipex_data->prev_fd);
			}
			apply_redirections(mini);
			if (!mini->command_list->argv || !mini->command_list->argv[0])
				exit(0);
			ft_cmd(mini);
		}
	}
}

void	wait_status(t_pipex *data)
{
	int		status;
	pid_t	pid;
	int		count;

	count = 0;
	while (count < data->n_cmds && data->pid[count] != -1)
	{
		pid = waitpid(data->pid[count], &status, 0);
		if (pid == -1)
			perror("waitpid");
		if (pid == -1)
			exit_with_error("Waitpid failed\n", 1, 2);
		if (pid == data->pid[data->n_cmds - 1])
		{
			if (WIFEXITED(status))
				g_status = WEXITSTATUS(status);
			else if (WIFSIGNALED(status))
				g_status = 128 + WTERMSIG(status);
		}
		count++;
	}
}

void	execute_pipeline(t_minishell *mini)
{
	int	i;

	i = 0;
	while (mini->command_list && i < mini->pipex_data->n_cmds - 1)
	{
		process_and_exec(mini, i);
		mini->command_list = mini->command_list->next;
		i++;
	}
	execute_last_command(mini, i);
	if (mini->pipex_data->prev_fd != -1)
		close(mini->pipex_data->prev_fd);
	wait_status(mini->pipex_data);
}

t_pipex	*init_pipex(void)
{
	t_pipex	*px;

	px = malloc(sizeof(t_pipex));
	if (!px)
		return (NULL);
	px->prev_fd = -1;
	px->count_heredoc = 0;
	px->n_cmds = 0;
	px->builtins = 0;
	px->pid = NULL;
	px->commands = NULL;
	return (px);
}

int	is_builtin(t_minishell *mini)
{
	t_token	*token;

	token = mini->t_list;
	if (!token || !token->value)
		return (NO_BUITIN);
	if (!ft_strcmp(token->value, "echo") || !ft_strcmp(token->value, "pwd")
		|| !ft_strcmp(token->value, "env"))
		return (BUILTIN_CHILD);
	else if (!ft_strcmp(token->value, "cd") || !ft_strcmp(token->value, "exit")
		|| !ft_strcmp(token->value, "export") || !ft_strcmp(token->value,
			"unset"))
		return (BUILTIN_PARENT);
	return (NO_BUITIN);
}

int	count_commands_list(t_minishell *mini)
{
	t_command	*cmd;
	int			count;

	count = 0;
	cmd = mini->command_list;
	while (cmd)
	{
		count++;
		cmd = cmd->next;
	}
	return (count);
}

void	delete_heredoc_files(int n)
{
	int		i;
	char	*filename;

	i = 0;
	while (i < n)
	{
		filename = get_filename(i);
		if (filename)
		{
			unlink(filename);
			free(filename);
		}
		i++;
	}
}

/*void	ft_execute(t_minishell *mini)
{
	int		i;
	t_token	*tokken;

	i = 0;
	mini->pipex_data = init_pipex();
	if (!mini->pipex_data)
		exit_with_error("Error init_pipex\n", 1, 2);
	mini->pipex_data->builtins = is_builtin(mini);
	tokken = mini->t_list;
	mini->command_list = parse_commands(mini);
	mini->t_list = tokken;
	mini->pipex_data->commands = mini->command_list;
	mini->pipex_data->n_cmds = count_commands_list(mini);
	if (mini->pipex_data->n_cmds > 1
		&& mini->pipex_data->builtins == BUILTIN_PARENT)
		mini->pipex_data->builtins = NO_BUITIN;
	mini->pipex_data->pid = malloc(sizeof(pid_t) * mini->pipex_data->n_cmds);
	if (!mini->pipex_data->pid)
		exit_with_error("Error malloc pid failed\n", 1, 2);
	while (i < mini->pipex_data->n_cmds)
		mini->pipex_data->pid[i++] = -1;
	execute_pipeline(mini);
	delete_heredoc_files(mini->pipex_data->count_heredoc);
	if (mini->pipex_data)
		free_pipex_data(mini->pipex_data);
}*/

void	ft_execute(t_minishell *mini)
{
	int		i;
	t_token	*tokken;

	i = 0;
	mini->pipex_data = init_pipex();
	if (!mini->pipex_data)
		exit_with_error("Error init_pipex\n", 1, 2);
	mini->pipex_data->builtins = is_builtin(mini);
	tokken = mini->t_list;
	mini->command_list = parse_commands(mini);
	if (g_status == 130)
	{
		g_status = -g_status;
		delete_heredoc_files(mini->pipex_data->count_heredoc);
		if (mini->pipex_data)
			free_pipex_data(mini->pipex_data);
		mini->pipex_data = NULL;
		mini->t_list = tokken;
		return ;
	}
	mini->t_list = tokken;
	mini->pipex_data->commands = mini->command_list;
	mini->pipex_data->n_cmds = count_commands_list(mini);
	if (mini->pipex_data->n_cmds > 1
		&& mini->pipex_data->builtins == BUILTIN_PARENT)
		mini->pipex_data->builtins = NO_BUITIN;
	mini->pipex_data->pid = malloc(sizeof(pid_t) * mini->pipex_data->n_cmds);
	if (!mini->pipex_data->pid)
		exit_with_error("Error malloc pid failed\n", 1, 2);
	while (i < mini->pipex_data->n_cmds)
		mini->pipex_data->pid[i++] = -1;
	execute_pipeline(mini);
	delete_heredoc_files(mini->pipex_data->count_heredoc);
	if (mini->pipex_data)
		free_pipex_data(mini->pipex_data);
}

void	sighandler(int signal)
{
	if (signal == SIGINT)
	{
		g_status = SIGINT;
		write(STDOUT_FILENO, "\n", 1);
		rl_on_new_line();
		rl_replace_line("", 0);
		rl_redisplay();
	}
}

void	do_signal(void)
{
	signal(SIGINT, sighandler);
	signal(SIGQUIT, SIG_IGN);
}

static int	handle_heredoc_and_error(char *input, t_minishell *shell)
{
	int	count;

	count = 0;
	while (input[count] == '<')
		count++;
	if (count > 2)
	{
		ft_putstr("minishell: syntax error near unexpected token `<<<'\n",
			STDERR_FILENO);
		shell->tokenizer->err = 1;
		return (0);
	}
	if (count == 2)
	{
		shell->tokenizer->prev_type = T_HEREDOC;
		shell->tokenizer->pos += 2;
		return (1);
	}
	return (0);
}

static int	handle_redirection_append(char *input, t_minishell *shell)
{
	if (input[0] == '>' && input[1] == '>')
	{
		shell->tokenizer->prev_type = T_RED_APPEND;
		shell->tokenizer->pos += 2;
		return (1);
	}
	return (0);
}

char	*extract_quoted_token(t_minishell *shell)
{
	int		j;
	char	*val;
	char	quote_char;

	quote_char = shell->tokenizer->input[shell->tokenizer->pos];
	j = shell->tokenizer->pos + 1;
	while (shell->tokenizer->input[j]
		&& shell->tokenizer->input[j] != quote_char)
	{
		j++;
	}
	if (shell->tokenizer->input[j] == '\0')
	{
		shell->tokenizer->err = 1;
		return (NULL);
	}
	val = ft_substr(shell->tokenizer->input, shell->tokenizer->pos + 1, j
			- shell->tokenizer->pos - 1);
	shell->tokenizer->prev_type = T_WORD;
	shell->tokenizer->pos = j + 1;
	return (val);
}

static int	is_word_char(char c)
{
	if (c == '\0')
		return (0);
	if (c == ' ' || c == '\t')
		return (0);
	if (c == '|' || c == '<' || c == '>')
		return (0);
	if (c == '\'' || c == '"')
		return (0);
	return (1);
}

char	*extract_word(t_minishell *shell)
{
	int		start;
	char	*word;

	start = shell->tokenizer->pos;
	while (is_word_char(shell->tokenizer->input[shell->tokenizer->pos]))
		shell->tokenizer->pos++;
	if (shell->tokenizer->pos == start)
		return (NULL);
	word = ft_substr(shell->tokenizer->input, start,
			shell->tokenizer->pos - start);
	return (word);
}

char	*get_next_token_part(t_minishell *shell)
{
	char	*temp;

	if (shell->tokenizer->input[shell->tokenizer->pos] == '\'')
	{
		shell->tokenizer->quote = Q_SINGLE;
		temp = extract_quoted_token(shell);
	}
	else if (shell->tokenizer->input[shell->tokenizer->pos] == '"')
	{
		shell->tokenizer->quote = Q_DOUBLE;
		temp = extract_quoted_token(shell);
	}
	else
	{
		temp = extract_word(shell);
		shell->tokenizer->quote = Q_NONE;
	}
	return (temp);
}

static char	*join_free(char *s1, char *s2)
{
	char	*res;

	res = ft_strjoin(s1, s2);
	free(s1);
	free(s2);
	return (res);
}

static int	process_token_part(t_minishell *shell, char **token,
	t_token_quote *first_quote, int *mixed)
{
	char	*part;

	while (shell->tokenizer->input[shell->tokenizer->pos]
		&& shell->tokenizer->input[shell->tokenizer->pos] != ' '
		&& shell->tokenizer->input[shell->tokenizer->pos] != '\t'
		&& !ft_strchr("|<>", shell->tokenizer->input[shell->tokenizer->pos]))
	{
		part = get_next_token_part(shell);
		if (!part)
			return (0);
		if (*first_quote == (t_token_quote) - 1)
			*first_quote = shell->tokenizer->quote;
		else if (shell->tokenizer->quote != *first_quote)
			*mixed = 1;
		if (shell->tokenizer->quote == Q_SINGLE)
			replace_char_inplace(part, '$', '\x07');
		*token = join_free(*token, part);
	}
	return (1);
}

char	*extract_complex_token(t_minishell *shell)
{
	char			*token;
	t_token_quote	first_quote;
	int				mixed;

	mixed = 0;
	first_quote = (t_token_quote)-1;
	token = ft_strdup("");
	if (!token)
	{
		shell->tokenizer->err = 1;
		return (NULL);
	}
	if (!process_token_part(shell, &token, &first_quote, &mixed))
	{
		free(token);
		return (NULL);
	}
	shell->tokenizer->prev_type = T_WORD;
	if (!mixed && first_quote != (t_token_quote)-1)
		shell->tokenizer->quote = first_quote;
	else
		shell->tokenizer->quote = Q_NONE;
	return (token);
}

static int	extract_double_metachar(t_minishell *shell)
{
	char	*input;

	input = shell->tokenizer->input + shell->tokenizer->pos;
	if (input[0] == '<')
		return (handle_heredoc_and_error(input, shell));
	if (input[0] == '>' && input[1] == '>')
		return (handle_redirection_append(input, shell));
	return (0);
}

static int	extract_single_metachar(t_minishell *shell)
{
	char	c;

	c = shell->tokenizer->input[shell->tokenizer->pos];
	if (c == '|')
		shell->tokenizer->prev_type = T_PIPE;
	else if (c == '>')
		shell->tokenizer->prev_type = T_RED_OUT;
	else if (c == '<')
		shell->tokenizer->prev_type = T_RED_IN;
	else
		return (0);
	shell->tokenizer->pos++;
	return (1);
}

char	*extract_metachar(t_minishell *shell)
{
	char	*symbol;

	symbol = NULL;
	shell->tokenizer->err = 0;
	if (extract_double_metachar(shell))
		symbol = ft_substr(shell->tokenizer->input,
				shell->tokenizer->pos - 2, 2);
	else if (extract_single_metachar(shell))
		symbol = ft_substr(shell->tokenizer->input,
				shell->tokenizer->pos - 1, 1);
	return (symbol);
}

char	*extract_token(t_minishell *shell)
{
	char	*val;

	while (shell->tokenizer->input[shell->tokenizer->pos] == ' '
		|| shell->tokenizer->input[shell->tokenizer->pos] == '\t')
		shell->tokenizer->pos++;
	if (shell->tokenizer->input[shell->tokenizer->pos] == '\0')
		return (NULL);
	val = extract_metachar(shell);
	if (val)
		return (val);
	return (extract_complex_token(shell));
}

t_token	*add_token(t_minishell *minishell, char *value)
{
	t_token	*new_node;
	t_token	*current;

	new_node = malloc(sizeof(t_token));
	if (!new_node)
		return (NULL);
	new_node->value = ft_strdup(value);
	new_node->type = minishell->tokenizer->prev_type;
	new_node->quote = Q_NONE;
	new_node->expansion_type = NO_EXPANSION;
	new_node->next = NULL;
	if (minishell->t_list == NULL)
		minishell->t_list = new_node;
	else
	{
		current = minishell->t_list;
		while (current->next)
			current = current->next;
		current->next = new_node;
	}
	return (new_node);
}

t_token	*check_expansion(t_minishell *minishell, char *val)
{
	t_token	*new_token;

	new_token = add_token(minishell, val);
	if (!new_token)
		return (NULL);
	new_token->type = minishell->tokenizer->prev_type;
	new_token->quote = minishell->tokenizer->quote;
	if (new_token->type == T_WORD && new_token->value[0] == '$')
	{
		if (new_token->quote == Q_SINGLE)
			new_token->expansion_type = NO_EXPANSION;
		else if (new_token->value[1] == '?' && new_token->value[2] == '\0')
			new_token->expansion_type = EXIT_STATUS_EXPANSION;
		else
			new_token->expansion_type = VAR_EXPANSION;
	}
	else
		new_token->expansion_type = NO_EXPANSION;
	if (new_token->quote == Q_SINGLE && ft_strchr(new_token->value, '\x07'))
		replace_char_inplace(new_token->value, '\x07', '$');
	return (new_token);
}

int	tokenize_input(t_minishell *minishell)
{
	char	*val;

	while (!minishell->tokenizer->err)
	{
		val = extract_token(minishell);
		if (minishell->tokenizer->err || !val)
			break ;
		if (!check_expansion(minishell, val))
		{
			free(val);
			return (0);
		}
		free(val);
	}
	return (1);
}

int	init_tokenizer(t_minishell *minishell, char *input)
{
	if (minishell->t_list)
	{
		free_t_list(minishell->t_list);
		minishell->t_list = NULL;
	}
	if (minishell->tokenizer)
		free(minishell->tokenizer);
	minishell->tokenizer = malloc(sizeof(t_tokenizer));
	if (!minishell->tokenizer)
		return (0);
	minishell->tokenizer->input = input;
	minishell->tokenizer->pos = 0;
	minishell->tokenizer->prev_type = T_WORD;
	minishell->tokenizer->quote = Q_NONE;
	minishell->tokenizer->err = 0;
	return (1);
}

void	cleanup_tokenizer(t_minishell *minishell, int success)
{
	if (minishell->tokenizer)
	{
		if (minishell->tokenizer->err)
			success = 0;
		free(minishell->tokenizer);
		minishell->tokenizer = NULL;
	}
	if (!success)
	{
		free_t_list(minishell->t_list);
		minishell->t_list = NULL;
	}
}

int	fill_tokens(t_minishell *minishell, char *input)
{
	int	success;

	success = 1;
	if (!init_tokenizer(minishell, input))
		return (0);
	success = tokenize_input(minishell);
	cleanup_tokenizer(minishell, success);
	return (success);
}

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
///////////

static int	env_len(char **env)
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
//////////

volatile sig_atomic_t	g_status = 0;

int	check_syntax_pipes(t_token *tokenizer)
{
	if (!tokenizer)
		return (1);
	if (tokenizer->type == T_PIPE)
	{
		ft_putstr("minishell: syntax error near unexpected token `|'\n", 2);
		g_status = 2;
		return (0);
	}
	while (tokenizer->next)
	{
		if (tokenizer->type == T_PIPE && tokenizer->next->type == T_PIPE)
		{
			ft_putstr("minishell: syntax error near unexpected token `|'\n", 2);
			g_status = 2;
			return (0);
		}
		tokenizer = tokenizer->next;
	}
	if (tokenizer->type == T_PIPE)
	{
		ft_putstr("minishell: syntax error near unexpected token `|'\n", 2);
		g_status = 2;
		return (0);
	}
	return (1);
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

static void	run_group_child(t_minishell *parent, char *inner)
{
	t_minishell	child;
	char		**env_arr;
	char		*status_str;

	signal(SIGINT, SIG_DFL);
	signal(SIGQUIT, SIG_DFL);
	child = init_minishell();
	env_arr = env_to_array(parent->env_list);
	if (env_arr)
		child.env_list = create_env_list(env_arr, &child);
	status_str = ft_itoa(g_status);
	if (status_str)
	{
		add_env_node(&child, "?", status_str, 0);
		free(status_str);
	}
	if (inner)
		process_input(inner, &child);
	else
		process_input("", &child);
	if (child.env_list)
		free_env_list(child.env_list);
	if (env_arr)
		ft_freedoom(env_arr);
	exit(g_status);
}

static int	execute_group_in_subshell(t_minishell *parent, char *inner)
{
	pid_t	pid;
	int		status;

	pid = fork();
	if (pid == -1)
		return (perror("fork"), 1);
	if (pid == 0)
		run_group_child(parent, inner);
	if (waitpid(pid, &status, 0) == -1)
		return (perror("waitpid"), 1);
	if (WIFEXITED(status))
		g_status = WEXITSTATUS(status);
	else if (WIFSIGNALED(status))
		g_status = 128 + WTERMSIG(status);
	return (g_status);
}

static char	*trim_whitespace(char *s)
{
	char	*start;
	char	*end;

	if (!s)
		return (NULL);
	start = s;
	while (*start && (*start == ' ' || *start == '\t'))
		start++;
	end = start + ft_strlen(start);
	while (end > start && (*(end - 1) == ' ' || *(end - 1) == '\t'))
		end--;
	return (ft_substr(start, 0, end - start));
}

////
static int	handle_quote_paren(char *input, t_split_state *st)
{
	if (input[st->pos] == '\'' || input[st->pos] == '"')
	{
		if (!st->quote)
			st->quote = input[st->pos];
		else if
			(st->quote == input[st->pos]) st->quote = 0;
		st->pos++;
		return (1);
	}
	if (!st->quote && input[st->pos] == '(')
	{
		st->paren_depth++;
		st->pos++;
		return (1);
	}
	if (!st->quote && input[st->pos] == ')')
	{
		if (st->paren_depth > 0)
			st->paren_depth--;
		st->pos++;
		return (1);
	}
	return (0);
}

static int	handle_operator(char *input, t_split_state *st, char *op)
{
	char	*seg;
	char	*trimmed;
	char	*to_add;

	seg = ft_substr(input, st->start, st->pos - st->start);
	trimmed = trim_whitespace(seg);
	free(seg);
	if (trimmed)
		to_add = trimmed;
	else
	{
		to_add = ft_strdup("");
		if (!to_add)
			return (0);
	}
	if (!append_ptr(&st->segments, st->seg_count + 1,
			st->seg_count, to_add))
		return (free(to_add), 0);
	st->seg_count++;
	if (!append_ptr(&st->ops, st->seg_count,
			st->seg_count - 1, ft_strdup(op)))
		return (free(st->segments[st->seg_count - 1]), st->seg_count--, 0);
	st->pos += 2;
	st->start = st->pos;
	return (1);
}

static int	try_process_operator(char *input, t_split_state *st)
{
	if (!st->quote && st->paren_depth == 0 && st->pos + 1 < st->len)
	{
		if (input[st->pos] == '&' && input[st->pos + 1] == '&')
			return (handle_operator(input, st, "&&"));
		if (input[st->pos] == '|' && input[st->pos + 1] == '|')
			return (handle_operator(input, st, "||"));
	}
	return (0);
}

static void	split_loop_and_append(char *input, t_split_state *st)
{
	char	*seg;
	char	*trimmed;

	while (st->pos < st->len)
	{
		if (handle_quote_paren(input, st))
			continue ;
		if (try_process_operator(input, st))
			continue ;
		st->pos++;
	}
	seg = ft_substr(input, st->start, st->len - st->start);
	trimmed = trim_whitespace(seg);
	free(seg);
	if (trimmed == NULL)
		trimmed = ft_strdup("");
	if (!append_ptr(&st->segments, st->seg_count + 1,
			st->seg_count, trimmed))
	{
		if (trimmed)
			free(trimmed);
		return ;
	}
	st->seg_count++;
}

static int	split_ops(char *input, char ***segments_out,
	char ***ops_out, int *count_out)
{
	t_split_state	st;

	if (!input)
		return (0);
	st.pos = 0;
	st.len = ft_strlen(input);
	st.start = 0;
	st.quote = 0;
	st.paren_depth = 0;
	st.seg_count = 0;
	st.segments = NULL;
	st.ops = NULL;
	split_loop_and_append(input, &st);
	*segments_out = st.segments;
	*ops_out = st.ops;
	*count_out = st.seg_count;
	return (1);
}

/////

static int	is_outer_parenthesized(const char *s)
{
	int		depth;
	size_t	i;
	char	quote;

	depth = 1;
	i = 1;
	quote = 0;
	if (!s || s[0] != '(')
		return (0);
	while (i < ft_strlen(s))
	{
		if ((s[i] == '\'' || s[i] == '"') && !quote)
			quote = s[i];
		else if (s[i] == quote)
			quote = 0;
		else if (!quote)
		{
			if (s[i] == '(')
				depth++;
			else if (s[i] == ')' && --depth == 0)
				break ;
		}
		i++;
	}
	return (depth == 0 && i == ft_strlen(s) - 1);
}

static char	*strip_outer_parentheses(char *s, int *removed)
{
	char	*cur;
	char	*tmp;
	int		did_remove;
	int		len;

	if (removed)
		*removed = 0;
	if (!s)
		return (NULL);
	cur = trim_whitespace(s);
	if (!cur)
		return (ft_strdup(""));
	did_remove = 0;
	while (is_outer_parenthesized(cur))
	{
		len = ft_strlen(cur);
		tmp = ft_substr(cur, 1, len - 2);
		if (!tmp)
			return (NULL);
		(free(cur), cur = trim_whitespace(tmp));
		(free(tmp), did_remove = 1);
	}
	if (removed && did_remove)
		*removed = 1;
	return (cur);
}

static void	free_split_result(char **segments, char **ops, int count)
{
	int	i;

	if (segments)
	{
		i = 0;
		while (i < count)
		{
			free(segments[i]);
			i++;
		}
		free(segments);
	}
	if (ops)
	{
		i = 0;
		while (i < count - 1)
		{
			free(ops[i]);
			i++;
		}
		free(ops);
	}
}

static int	prepare_segments(char *input, char ***segments,
	char ***ops, int *seg_count)
{
	add_history(input);
	g_status = 0;
	*seg_count = 0;
	*ops = NULL;
	*segments = NULL;
	if (!split_ops(input, segments, ops, seg_count))
	{
		ft_putstr("minishell: internal split error\n", 2);
		return (0);
	}
	return (1);
}

static void	process_segment(t_minishell *minishell, char *seg)
{
	char	*inner;
	int		is_group;

	is_group = 0;
	if (!seg || *seg == '\0')
		return ;
	inner = strip_outer_parentheses(seg, &is_group);
	if (is_group)
	{
		if (inner)
			execute_group_in_subshell(minishell, inner);
		else
			execute_group_in_subshell(minishell, "");
		if (inner)
			free(inner);
	}
	else
		process_command(minishell, seg, inner);
}

/*static void	update_env_status(t_minishell *minishell)
{
	char	*status_str;

	status_str = ft_itoa(g_status);
	if (status_str)
	{
		add_env_node(minishell, "?", status_str, 0);
		free(status_str);
	}
}*/

static void	update_env_status(t_minishell *minishell)
{
	char	*status_str;
	int		status;

	if (g_status < 0)
	{
		status = -g_status;
		g_status = SIGINT;
	}
	else
		status = (int)g_status;
	status_str = ft_itoa(status);
	if (status_str)
	{
		add_env_node(minishell, "?", status_str, 0);
		free(status_str);
	}
}

static void	handle_segments(t_minishell *minishell,
							char **segments, char **ops, int seg_count)
{
	if (seg_count <= 0)
		return ;
	process_segment(minishell, *segments);
	update_env_status(minishell);
	if (seg_count > 1 && ops && *ops)
	{
		if (!ft_strcmp(*ops, "&&") && g_status != 0)
		{
			segments++;
			ops++;
			seg_count--;
		}
		else if (!ft_strcmp(*ops, "||") && g_status == 0)
		{
			segments++;
			ops++;
			seg_count--;
		}
	}
	handle_segments(minishell, segments + 1, ops + 1, seg_count - 1);
}

static void	process_input(char *input, t_minishell *minishell)
{
	char	**segments;
	char	**ops;
	int		seg_count;

	segments = NULL;
	ops = NULL;
	seg_count = 0;
	if (!prepare_segments(input, &segments, &ops, &seg_count))
		return ;
	handle_segments(minishell, segments, ops, seg_count);
	free_split_result(segments, ops, seg_count);
}

static void	process_command(t_minishell *minishell, char *seg, char *inner)
{
	if (!fill_tokens(minishell, seg))
	{
		ft_putstr("syntax error: unclosed quote\n", 2);
		g_status = 2;
		update_env_status(minishell);
		free_t_list(minishell->t_list);
		minishell->t_list = NULL;
		if (inner)
			free(inner);
		return ;
	}
	if (!check_syntax_pipes(minishell->t_list))
	{
		update_env_status(minishell);
		free_t_list(minishell->t_list);
		minishell->t_list = NULL;
		if (inner)
			free(inner);
		return ;
	}
	ft_execute(minishell);
	free_t_list(minishell->t_list);
	minishell->t_list = NULL;
	if (inner)
		free(inner);
}

////////
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
		if (!input)
		{
			(free_env_list(mini->env_list), free_tokenizer(mini->tokenizer),
				free_t_list(mini->t_list), close(saved_stdin));
			break ;
		}
		if (*input == '\0' || g_status == 128 + SIGINT)
		{
			(free(input), g_status = 0, dup2(saved_stdin, STDIN_FILENO),
				close(saved_stdin));
			continue ;
		}
		(process_input(input, mini), free(input), dup2(saved_stdin,
				STDIN_FILENO), close(saved_stdin));
	}
}

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
