/*############# expander ######### */

char	*ft_strjoin_with_char_free_both(char *s1, char *s2, char c)
{
	char	*joined;
	size_t	total_len;
	size_t	i;
	size_t	j;

	if (!s1 || !s2)
    	return (NULL);
	if (!c || !ft_strlen(s1) || !ft_strlen(s2))
    	return (ft_strjoin(s1, s2));
	total_len = ft_strlen(s1) + ft_strlen(s2) + 1 + 1;
	joined = ft_calloc(total_len, sizeof(char));
	if (!joined)
    	return (NULL);
	i = 0;
	while (s1[i])
	{
    	joined[i] = s1[i];
    	i++;
	}
	joined[i++] = c;
	j = 0;
	while (s2[j])
    	joined[i++] = s2[j++];
	joined[i] = '\0';
	return (free(s1), free(s2), joined);
}

static char	*ft_strjoin_free_both(char *s1, char *s2)
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
        	ret = ft_strjoin_free_both(ret, dollar_handler(shell, str, i));
    	else
        	ret = ft_strjoin_free_both(ret, double_quote_str(shell, str, i));
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
        	ret = ft_strjoin_free_both(ret, single_quotes_handler(str, &i));
    	else if (str[i] == '"')
        	ret = ft_strjoin_free_both(ret, double_quotes_handler(shell, str, &i));
    	else if (str[i] == '$')
        	ret = ft_strjoin_free_both(ret, dollar_handler(str, &i));
    	else
        	ret = ft_strjoin_free_both(ret, normal_str_handler(str, &i));
	}
	(*i)++;
	return (ft_strjoin_free_both(ret, ft_strdup("\"")));
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
	DIR            	*dir;
	size_t        	match_count;
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
	char        	**returned;
	DIR            	*dir;
	struct dirent	*entry;
	size_t        	match_count;

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
	return (ft_free_3d_array(str_arrays), joined);
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
	return (ft_free_2d_array(expanded), ft_flattern_str_arrays(globbed));
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



/* -------------main signals-----------*/
static void	main_sigint_handler(int signum)
{
	(void)signum;
	if (g_signal_status == SHELL_STATE_HEREDOC_INPUT)
	{
    	close(STDIN_FILENO);
    	ft_putstr_fd("\n", 1);
    	g_signal_status == SHELL_STATE_HEREDOC_INTERRUPTED;
	}
	else if (g_signal_status == SHELL_STATE_READLINE)
	{
    	ft_putstr_fd("\n", 1);
    	rl_replace_line("", 0);
    	rl_on_new_line();
    	rl_redisplay();
	}
	else if (g_signal_status == SHELL_STATE_EXECUTION)
	{
    	ft_putstr_fd("\n", 1);
	}
}

void	config_main_signals(t_shell *shell)
{
	struct termios	term;

	term = shell->original_term;
	term.c_lflag &= ~ECHOCTL;
	tcsetattr(STDIN_FILENO, TCSANOW, &term);
	signal(SIGINT, main_sigint_handler);
	signal(SIGQUIT, SIG_IGN);
}

/* ------------ HEREDOC ----------------*/
// #include <errno.h>
//  may not need this , because I can use the main_signal_handler
static void	heredoc_sigint_handler(int signum)
{
	close(STDIN_FILENO); // i need to clean it up
	g_signal_status = signum;
}
/*----------- not need ------*/

void	heredoc_sigquit_handler(int signum)
{
	(void)signum;
	ft_putendl_fd("Quit: 3", 1);
}

void	handle_sigint_exec(int signum)
{
	g_signal_status = signum;
	write(1, "\n", 1);
}

/* ---- I don't need this */
// because I let the main_signal_hanlder to deal with it
// the second SIGQUIT, in the parent process there has a setting
// child process will inherent the signal(SIGQUIT, SIG_IGN); from the parent
// process
void	config_signals_heredoc(void)
{
	signal(SIGINT, heredoc_sigint_handler);
	signal(SIGQUIT, SIG_IGN);
}
/* ---- I don't need this above ----*/

/* delete it later */
void	handle_sigint_input(int signum)
{
	g_signal_status = signum;
	ioctl(STDINFILENO, TIOCSTI, "\n");
	rl_replace_line("", 0);
	rl_on_new_line();
}
/* --------- delete it later above -------*/

