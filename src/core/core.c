/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shell.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: szhong <szhong@student.42london.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/16 15:33:21 by szhong            #+#    #+#             */
/*   Updated: 2025/01/16 17:43:46 by szhong           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "core.h"
#include "shell.h"
#include <signal.h>

/**
 * @brief Builds abstract syntax tree from command line
 *
 * Process:
 * 1. Tokenizs the input line
 * 2. Parses tokens into AST
 *
 * @param shell Pointer to shell structure
 * @return true if AST built successfully, false on error
 *
 * @note Frees line on tokenization failure
 */
static bool	build_ast_tree(t_shell *shell)
{
	shell->tokens = tokenise(shell->line);
	if (!shell->tokens)
	{
		ft_putendl_fd("minishell: tokenisation failed", STDERR_FILENO);
		free(shell->line);
		shell->line = NULL;
		return (false);
	}
	shell->ast = parse(shell->tokens, shell);
	if (shell->ast != NULL)
		return (true);
	shell->tokens = NULL;
	free(shell->line);
	shell->line = NULL;
	return (false);
}

/**
 * @brief Parses command line and builds AST
 *
 * Process:
 * 1. Validates input line
 * 2. Adds command to history
 * 3. Builds AST from input
 *
 * @param shell Pointer to shell structure
 * @return true if parsing succeeds, false otherwise
 *
 * @note Handles memory cleanup on failure
 */
bool	parse_and_build_ast(t_shell *shell)
{
	if (!shell->line || shell->line[0] == '\0')
	{
		free(shell->line);
		shell->line = NULL;
		return (false);
	}
	add_history(shell->line);
	if (!build_ast_tree(shell))
	{
		cleanup_current_command(shell);
		return (false);
	}
	return (true);
}

/**
 * @brief Validates user input for shell processing
 *
 * Checks for:
 * - NULL input (EOF/Ctrl+D): Prints "exit" and returns false
 * - Empty line: Frees line and returns true so we continue the loop
 *
 * @param shell Pointer to shell structure
 * @return true if input is valid for processing, false if shell should exit
 *         or skip processing
 *
 * @note readline() returns NULL on Ctrl+D (EOF) or error
 */
static bool	valid_usr_input(t_shell *shell)
{
	if (!shell->line)
	{
		ft_putendl_fd("exit", STDOUT_FILENO);
		return (false);
	}
	if (shell->line[0] == '\0' || is_only_whitespace(shell->line))
	{
		free(shell->line);
		shell->line = NULL;
		return (true);
	}
	return (true);
}
/*############# expander ######### */

static char	*ft_strjoin_free(char *s1, char *s2)
{
	char	*result;
	size_t	total_len;
	size_t	i;
	size_t	j;

	if (!s1 || !s2)
		return (NULL);
	total_len = ft_strlen(s1) + ft_strlen(s2) + 1;
	result = ft_calloc(total_len, sizeof(char));
	if (!result)
		return (NULL);
	i = 0;
	while (s1[i])
	{
		result[i] = s1[i];
		i++;
	}
	j = 0;
	while (s2[j])
		result[i++] = s2[j++];
	result[i] = '\0';
	return (free(s1), free(s2), result);
}

