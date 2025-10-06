/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mini.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sofernan <sofernan@student.42madrid.es>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/07 14:02:20 by sofernan          #+#    #+#             */
/*   Updated: 2025/10/06 19:55:43 by sofernan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINI_H

# define MINI_H

# include "libft/libft.h"
# include <readline/history.h>
# include <readline/readline.h>
# include <signal.h>
# include <dirent.h>
# include <sys/stat.h>
# define ERR_PIPE "Error Pipe\n"
# define ERR_FORK "Error Fork\n"
# define SYNTAX_ERROR "bash: syntax error near unexpected token `newline'\n"

extern volatile sig_atomic_t	g_status;

typedef struct s_env
{
	int					signal;
	int					exported;
	char				*name;
	char				*value;
	struct s_env		*next;
}						t_env;

typedef enum e_token_type
{
	T_WORD,
	T_PIPE,
	T_RED_IN,
	T_RED_OUT,
	T_RED_APPEND,
	T_RED_INOUT,
	T_EOF,
	T_HEREDOC,
	T_INFILE,
	T_OUTFILE,
}						t_token_type;

typedef enum e_token_quote
{
	Q_NONE,
	Q_SINGLE,
	Q_DOUBLE
}						t_token_quote;

typedef enum e_expansion_type
{
	NO_EXPANSION,
	VAR_EXPANSION,
	EXIT_STATUS_EXPANSION
}						t_expansion_type;

typedef struct s_tokenizer
{
	int					pos;
	int					err;
	int					last_adjacent;
	char				*input;
	t_token_type		prev_type;
	t_token_quote		quote;
}						t_tokenizer;

typedef struct s_token
{
	int					adjacent;
	char				*value;
	struct s_token		*next;
	t_token_type		type;
	t_token_quote		quote;
	t_expansion_type	expansion_type;
}						t_token;

typedef struct s_redir
{
	int					type;
	char				*filename;
	struct s_redir		*next;
}						t_redir;

typedef struct s_command
{
	int					append;
	int					is_heredoc;
	char				**argv;
	char				*infile;
	char				*outfile;
	char				*heredoc_file;
	t_redir				*redirs;
	t_redir				*last_redir;
	struct s_command	*next;
}						t_command;

typedef struct s_fd_pipex
{
	int					prev_fd;
	int					count_heredoc;
	int					n_cmds;
	int					builtins;
	pid_t				*pid;
	t_command			*commands;
}						t_pipex;

typedef struct s_minishell
{
	char				**paths_execve;
	char				**envir_execve;
	t_env				*env_list;
	t_pipex				*pipex_data;
	t_token				*t_list;
	t_token				*curr_token;
	t_token				*new_token;
	t_token				*new_node;
	t_token				*current;
	t_command			*cmd_list;
	t_command			*head;
	t_command			*tmp;
	t_command			*curr;
	t_tokenizer			*tokenizer;
}						t_minishell;

typedef struct s_split_state
{
	int					pos;
	int					len;
	int					start;
	int					paren_depth;
	int					seg_count;
	char				quote;
	char				**segments;
	char				**ops;
}						t_split_state;

typedef struct s_glob_ctx
{
	int					matched_any;
	int					allow_dot;
	char				*dir;
	char				*pat;
	char				**matches;
	DIR					*d;
	size_t				mcount;
	size_t				mcap;
}						t_glob_ctx;

typedef struct s_extract
{
	char				quote_char;
	char				*buf;
	char				*more;
	char				*tmp;
	char				*cdup;
	char				c;
	char				next;
	size_t				j;
	size_t				len;
	size_t				pos;
	size_t				bi;
	size_t				bufcap;
}						t_extract;

void		exec_builtin_parent(t_minishell *mini);
void		ft_cd(t_minishell *mini, int free_path, int print_new);
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
void		add_node_to_list(t_minishell *mini, t_env *new);
void		update_node_value(t_env *tmp, char *value, int exported);
void		add_env_node(t_minishell *mini, char *name,
				char *value, int exported);
void		exec_builtin_child(char **argv, char ***env, t_minishell *mini);
void		add_command_to_list(t_minishell *mini);
void		add_arg_to_command(t_minishell *mini, char *arg);
void		free_struct(t_pipex *data, char *message, int exit_code, int std);
void		exit_with_error(char *message, int exit_code, int std);
void		ft_freedoom(char **str);
void		free_and_exit(char **args, char **paths, int exit_code);
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
void		execute_command(t_minishell *mini, char **paths, char **env);
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
void		free_minishell(t_minishell *mini);
void		free_tokenizer(t_tokenizer *tokenizer);
void		sighandler(int signal);
void		do_signal(void);
void		append_var(char **res, char *src, int *i, t_minishell *mini);
void		append_literal(char **res, char *src, int len);
void		expand_token(t_token *token, t_minishell *mini);
void		mini_loop(t_minishell *mini);
void		append_exit_code(char **res, int *i, t_minishell *mini);
void		append_variable(char **res, char *src, int *i, t_minishell *mini);
void		process_input(char *input, t_minishell *mini);
void		replace_char_inplace(char *s, char find, char replace);
void		free_split_result(char **segments, char **ops, int count);
void		exec_paths(char **paths, char *cmd, t_minishell *mini, char **env);
void		run_group_child(t_minishell *parent, char *inner);
void		process_segment(t_minishell *mini, char *segment);
void		update_env_status(t_minishell *mini);
void		exec_segments(t_minishell *mini, char **segments,
				char **ops, int seg_count);
void		process_command(t_minishell *mini, char *segment, char *inner);
void		process_heredoc_2(t_minishell *mini, t_token **token, int *index);
void		handle_word_token(t_minishell *mini, t_token *token);
void		remove_marker_inplace(char *s);
void		syntax_error_unexpected(t_minishell *mini, const char *tok);
void		parse_red_inout(t_minishell *mini, t_token **token);
void		ft_execute_helper(t_minishell *mini);
void		split_loop_and_append(char *input, t_split_state *st);
void		add_results(t_minishell *mini, t_glob_ctx *ctx, size_t idx);
void		free_matches_recursive(t_glob_ctx *ctx, size_t idx);
void		split_path(const char *pattern, char **dir_out, char **base_out);
int			complete_last_seg(char **cur_input, char ***segments,
				char ***ops, int *seg_count);
int			ft_isspace(int c);
int			append_more_line(char **cur_input);
int			handle_continuation(char **cur_input);
int			is_builtin(t_minishell *mini);
int			is_builtin_str(char *str);
int			is_numeric(char const *str);
int			ft_exit(t_minishell *mini);
int			count_exported(t_minishell *mini);
int			is_valid_identifier(char const *str);
int			process_export_arg(char *arg, t_minishell *mini);
int			update_node(t_env *tmp, char *name, char *value, int exported);
int			count_commands_list(t_minishell *mini);
int			here_doc(const char *limiter, const char *filename,
				t_minishell *mini, t_token_quote quote);
int			process_heredoc(int fd, const char *limiter,
				t_minishell *mini, t_token_quote quote);
int			env_list_size(t_env *env);
int			has_redir_type(t_command *cmd, int type);
int			check_syntax_pipes(t_token *tokenizer);
int			fill_tokens(t_minishell *mini, char *input);
int			init_tokenizer(t_minishell *mini, char *input);
int			tokenize_input(t_minishell *mini);
int			env_len(char **env);
int			handle_heredoc_and_error(char *input, t_minishell *mini);
int			handle_redirection_append(char *input, t_minishell *mini);
int			extract_double_metachar(t_minishell *mini);
int			extract_single_metachar(t_minishell *mini);
int			is_word_char(char c);
int			split_ops(char *input, char ***segments_out,
				char ***ops_out, int *count_out);
int			execute_subshell(t_minishell *parent, char *inner);
int			is_outer_parenthesized(const char *s);
int			process_token_part(t_minishell *mini, char **token,
				t_token_quote *first_quote, int *mixed);
int			pattern_has_slash(const char *s);
int			match_class(const char **pp, char c);
int			match_glob(const char *pat, const char *s);
int			insert_sorted(char ***arr, size_t *count, size_t *cap, char *s);
int			handle_operator(char *input, t_split_state *st, char *op);
int			prepare_segments(char *input, char ***segments,
				char ***ops, int *seg_count);
int			handle_quote_paren(char *input, t_split_state *st);
int			try_process_operator(char *input, t_split_state *st);
int			ends_with_unquoted_redir(const char *s, int in_sq, int in_dq);
int			ends_with_unquoted_continuation_op(const char *s);
int			handle_input_cycle(char *input, t_minishell *mini, int saved_stdin);
int			split_ops(char *input, char ***segments_out,
				char ***ops_out, int *count_out);
int			append_ptr(char ***arr, int new_size, int copy_count, char *value);
int			expand_and_add_glob(char *pattern, t_minishell *mini);
int			open_dir(t_glob_ctx *ctx, char *pattern, t_minishell *mini);
int			glob_init(const char *pattern, t_minishell *mini, t_glob_ctx *ctx);
int			match_glob_star(const char *p, const char *str);
int			process_dir(t_glob_ctx *ctx, char *pattern, t_minishell *mini);
int			process_and_insert(t_glob_ctx *ctx, const char *name);
int			ensure_capacity(char ***arr, size_t *count, size_t *cap);
int			parse_exit_code(const char *s, unsigned char *out_code,
				int i, int neg);
int			is_all_digits(const char *s);
char		*get_env_value(char *name, t_env *env);
char		*get_filename(int index);
char		*handle_heredoc(t_minishell *mini, t_command *cmd,
				t_token *tok, int index);
char		*env_entry(t_env *node);
char		*find_execpath(char **env);
char		*create_path(char *possible_path, char *command);
char		*expand_env_in_str(char *src, t_minishell *mini);
char		*extract_metachar(t_minishell *mini);
char		*get_next_token_part(t_minishell *mini);
char		*extract_quoted_token(t_minishell *mini);
char		*extract_complex_token(t_minishell *mini);
char		*extract_word(t_minishell *mini);
char		*extract_token(t_minishell *mini);
char		*env_value(char const *name, t_env *env);
char		*get_prompt(void);
char		**copy_env(char **env);
char		**env_to_array(t_env *env_list);
char		*join_free(char *s1, char *s2);
char		*trim_whitespace(char *s);
char		*strip_outer_parentheses(char *s, int *removed);
char		*join_with_marker(char *s1, char *s2);
const char	*init_class(const char *p, char c, int *negate, int *matched);
const char	*process_class_content(const char *p, char c, int *matched);
t_env		*create_new_node(char *name, char *value, int exported);
t_env		*find_env(t_env *env_list, char const *name);
t_env		*create_env_list(char **envp, t_minishell *mini);
t_token		*check_expansion(t_minishell *mini, char *val);
t_token		*add_token(t_minishell *mini, char *value);
t_redir		*init_redir(int type, char const *filename);
t_pipex		*init_pipex(void);
t_command	*init_new_command(void);
t_command	*parse_commands(t_minishell *mini);
t_minishell	init_minishell(void);

#endif