void	restore_signalhandle_postheredoc(void)
{
	signal(SIGQUIT, heredoc_sigquit_handler)
}

void	config_execution_signals(void)
{
	g_signal_status = SHELL_STATE_EXECUTION;
	signal(SIGQUI, SIG_IGN);
}

bool	is_delimiter(char	*delimiter, char *str)
{
	while (*str)
	{
    	if (*delimiter == '"' || *delimiter == '\'')
    	{
        	delimiter++;
        	continue ;
    	}
    	else if (*str == *delimiter)
    	{
        	str++;
        	delimiter++;
    	}
    	else
        	return (false);
	}
	while (*delimiter == '"' || *delimiter == '\'')
    	delimiter++;
	return (!*delimiter)
}

int    	heredoc_expand_var_to_fd(t_shell *shell, char *str, int fd)
{
	size_t	start;
	char	*tmp;

	start = ++i;
	if (str[i] == '?')
    	return (ft_putnbr_fd(shell->exit_status, fd), 2);
	while (str[i] && str[i] != '$' && str[i] != ' ')
    	i++;
	if (i != start)
	{
    	tmp = ft_memory_collector(shell, ft_substr(str, start, i), false);
    	tmp = hashmap_search(shell->env, tmp);
    	if (tmp)
        	ft_putstr_fd(tmp, fd);
	}
	return (i);
}

void	heredoc_expander(t_shell *shell, char *str, int fd)
{
	size_t	i;

	i = 0;
	while (str[i])
	{
    	if (str[i] == '$')
        	i += heredoc_expand_var_to_fd(shell, str, i, fd);
    	else
        	i += (ft_putchar_fd(str[i], fd), 1);
	}
	ft_putchar_fd('\n', fd);
}

# define HEREDOC_CTRL_C 130
# define CHILD_EXECUTING 142 // may not need it
# define NORMAL_OPERATION 0
# define EXIT_NORMAL 0
# define HEREDOC_CTRL_D 0

