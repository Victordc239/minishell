/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mini.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sofernan <sofernan@student.42madrid.es>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/07 14:02:20 by sofernan          #+#    #+#             */
/*   Updated: 2025/09/16 17:54:28 by sofernan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINI_H

# define MINI_H

# include "libft/libft.h"
# include <readline/history.h>
# include <readline/readline.h>
# include <signal.h>
# include <dirent.h>
# define ERR_FLASH "Error\n"
# define ERR_ARG "Error Arg or Pipe\n"
# define ERR_PIPE "Error Pipe\n"
# define ERR_FORK "Error Fork\n"
# define ERRO_INFILE "No such file or directory\n"
# define ERRO_DUP "Error DUP\n"
# define ERRO_OUFILE "Error outfile\n"
# define ERRO_DOC "Error here doc\n"
# define ERRO_FEW "Few args\n"
# define SYNTAX_ERROR "bash: syntax error near unexpected token `newline'\n"

extern volatile sig_atomic_t	g_status;

typedef struct s_env
{
	int							signal;
	char						*name;
	char						*value;
	int							exported;
	struct s_env				*next;
}								t_env;

typedef enum e_token_type
{
	T_WORD,
	T_PIPE,
	T_RED_IN,
	T_RED_OUT,
	T_RED_APPEND,
	T_HEREDOC,
	T_INFILE,
	T_OUTFILE,
}								t_token_type;

typedef enum e_token_quote
{
	Q_NONE,
	Q_SINGLE,
	Q_DOUBLE
}								t_token_quote;

typedef enum e_expansion_type
{
	NO_EXPANSION,
	VAR_EXPANSION,
	EXIT_STATUS_EXPANSION
}								t_expansion_type;

typedef struct s_tokenizer
{
	char						*input;
	int							pos;
	t_token_type				prev_type;
	t_token_quote				quote;
	int							err;
}								t_tokenizer;

typedef struct s_token
{
	char						*value;
	t_token_type				type;
	t_token_quote				quote;
	t_expansion_type			expansion_type;
	struct s_token				*next;
}								t_token;

typedef struct s_redir
{
	int							type;
	char						*filename;
	struct s_redir				*next;
}								t_redir;

typedef struct s_redir_list
{
	t_redir						*first;
	t_redir						*last;
}								t_redir_list;

typedef struct s_command
{
	char						**argv;
	char						*infile;
	char						*outfile;
	int							append;
	int							is_heredoc;
	char						*heredoc_file;
	struct s_command			*next;
	t_redir						*redirs;
	t_redir						*last_redir;
}								t_command;

typedef struct s_fd_pipex
{
	int							prev_fd;
	int							count_heredoc;
	int							n_cmds;
	int							builtins;
	pid_t						*pid;
	t_command					*commands;
}								t_pipex;

typedef struct s_minishell
{
	t_env						*env_list;
	t_token						*t_list;
	t_command					*command_list;
	t_command					*head;
	t_command					*tmp;
	t_command					*curr;
	t_tokenizer					*tokenizer;
	t_pipex						*pipex_data;
	t_token						*curr_token;
	t_token						*new_token;
	t_token						*new_node;
	t_token						*current;
	char						**paths_execve;
	char						**envir_execve;
}								t_minishell;

typedef struct s_split_state
{
	int							pos;
	int							len;
	int							start;
	int							paren_depth;
	int							seg_count;
	char						quote;
	char						**segments;
	char						**ops;
}								t_split_state;

typedef struct s_glob_ctx
{
	char						*dir;
	char						*pat;
	char						**matches;
	size_t						mcount;
	size_t						mcap;
	int							matched_any;
	int							allow_dot;
	DIR							*d;
}								t_glob_ctx;

typedef enum e_builtin_type
{
	NO_BUITIN = -1,
	BUILTIN_CHILD = 0,
	BUILTIN_PARENT = 1
}								t_builtin_type;

int			is_builtin(t_minishell *mini);
int			is_builtin_str(char *str);
int			is_numeric(char const *str);
int			ft_exit(t_minishell *mini);
int			count_exported(t_minishell *mini);
int			is_valid_identifier(char const *str);
int			process_export_argument(char *arg, t_minishell *mini);
int			update_node(t_env *tmp, char *name, char *value, int exported);
int			count_commands_list(t_minishell *mini);
int			is_limiter(char *line, char *limiter);
int			here_doc(char *limiter, char const *filename);
int			process_heredoc(int fd, char *limiter);
int			env_list_size(t_env *env);
int			is_redir(t_redir *redir);
int			has_redir_type(t_command *cmd, int type);
int			check_syntax_pipes(t_token *tokenizer);
int			fill_tokens(t_minishell *minishell, char *input);
int			init_tokenizer(t_minishell *minishell, char *input);
int			tokenize_input(t_minishell *minishell);
static int	env_len(char **env);
static int	handle_heredoc_and_error(char *input, t_minishell *shell);
static int	handle_redirection_append(char *input, t_minishell *shell);
static int	extract_double_metachar(t_minishell *shell);
static int	extract_single_metachar(t_minishell *shell);
static int	is_word_char(char c);
static int	split_ops(char *input, char ***segments_out,
				char ***ops_out, int *count_out);
static int	execute_group_in_subshell(t_minishell *parent, char *inner);
static int	is_outer_parenthesized(const char *s);
static int	process_token_part(t_minishell *shell, char **token,
				t_token_quote *first_quote, int *mixed);
void		execute_buitin(t_minishell *minishell);
void		ft_cd(t_minishell *mini);
void		ft_cmd(t_minishell *mini);
void		ft_echo_arg(char **argv);
void		ft_env(char **argv, char **env);
void		print_env_array(t_env **arr, int count);
void		print_sorted_env(t_minishell *mini);
void		ft_export(t_minishell *mini);
void		sort_env_array(t_env **arr, int count);
void		ft_pwd(char **argv, char **env);
void		ft_unset(t_minishell *mini);
void		add_or_update_env(char *arg, t_minishell *mini);
void		remove_env_var(char const *name, t_minishell *mini);
void		mark_as_exported(char *name, t_minishell *mini);
void		append_node_to_list(t_minishell *mini, t_env *new);
void		update_node_value(t_env *tmp, char *value, int exported);
void		add_env_node(t_minishell *mini, char *name,
				char *value, int exported);
void		execute_buitin_args(char **argv, char ***env, t_minishell *mini);
void		add_command_to_list(t_minishell *mini);
void		add_arg_to_command(t_minishell *mini, char *arg);
void		free_struct(t_pipex *data, char *message, int exit_code, int std);
void		exit_with_error(char *message, int exit_code, int std);
void		ft_freedoom(char **str);
void		free_and_exit(char **args, char **paths, int exit_code);
void		free_and_error(char *str, char *message, int exit_code, int std);
void		delete_heredoc_files(int n);
void		heredoc_signal(int sing);
void		parse_red_in(t_minishell *mini, t_token **token);
void		parse_red_out(t_minishell *mini, t_token **token);
void		parse_red_append(t_minishell *mini, t_token **token);
void		parse_heredoc(t_minishell *mini, t_token **token, int *index);
void		process_token(t_minishell *mini, int *index);
void		apply_redirections(t_minishell *mini);
void		apply_one_redirection(t_minishell *mini, t_redir *redir);
void		add_redir_to_cmd(t_minishell *mini, int type, char const *filename);
void		execute_command(t_minishell *mini, char **paths, char **envir);
void		check_errno(int err, t_minishell *mini);
void		wait_status(t_pipex *data);
void		process_and_exec(t_minishell *mini, int i);
void		child_process(t_minishell *mini, int fd[2]);
void		execute_pipeline(t_minishell *mini);
void		execute_last_command(t_minishell *mini, int i);
void		ft_execute(t_minishell *mini);
void		free_env_list(t_env *env);
void		free_t_list(t_token *list);
void		free_redir_list(t_redir *redir);
void		free_command_list(t_command *cmd);
void		free_pipex_data(t_pipex *data);
void		free_minishell(t_minishell *shell);
void		free_tokenizer(t_tokenizer *tokenizer);
void		sighandler(int signal);
void		do_signal(void);
void		append_var(char **res, char *src, int *i, t_minishell *mini);
void		append_literal(char **res, char *src, int len);
void		cleanup_tokenizer(t_minishell *minishell, int success);
void		expand_token(t_token *token, t_minishell *mini);
void		mini_loop(t_minishell *mini);
static void	append_exit_code(char **res, int *i, t_minishell *mini);
static void	append_variable(char **res, char *src, int *i, t_minishell *mini);
static void	process_input(char *input, t_minishell *minishell);
static void	replace_char_inplace(char *s, char find, char replace);
static void	free_split_result(char **segments, char **ops, int count);
static void	exec_paths(char **paths, char *cmd,
				t_minishell *mini, char **envir);
static void	run_group_child(t_minishell *parent, char *inner);
char		*get_env_value(char *name, t_env *env);
char		*get_filename(int index);
char		*handle_heredoc(t_command *cmd, char *limiter, int index);
char		*env_entry(t_env *node);
char		*find_execpath(char **envir);
char		*create_path(char *possible_path, char *command);
char		*expand_env_in_str(char *src, t_minishell *mini);
char		*extract_metachar(t_minishell *shell);
char		*get_next_token_part(t_minishell *shell);
char		*extract_quoted_token(t_minishell *shell);
char		*extract_complex_token(t_minishell *shell);
char		*extract_word(t_minishell *shell);
char		*extract_token(t_minishell *shell);
char		*env_value(char const *name, t_env *env);
char		*get_prompt(void);
char		**copy_env(char **env);
char		**env_to_array(t_env *env_list);
static char	*join_free(char *s1, char *s2);
static char	*trim_whitespace(char *s);
static char	*strip_outer_parentheses(char *s, int *removed);
t_env		*create_new_node(char *name, char *value, int exported);
t_env		*find_env(t_env *env_list, char const *name);
t_env		*create_env_list(char **envp, t_minishell *mini);
t_command	*init_new_command(void);
t_command	*parse_commands(t_minishell *mini);
t_token		*check_expansion(t_minishell *minishell, char *val);
t_token		*add_token(t_minishell *minishell, char *value);
t_redir		*init_redir(int type, char const *filename);
t_pipex		*init_pipex(void);
t_minishell	init_minishell(void);

static int	pattern_has_slash(const char *s);
static char	*pattern_dirname(const char *pattern);
static char	*pattern_basename(const char *pattern);
static int	match_class(const char **pp, char c);
static int	match_glob(const char *pat, const char *s);
static int	insert_sorted(char ***arr, size_t *count, size_t *cap, char *s);

static void	handle_word_token(t_minishell *mini, t_token *token);

static int	handle_operator(char *input, t_split_state *st, char *op);

static int	prepare_segments(char *input, char ***segments,
				char ***ops, int *seg_count);
static void	process_segment(t_minishell *minishell, char *seg);
static void	update_env_status(t_minishell *minishell);
static void	handle_segments(t_minishell *minishell, char **segments,
				char **ops, int seg_count);
static void	process_input(char *input, t_minishell *minishell);
static void	process_command(t_minishell *minishell, char *seg, char *inner);

static int	handle_quote_paren(char *input, t_split_state *st);
static int	try_process_operator(char *input, t_split_state *st);
static void	split_loop_and_append(char *input, t_split_state *st);
static int	split_ops(char *input, char ***segments_out,
				char ***ops_out, int *count_out);

static const char	*init_class(const char *p, char c, int *negate, int *matched);
static const char	*process_class_content(const char *p, char c, int *matched);

static int	append_ptr(char ***arr, int new_size, int copy_count, char *value);

int			expand_and_add_glob(char *pattern, t_minishell *mini);
static void	add_results(t_minishell *mini, t_glob_ctx *ctx, size_t idx);
static int	process_dir(t_glob_ctx *ctx, char *pattern, t_minishell *mini);
static int	process_and_insert(t_glob_ctx *ctx, const char *name);
static void	free_matches_recursive(t_glob_ctx *ctx, size_t idx);
static int	open_dir_or_error(t_glob_ctx *ctx, char *pattern, t_minishell *mini);
static int	glob_init(const char *pattern, t_minishell *mini, t_glob_ctx *ctx);

#endif