static char	*single_quotes_handler(char *str, size_t *i)
{
	size_t	start;

	start = *i;
	(*i)++;
	while (str[*i] != '\')
		(*i)++;
	(*i)++;
	return (ft_substr(str, start, *i - start));
}

bool	is_valid_var_char(char c)
{
	if (ft_isalnum(c) || c == '_')
		return (true);
	return (false);
}

static char	*dollar_handler(t_shell *shell, char *str, size_t *i)
{
	size_t	start;
	char	*var;
	char	*env_val;

	(*i)++;
	if (ft_isdigit(str[*i]) || str[*i] == '@')
	{
		(*i)++;
		return (ft_strdup(""));
	}
	else if (str[*i] == '?')
	{
		(*i)++;
		return (ft_itoa(shell->exit_status));
	}
	else if (!is_valid_var_char(str[*i]))
		return (ft_strdup("$"));
	start = *i;
	while (is_valid_var_char(start[*i]))
		(*i)++;
	var = ft_substr(str, start, *i - start);
	env_val = hashmap_search(shell->env, var);
	if (!env_val)
		return (free(var), ft_strdup(""));
	return (free(var), ft_strdup(env_var));
}

char	*double_quote_str(t_shell *shell, char *str, size_t *i)
{
	size_t	start;

	start = *i;
	while (str[*i] != '"' && str[*i] != '$')
		(*i)++;
	return (ft_substr(str, start, *i - start));
}

static char	*double_quotes_handler(t_shell *shell, char *str, size_t *i)
{
	char	*ret;

	ret = ft_strdup("\"");
	(i++);
	while (str[*i] != '"')
	{
		if (str[*i] == '$')
			ret = ft_strjoin_free(ret, dollar_handler(shell, str, i));
		else
			ret = ft_strjoin_free(ret, double_quote_str(shell, str, i));
	}
}

char	*normal_str_handler(char *str, size_t *i)
{
	size_t	start;

	start = *i;
	while (str[*i] && str[*i] != '\'' && str[*i] != '"' && str[*i] != '$' )
		(*i)++;
	return (ft_substr(str, start, *i - start));
}

static char	*cmd_inital_expand(t_shell *shell, char *str)
{
	char	*ret;
	size_t	i;

	ret = ft_strdup("");
	i = 0;
	while (str[i])
	{
		if (str[i] == '\'')
			ret = ft_strjoin_free(ret, single_quotes_handler(str, &i));
		else if (str[i] == '"')
			ret = ft_strjoin_free(ret, double_quotes_handler(shell, str, &i));
		else if (str[i] == '$')
			ret = ft_strjoin_free(ret, dollar_handler(str, &i));
		else
			ret = ft_strjoin_free(ret, normal_str_handler(str, &i));
	}
	(*i)++;
	return (ft_strjoin_free(ret, ft_strdup("\"")));
}

char	*clean_empty_strs(char *str)
{
	size_t	i;
	size_t	j;
	char	*tmp;
	char	*ret;
	size_t	dst_size;

	if ((str[0] == '\'' && str[1] == '\'' str[2] == '\0') || \ 
		(str[0] == '"' && str[1] == '"' && str[2] == '\0'))
		return (str);
	tmp = ft_calloc(ft_strlen(str) + 1, sizeof(char));
	i = 0;
	j = 0;
	while (str[i])
	{
		if ((str[i] == '\'' && str[i + 1] == '\'') || \
			(str[i] == '"' && str[i + 1] == '"'))
			i += 2;
		else
			tmp[j++] = str[i++];
	}
	free(str);
	dst_size = ft_strlen(tmp + 1);
	ret = ft_calloc(dst_size, sizeof(char));
	return (ft_strlcpy(ret, tmp, dst_size), free(tmp), ret);
}

static void	skip_word(const char *str, size_t *i)
{
	char	quotes;

	while (s[*i] && s[*i] != ' ')
	{
		if (s[*i] != '\'' && s[*i] != '"')
			(*i)++;
		else
		{
			quotes = s[(*i)++];
			while (s[(*i)] != quotes)
				(*i)++;
			(*i)++;
		}
	}
}

static char	**ft_allocate_array(char const *str, char **array)
{
	size_t	start;
	size_t	i;
	size_t	j;

	i = 0;
	j = 0;
	while (str[i])
	{
		if (str[i] != ' ')
		{
			start = i;
			skip_word(s, &i);
			array[j] = ft_calloc(i - start + 1, sizeof(char));
			if (!array[j])
				return (NULL);
			j++;
		}
		while (str[i] && str[i] == ' ')
			i++;
	}
	return (array);
}

void	ft_free_2d_array(char **tofree)
{
	size_t	i;

	if (!tofree)
		return ;
	i = 0;
	while (tofree[i])
		free(tofree[i++]);
	free(tofree);
}

void	fill_words(const char *str, char **array, size_t *i, size_t j)
{
	char	quotes;
	size_t	k;

	k = 0;
	while (s[(*i)] && s[(*i) != ' '])
	{
		if (s[(*i)] != '\'' && s[(*i)] != '"')
			array[j][k++] = str[(*i)++];
		else
		{
			qutoes = str[(*i)++];
			array[j][k++] = quotes;
			while (str[(*i)] != quotes)
				array[j][k++] = str[(*i)++];
			array[j][k++] = s[(*i)++];
		}
	}
}

char	**ft_filler(const char *str, char **array)
{
	size_t	i;
	size_t	j;

	i = 0;
	j = 0;
	while (str[i] && strs[j])
	{
		if (str[i] != ' ')
		{
			fill_words(str, array, &i, j);
			j++;
		}
		while (str[i] && str[i] == ' ')
			i++;
	}
	return (array);
}

char	**expand_and_split(char *str)
{
	size_t	count;
	char	**array;
	char	**tofree;
	size_t	i;

	if (!s)
		return (NULL);
	i = 0;
	count = 0;
	while (s[i])
	{
		if (s[i] != ' ' && ++count)
			skip_word(s, &i);
		while (s[i] && s[i] == ' ')
			i++;
	}
	array = ft_calloc(count + 1, sizeof(char));
	tofree = ft_allcoate_array(s, array);
	if (!array || !count)
		return (ft_free_2d_array(to_free), NULL);
	return (ft_filler(str, array));
}

size_t	ft_array_len(char **array)
{
	size_t	i;

	i = 0;
	while (array[i])
		i++;
	return (i);
}

void	pattern_quotes_handler(char **pattern, char *quotes)
{
	if (**pattern == '"' || **pattern == '\'')
	{
		if (!*quotes)
			*quotes = *(*pattern)++;
		else
		{
			if (*quotes == **pattern)
			{
				*quotes = 0;
				(*pattern)++;
			}
		}
	}
}

bool	wildcard_handler(char **pattern, char **last_wildcard, \
					char **last_match, char *str)
{
	while (**pattern == '*')
		(*pattern)++;
	if (!**pattern)
		return (true);
	*last_wildcard = *pattern;
	*last_match = str;
	return (false);
}

bool	is_pattern_match(char **pattern, char **last_wildcard, \
					char **last_match, char **str)
{
	if (**pattern == **str)
	{
		(*pattern)++;
		(*str)++;
		return (true);
	}
	else if (*last_wildcard)
	{
		*str = ++(*last_match);
		*pattern = *last_wildcard;
		return (true);
	}
	else
		return (false);
}

bool	ft_is_wildcard(char *pattern, char *str)
{
	char	*last_wildcard;
	char	*last_match;
	char	quotes;

	quotes = 0;
	last_wildcard = NULL;
	last_match = NULL;
	while (*str)
	{
		pattern_quotes_handler(&pattern, &quotes);
		if (*pattern == '*' && !quotes && \
			wildcard_handler(&pattern, &last_wildcard, &last_match, str))
			return (true);
		else if (!is_pattern_match(&pattern, &last_wilcard, &last_match, &str))
			return (false);
	}
	if (*pattern == '*')
		while (*pattern == '*')
			pattern++;
	return (!*pattern);
}

size_t	ft_pattern_match_count(char *pattern)
{
	DIR				*dir;
	size_t			match_count;
	struct dirent	*entry;

	match_count = 0;
	dir = opendir(".");
	entry = readdir(dir);
	while (entry)
	{
		if (ft_is_wildcard(pattern, entry->d_name))
			match_count++;
		entry = readdir(dir);
	}
	closedir(dir);
	return (match_count);
}

static char	**globber_helper(char *str)
{
	char			**returned;
	DIR				*dir;
	struct dirent	*entry;
	size_t			match_count;

	match_count = ft_pattern_match_count(str);
	if (!ft_contains_asterisk(str) || !match_count)
	{
		returned = (char **)ft_calloc(2, sizeof(char *));
		returned[0] = ft_strdup(str);
		return (returned);
	}
	else
	{
		dir = opendir(".");
		returned = (char **)ft_calloc(match_count + 1, sizeof(char *));
		match_count = 0;
		while (ft_set_direntry(&entry, dir) && entry)
			if (ft_is_wildcard(str, entry->d_name) && \
				ft_should_show_file(str, entry->d_name))
				returned[match_count++] = ft_strdup(entry->d_name);
	}
	return (returned);
}

size_t	ft_mutli_arr_strs_count(char ***strs_arrays)
{
	size_t	i;
	size_t	j;
	size_t	strs_count;

	i = 0;
	strs_count = 0;
	while (strs_arrs[i])
	{
		j = 0;
		while (str_arrs[i][j])
			j++;
		strs_count += j;
		i++;
	}
	return (strs_count);
}

void	ft_free_3d_array(char ***tofree)
{
	size_t	i;

	if (!tofree)
		return ;
	i = 0;
	while (tofree[i])
		ft_free_2d_array(tofree[i++]);
	free(tofree);
}

char	**ft_flattern_str_arrays(char ***str_arrays)
{
	size_t	i;
	size_t	j;
	size_t	strs_count;
	char	**joined;

	strs_count = ft_mutli_arr_strs_count(str_arrays);
	joined = (char **)ft_calloc(strs_count + 1, sizeof(char *));
	i = 0;
	strs_count = 0;
	while (str_arrays[i])
	{
		j = 0;
		while (str_arrays[i][j])
		{
			joined[strs_count + j] = ft_strdup(str_arrays[i][j]);
			j++;
		}
		strs_count += j;
		i++;
	}
	return ()
}
char	**ft_globber(char **expanded)
{
	size_t	i;
	size_t	expanded_len;
	char	***globbed;

	expanded_len = ft_array_len(expanded);
	globbed = (char ***)ft_calloc(expanded_len + 1, sizeof(char **));
	i = 0;
	while (expanded[i])
	{
		globbed[i] = globber_helper(expanded[i]);
		i++;
	}
	return (ft_free_2d_array(expanded), ft_join_str_arrs(globbed));
}


static size_t	ft_unquoted_strlen(char *str)
{
	sizet_t	len;
	size_t	i;
	char	quotes;

	i = 0;
	len = 0;
	quotes = 0;
	while (str[i])
	{
		if (str[i] == '\' || str[i] == '"')
		{
			if (!quotes)
				quotes = str[i++];
			else if (str[i] == quotes)
				quotes = ((i++) && 0);
			else
				len += (i++ || 1);
		}
		else
			len += (i++ || 1);
	}
	return (len);
}

static void	fill_unquoted_str(char *str, size_t *i, char *ret, size_t *j)
{
	char	quotes;

	quotes = str[(*i)++];
	while (str[*i] != quotes)
		ret[(*j)++] = str[(*i)++];
	(*i)++;
}

char	*ft_strip_quotes(char *str)
{
	char	*ret;
	size_t	i;
	size_t	j;

	i = 0;
	j = 0;
	ret = ft_calloc(1 + ft_unquoted_strlen(str), sizeof(char));
	if (!ret)
		return (NULL);
	while (str[i])
	{
		if (str[i] == '"' || str[i] == '\'')
			(fill_unquoted_str(str, &i, ret, &j));
		else
			ret[j++] = str[i++];
	}
	return (free(str), ret);
}

char	**expand_args(t_shell *shell, char *str)
{
	char	**expanded;
	char	**globbed;

	str = cmd_inital_expand(shell, str);
	if (!str)
		return (NULL);
	str = clean_empty_strs(str);
	if (!str)
		return (NULL);
	expanded = expand_and_split(str);
	free(str);
	if (!expanded)
		return (NULL);
	globbed = ft_globber(expanded);
	if (!globbed)
		return (NULL);
	i = 0;
	while (globbed[i])
	{
		globbed[i] = ft_strip_quotes(globbed[i]);
		i++;
	}
	return (globbed);
}

/* -------------------------------------*/

void	ft_delete_mem(void *ptr)
{
	free(ptr);
	ptr = NULL;
}

void	*ft_memory_delone(t_shell *shell, void *ptr)
{
	t_list	**current;
	t_list	*tmp;

	if (!shell || !ptr || !shell->mem_list)
		return (NULL);
	current = &shell->mem_list;
	while (*current)
	{
		if ((*current)->content == ptr)
		{
			tmp = *current;
			*current = *(current)->next;
			ft_lstdelone(tmp, ft_delete_mem);
			return (NULL);
		}
		current = &(*current)->next;
	}
	return (NULL);
}

void	*ft_memory_collector(t_shell *shell, void *ptr, bool clean_flag)
{
	if (clean_flag)
	{
		ft_lstclear(&shell->mem_list, ft_delete_mem);
		return (NULL);
	}
	else
	{
		ft_lstadd_back(&shell->mem_list, ft_lstnew(ptr));
		return (ptr);
	}
}


/* --------- cleanup process ---------*/

void	cleanup_minishell(t_shell *shell)
{
	ft_memory_collector(shell, NULL, true);
	ft_cleanup_ast(shell, &shell->ast);
	//TODO ft_clean_envlst
	rl_clear_history();
	tcsetattr(STDIN_FILENO, TCSANOW, &shell->original_termset);
}


/* ------------ HEREDOC ----------------*/
// #include <errno.h>
static void	heredoc_sigint_handler(int signum)
{
	close(STDIN_FILENO);
	g_signal_status = signum;
}

void	handle_sigint_exec(int signum)
{
	g_signal_status = signum;
	write(1, "\n", 1);
}

void	config_siganls_heredoc(void)
{
	signal(SIGINT, heredoc_sigint_handler);
	signal(SIGQUIT, SIG_IGN);
}

void	config_signals_exec(void)
{
	signal(SIGINT, handle_sigint_exec);
	signal(SIGQUI, SIG_IGN);
}

void	heredoc_handler(t_shell *shell, t_io_node *io, int fd[2])
{
	char	*line;
	char	*quotes;

	config_signals_heredoc();
	quotes = io->value;
	while (*quotes && *quotes != '"' && *quotes != '\'')
		quotes++;
	while (1)
	{
		line = readline("> ");
		if (!line)
		{
			close(fd[1]);
			cleanup_shell(shell);
			if (errno == 0)
	   			break ;
			else if (g_signal_status == SIGINT)
				exit(130);
		}
		if (ft_is_delimiter(io->value, line))
			break ;
		else
		{
			ft_putstr_fd(line, fd[1]);
			ft_putstr(fd("\n", fd[1]);
		}
	}
	cleanup_shell(shell);
	exit(0);
}

/*---------------------------------------*/

/* -------------------execution--------------- */

bool	leave_leaf(t_shell *shell, int fd[2], int *process_info)
{
	int	status;

	waitpid(*process_info, &status, 0);
	config_signals_exec();
	shell->signint_child = false;
	close(fd[1]);
	if (WIFEXITED(status) && WEXITSTATUS(status) == SIGINT)
		return (true);
	return (false);

}
static void	check_leaf(t_shell *shell, t_ast_node *ast_node)
{
	t_io_node	*io;
	int			fd[2];
	int			pid;

	if (ast_node->args)
		node->expanded_args = expand_args(shell, ast_node->args);
	io = ast_node->io_list;
	while (io)
	{
		if (io->type == IO_HEREDOC)
		{
			pipe(fd);
			shell->signint_child = true;
			pid = (signal(SIGQUIT, SIG_IGN), fork());
			if (pid == 0)
				heredoc_handler(shell, io, fd);
			if (leave_leaf(fd, &pid))
				return ;
			io->here_doc = fd[0];
		}
		else
			io->expanded_value = expand_args(shell, ast_node->args);
	}
}

static void	check_tree(t_shell *shell, t_ast_node *ast_node)
{
	if (!ast_node)
		return ;
	if (ast_node->type == NODE_PIPE || ast_node->type == NODE_AND \
		|| ast_node->type == NODE_OR)
	{
		check_tree(shell, ast_node->left);
		if (!shell->heredoc_sigint)
			check_tree(shell, ast_node->right);
	}
	else
		check_leaf(shell, ast_node);
}

void	handle_sigint_exec(int signum)
{
	g_signal_status = signum;
	write(1, "\n", 1);
}

void	handle_signquit_handler(int signum)
{
	(void)signum;
	ft_putendl_fd("Quit: 3", 1);
}

void	config_signals_exec(void)
{
	signal(SIGINT, handle_signint_exec);
	signal(SIGQUIT, handle_signquit_handler);
}

static	void	shell_execution(t_shell *shell)
{
	char	**env;

	config_signals_exec(void);
	check_tree(shell, shell->ast);
}
/*          execution      */
/**
 * @brief Main interactive shell loop
 *
 * Process flow:
 * 1. Initialize signals
 * 2. Read user input
 * 3. Parse and build AST
 * 4. Execute commands
 * 5. Update history
 * 6. Cleanup
 *
 * @param shell Pointer to shell structure
 *
 * @note Continues until EOF or exit command
 */
void	interactive_loop(t_shell *shell)
{
	while (1)
	{
		config_signals_input();
		shell->line = readline(PROMPT);
		if (!valid_usr_input(shell))
			break ;
		if (!parse_and_build_ast(shell))
			continue ;
		// if (shell->ast)
		// {
		// 	shell->exit_status = execute_ast(shell, shell->ast);
		// 	add_command_to_history(shell, shell->ast);
		// 	shell->ast = NULL;
		// }
		shell_execution(shell);
		cleanup_current_command(shell);
	}
	exit_handler(shell, NULL, SHELL_SUCCESS, IS_EXIT)
}