void	heredoc_handler(t_shell *shell, t_io_node *io, int fd[2])
{
	char	*line;
	char	*quotes;

	quotes = io->value;
	while (*quotes && *quotes != '"' && *quotes != '\'')
    	quotes++;
	while (1)
	{
    	line = readline("> ");
    	if (!line)
    	{
        	close(fd[WRITE_END]);
        	cleanup_shell(shell);
        	if (errno == HEREDOC_CTRL_D)
               	break ;
        	else if (g_signal_status == SHELL_STATE_HEREDOC_INTERRUPTED)
            	exit(HEREDOC_CTRL_C);
    	}
    	if (is_delimiter(io->value, line))
        	break ;
    	if (!*quotes)
        	heredoc_expander(shell, line, fd[WRITE_END]);
    	else
    	{
        	ft_putstr_fd(line, fd[WRITE_END]);
        	ft_putstr(fd("\n", fd[WRITE_END]);
    	}
	}
	cleanup_shell(shell);
	exit(EXIT_NORMAL);
}

/*---------------------------------------*/

/* -------------------execution--------------- */

//
// command_executor_signals -> exited_normally
bool	should_leave_leaf(t_shell *shell, int fd[2], int *process_info)
{
	int	status;

	waitpid(*process_info, &status, 0);
	// restore_signquit_postheredoc(); // we don't need to restore it because of
	// main_signals
	close(fd[WRITE_END]);
	if (exited_normally(status) && get_exit_status(status) == HEREDOC_CTRL_C)
	{
    	g_signal_status = SHELL_STATE_HEREDOC_INTERRUPTED;
    	close(fd[READ_END]);
    	return (true);
	}
	return (false);

}

// # define WRITE_END 1
// # define READ_END 0
// # define SHELL_STATE_READLINE 1
// # define SHELL_STATE_EXECUTION 2
// # define SHELL_STATE_HEREDOC_INPUT 42
// # define SHELL_STATE_HEREDOC_INTERRUPTED 43
// # define SHELL_STATE_SIGINT 2 i don't think you need that
// # define SHELL_STATE_NORMAL 0

static bool	handle_leaf_heredoc(t_shell *shell, t_ast_node *ast_node)
{
	t_io_node	*io;
	int        	fd[2];
	int        	pid;

	if (ast_node->args)
    	node->expanded_args = expand_args(shell, ast_node->args);
	io = ast_node->io_list;
	while (io)
	{
    	if (io->type == IO_HEREDOC)
    	{
        	pipe(fd);
        	g_signal_status = SHELL_STATE_HEREDOC_INPUT;
        	pid = (signal(SIGQUIT, SIG_IGN), fork());
        	if (pid == 0)
            	heredoc_handler(shell, io, fd);
        	if (should_leave_leaf(fd, &pid))
            	return (true);
        	io->here_doc = fd[READ_END];
    	}
    	else
        	io->expanded_value = expand_args(shell, ast_node->args);
	}
	return (false);
}

static bool	traverse_tree_heredoc(t_shell *shell, t_ast_node *ast_node)
{
	bool	heredoc_interrupted;

	if (!ast_node)
    	return ;
	if (ast_node->type == NODE_PIPE || ast_node->type == NODE_AND \
    	|| ast_node->type == NODE_OR)
	{
    	heredoc_interrupted = traverse_tree_heredoc(shell, ast_node->left);
    	if (!heredoc_interrupted)
        	traverse_tree_heredoc(shell, ast_node->right);
	}
	else
    	handle_leaf_heredoc(shell, ast_node);
    
}

// may delete it later
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

void	config_execution_signals(void)
{
	signal(SIGQUIT, handle_signquit_handler);
}

void	handle_signint_input(int signum)
{
	g_signal_status = signum;


}

void	config_signals_input(void)
{
	signal(SIGNINT, handle_sign_input);
	signal(SIGQUIT, SIG_IGN);
}

/* --------- execution ------------ */
static void	execute_pipe_child(t_shell *shell, t_ast_node *node, \
                        	int pipe_ends[2], t_pipe_role role)
{
	int	child_status;

	if (role == PIPE_WRITER)
	{
    	close(pipe_ends[READ_END]);
    	dup2(pipe_ends[WRITE_END], STDOUT_FILENO);
    	close(pipe_ends[WRITE_END]);
	}
	else if (role == PIPE_READER)
	{
    	close(pipe_ends[WRITE_END]);
    	dup2(pipe_ends[READ_END], STDIN_FILENO);
    	close(pipe_ends[READ_END]);
	}
	child_status = execute_ast_node(node, true);
	(cleanup_shell(shell), exit(child_status));

}
int	exec_get_exit_status(int status)
{
	if (was_signaled(status))
    	return (128 + get_signal_from_status(status));
	return (get_exit_status(status))
}

int	execute_pipeline(t_shell *shell, t_ast_node *ast_tree)
{
	int    	pipe_exit_status;
	int    	pipe_ends[2];
	pid_t	pipe_left;
	pid_t	pipe_right;

	if (pipe(pipe_ends) == -1)
    	return (ERRNO_PIPE_FAILURE);
	pipe_left = fork();
	if (pipe_left == -1)
    	return (close(pipe_ends[WRITE_END]), close(pipe_ends[READ_END]), ERRNO_FORK_FAILURE);    
e	if (pipe_left == 0)
    	execute_pipe_child(shell, ast_tree->left, pipe_ends, PIPE_WRITER);
	else
	{
    	pipe_right = fork();
    	if (pipe_right == -1)
        	return (close(pipe_ends[WRITE_END]), close(pipe_ends[READ_END]), ERRNO_FORK_FAILURE);
    	if (pipe_right == 0)
        	execute_pipe_child(shell, ast_tree->right, pipe_ends, PIPE_READER);
    	else
    	{
        	(close(pipe_ends[WRITE_END]), close(pipe_ends[READ_END]), \
        	waitpid(pipe_left, &pipe_exit_status, 0));
        	g_signal_status = SHELL_STATE_READLINE;
        	return (exec_get_exit_status(pipe_exit_status));
    	}
	}
	return (ERRNO_GENERAL);
}
// check mine

typedef enum e_pipe_role
{
	PIPE_WRITER,
	PIPE_READER
}	t_pipe_role

// # define ERRNO_EMPTY_COMMAND 1
//
typedef enum e_err_no
{
	ERRNO_NONE,
	ERRNO_GENERAL,
	ERRNO_PIPE_FAILURE,
	ERRNO_FORK_FAILURE,
	ERRNO_EXEC_FAILURE,
	ERRNO_CANT_EXEC = 126,
	ERRNO_NOT_FOUND,
	ERRNO_EXEC_255 = 255,
}	t_err_no;


typedef enum e_msg_err
{
	MSGERR_CMD_NOT_FOUND,
	MSGERR_NO_SUCH_FILE,
	MSGERR_PERM_DENIED,
	MSGERR_AMBIGUOUS,
	MSGERR_TOO_MANY_ARGS,
	MSGERR_NUMERIC_REQUI
}	t_msg_err;

typedef struct s_err
{
	t_err_no	num;
	t_msg_err	msg;
	char    	*prob;
}	t_err;

//ft_err_msg
int	exec_print_err(t_err err)
{
	if (err.msg == MSGERR_CMD_NOT_FOUND)
    	return (ft_putstr_fd("minishell: ", 2), ft_putstr_fd(err.prob, 2), \
    	ft_putstr_fd(": command not found\n", 2), err.no);
	else if (err.msg == MSGERR_NO_SUCH_FILE)
    	return (ft_putstr_fd("minishell: ", 2), ft_putstr_fd(err.prob, 2), \
    	ft_putstr_fd(": no such file or directory\n", 2), err.no);
	else if (err.msg == MSGERR_PERM_DENIED)
    	return (ft_putstr_fd("minishell: ", 2), ft_putstr_fd(err.prob, 2), \
    	ft_putstr_fd(": Permission denied\n", 2), erro.no);
	else if (err.msg == MSGERR_AMBIGUOUS)
    	return (ft_putstr_fd("minishell: ", 2), ft_putstr_fd(err.prob, 2), \
    	ft_putstr_fd(": ambiguous redirect\n", 2), err.no);
	else if (err.msg == MSGERR_TOO_MANY_ARGS)
    	return (ft_putstr_fd("minishell: exit: too many argument\n", 2), err.no);
	else if (err.msg == MSGERR_NUMERIC_REQUI)
    	return (ft_putstr_str("minishell: ", 2), ft_putstr_fd(err.prob, 2), \
    	ft_putstr_fd("minishell: exit ", 2), ft_putstr_fd(err.prob, 2), \
    	ft_putstr_fd(": numeric argument required\n"), err.no);
	return (0);
}

t_err	ft_validate_write_perm(char *file)
{
	if (!*file)
    	return ((t_err){ERRNO_GENERAL, MSGERR_NO_SUCH_FILE, file});
	if (access(file, F_OK) == 0)
	{
    	if (access(file, W_OK) == -1)
        	return ((t_err){ERRNO_GENERAL, MSGERR_PERM_DENIED, file});
    	return ((t_err){ERRNO_NONE, 42, NULL});
	}
	return ((t_err){ERRNO_NOT_FOUND, MSGERR_NO_SUCH_FILE,, file});
}

int	ft_handle_output_redirection(t_io_node io_lst, int *status)
{
	int	fd;

	if (!io_lst->expanded_value || io_lst->expanded_value[1])
	{
    	*status = exec_print_err(\
                    	(t_err){ERRNO_GENERAL, MSGERR_AMBIGUOUS, \
                    	io_lst->value});
    	return (*status);
	}
	fd = open(io_lst->expanded_value[0], O_CREAT | O_WRONLY | O_TRUNC, 0644);
	if (fd == -1)
	{
    	*status = exec_print_err(\
                    	ft_validate_write_perm(io_lst->expanded_value[0]));
    	return (*status);
	}
	dup2(fd, STDOUT_FILENO);
	close(fd);
	*status = 0;
	return (*status);
}

t_err	ft_validate_read_perm(char *file)
{
	if (!*file)
    	return ((t_err){ERRNO_GENERAL, MSGERR_PER_DENIED, file});
	if (access(file, F_OK) == 0)
	{
    	if (access(file, R_OK) == -1)
        	return ((t_err){ERRNO_GENERAL, MSGERR_PER_DENIED, file});
    	return ((t_err){ERRNO_NONE, 42, NULL});
	}
	return ((t_err){ERRNO_NOT_FOUND, MSGERR_NO_SUCH_FILE, file});
}

int	ft_handle_input_redirection(t_io_node *io_lst, int *status)
{
	int	fd;

	if (!io_lst->expaned_value || io_lst->expaned_value[1])
	{
    	*status = exec_print_err((t_err){ERRNO_GENERAL, MSGERR_AMBIGUOUS,
        	\io_lst->value});
    	return (*status);
	}
	fd = open(io_lst->expanded_value[0], O_RDONLY);
	if (fd == -1)
	{
    	*status = exec_print_err(\
                    	ft_validate_read_perm(io_lst->expanded_value[0]));
    	return (*status);
	}
	dup2(fd, STDIN_FILENO);
	close(fd);
	*status = 0;
	return (*status);
}

int	ft_handle_append_redirection(t_io_node *io_lst, int *status)
{
	int	fd;

	if (!io->lst->expanded_value || io_lst->expanded_value[1])
	{
    	*status = exec_print_err(\
                    	(t_err){ERRNO_GENERAL, MSGERR_AMBIGUOUS, \
                    	io_lst->value});
    	return (*status);
	}
	fd = open(io_lst->expanded_value[0], O_CREATE | O_WRONLY | O_APPEND, 0644);
	if (fd == -1)
	{
    	*status = exec_print_err(\
                       	ft_validate_write_perm(io_lst->expanded_value[0]));
    	return (*status);
	}
	dup2(fd, STDOUT_FILENO);
	close(fd);
	*status = 0;
	return (*status);
}

int	check_redirection(t_shell *shell, t_ast_node *node)
{
	t_io_node	*current_io;
	int        	status;

	current_io = node->io_list;
	while (current_io)
	{
    	if (current_io->type == IO_OUT && \
        	ft_handle_output_redirection(current_io, &status) != ERRNO_NONE)
        	return (status);
    	else if (current_io->type == IO_IN && \
            	ft_handle_input_redirection(current_io, &status) != ERRNO_NONE)
        	return (status);
    	else if (current_io->type == IO_APPEND && \
            	ft_handle_append_redirection(current_io, &status) != ERRNO_NONE)
        	return (status);
    	else if (current_io->type == IO_HEREDOC)
        	(dup2(current_io->here_doc, STDOUT_FILENO), \
            	close(current_io->here_doc))
    	current_io = current_io->next;
	}
	return (ERRNO_NO);
}

void	reset_stds(t_shell *shell, bool is_pipe)
{
	if (is_pipe)
    	return ;
	dup2(shell->stdin_backup, STDIN_FILENO);
	dup2(shell->stdout_backup, STDOUT_FILENO);
}

typedef struct s_path
{
	t_err	err;
	char	*path;
}	t_path;

t_err	check_executable(char *file, bool is_cmd)
{
	if (!*file)
    	return ((t_err){ERRNO_GENERAL, MSGERR_NO_SUCH_FILE, file});
	if (access(file, F_OK) == 0)
	{
    	if (access(file, X_OK) == -1)
        	return ((t_err){ERRNO_CANT_EXEC, MSGERR_PERM_DENIED, file});
    	return ((t_err){ERRNO_NONE, 42, NULL});
	}
	if (is_cmd)
    	return ((t_err){ERRNO_NOT_FOUND, MSGERR_CMD_NOT_FOUND, file});
	return ((t_err){ERRNO_NOT_FOUND, MSGERR_NO_SUCH_FILE, file})
}

t_path	get_env_path(t_shell *shell, char *path, char *cmd)
{
	size_t	i;
	t_err	err;
	char	*cmd_path;
	char	**split_path;

	split_path = ft_split(path, ':');
	i = 0;
	while (split_path[i])
	{
    	cmd_path = ft_memory_collector(shell, \
                            	ft_strjoin_with_char_free_both(\
                                	ft_strdup(split_path[i]), ft_strdup(cmd),\
                                	'/'), false);
    	err= check_executable(cmd_path, true);
    	if (err.no == ERRNO_NONE)
        	return ((t_path){\
            	(t_err){ERRNO_NONE, 42, cmd}, NULL});
    	i++;
	}
	ft_free_2d_array(split_path);
	return ((t_path){(t_err){ERRNO_NOT_FOUND, MSGERR_CMD_NOT_FOUND, cmd}, \
    	NULL});
}

t_path	get_path(t_shell *shell, char *cmd)
{
	char	*path_env_value;

	if (*cmd == '\0')
    	return ((t_path){ERRNO_NOT_FOUND, MSGERR_CMD_NOT_FOUND, cmd}, NULL);
	if (ft_strnstr(cmd, "/", ft_strlen(cmd)))
    	return ((t_path){check_executable(cmd, false), cmd});
	path_env_value = hashtmap_search(shell->env, "PATH");
	if (path_env_value)
    	return (get_env_path(shell, path_env_value, cmd));
	return ((t_path){(t_err){ERRNO_NOT_FOUND, MSGERR_NO_SUCH_FILE, cmd}, NULL});
}

int	execute_external_cmd(t_shell *shell, t_ast_node *node)
{
	t_path	path_status;
	int    	tmp_status;
	int    	child_pid;

	g_signal_status = SHELL_STATE_EXECUTION;
	child_pid = fork();
	if (child_pid == 0)
	{
    	tmp_status = check_redirection(node);
    	if (tmp_status != ERRNO_NONE)
        	(cleanup_shell(shell), exit(ERRNO_GENERAL));
    	path_status = get_path(shell, (node->expanded_args)[0]);
    	if (path_status.err.no != ERRNO_NONE)
    	{
        	tmp_status = exec_print_err(path_status.err);
        	(cleanup_shell(shell), exit(tmp_status));
    	}
    	if (execve(path_status.path, node->expanded_args, shell->environ) == -1)
        	(cleanup_shell(shell), exit(1));
	}
	waitpid(fork_pid, &tmp_status, 0);
	g_signal_status = SHELL_STATE_NORMAL;
	return (exec_get_exit_status(tmp_status))
}

int	execute_simple_cmd(t_shell *shell, t_ast_node *node, bool is_pipe)
{
	int	exec_status;

	if (!node->expanded_args)
	{
    	exec_status = check_redirection(node);
    	return (reset_stds(shell, is_pipe), (exec_status && ERRNO_GENERAL));
	}
	else if (is_builtin((node->expanded_args)[0]))
	{
    	exec_status = check_redirection(node);
    	if (exec_status != ERRNO_NONE)
        	return (reset_stds(shell, is_pipe), ERRNO_GENERAL);
    	exec_status = execute_builtin(shell, node);
    	return (reset_stds(shell, is_pipe), ERRNO_GENERAL);
	}
	else
    	return (execute_external_cmd(shell, node));
}

int	execute_ast_node(t_shell *shell, t_ast_node *ast_tree, bool is_pipe)
{
	int	cmd_exit_status;

	config_execution_signals(void);
	if (!ast_tree)
    	return (ERRNO_EMPTY_COMMAND);
	if (ast_tree->type == NODE_PIPE)
    	return (execute_pipeline(shell, ast_tree));
	else if (ast_tree->type == NODE_AND)
	{
    	cmd_exit_status = execute_ast_node(shell, ast_tree->left, false);
    	if (cmd_exit_status == ERRNO_NONE)
        	return (execute_ast_node(shell, ast_tree->right, false));
    	return (cmd_exit_status);
	}
	else if (ast_tree->type == NODE_OR)
	{
    	cmd_exit_status = execute_ast_node(shell, ast_tree->left, false);
    	if (cmd_exit_status == ERRNO_NONE)
        	return (cmd_exit_status);
    	return (execute_ast_node(shell, ast_tree->right, false));
	}
	else
    	return (execute_simple_cmd(shell, shell->ast, is_pipe));
}


/*---------- end execution --------- */
static	void	execute_shell_command(t_shell *shell)
{
	char	**env;

	traverse_tree_heredoc(shell, shell->ast);
	if (g_signal_status == SHELL_STATE_HEREDOC_INTERRUPTED)
	{
    	ft_cleanup_ast(shell->ast);
    	g_signal_status == SHELL_STATE_READLINE;
	}
	termsetattr(STDIN_FILENO, TCSANOW, &shell->original_term);
	shell->exit_status = execute_ast_node(shell, shell->ast, false);
}
/*      	execution  	*/




