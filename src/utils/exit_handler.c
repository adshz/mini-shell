void	close_fds(t_shell *shell, t_cmd *cmd)
{
	t_cmd	  *node;
	t_list	*curr;

	curr = shell->cmds;
	while (curr != NULL)
	{
		node = curr->content;
		if (node == cmd)
		{
			curr = curr->next;
			continue ;
		}
		if (node->in_fd != STDIN_FILENO)
			close(node->in_fd);
		if (node->out_fd != STDOUT_FILENO)
			close(node->out_fd);
		curr = curr->next;
	}
}

void	free_scmd(void *content)
{
	t_ast_node	*node;

	node = content;
	ft_matrixfree(&node->full_cmd);
	free(node->full_cmd);
	if (node->in_fd != STDIN_FILENO)
		close(node->in_fd);
	if (node->out_fd != STDOUT_FILENO)
		close(node->out_fd);
	free(node);
}

static void	clean_and_exit(t_shell *shell)
{
	if (shell)
	{
		if (shell->env)
			hashmap_free_table(shell->env);
		if (shell->cmds)
			ft_lstclear(&shell->cmds, free_scmd);
		if (shell->pids)
			free(shell->pids);
	}
	exit(shell->exit_status);
}

void	exit_handler(t_shell *shell, char *param, int err, int is_exit)
{
	if (err == FORK_ERR || err == PIPE_ERR || err == HIS_WRONG_ARG
		|| err == HIS_WRONG_ARGS || err == CD_ERR || err == NOT_DIR)
		shell->exit_status = 1;
	else if (err == SYNTAX_ERR)
		shell->exit_status = 2;
	else if (err == CMD_NOT_FOUND)
		shell->exit_status = 127;
	else if (err == IS_DIR)
		shell->exit_status = 126;
	else
		mish->exit_status = err;
	if (err != SUCCESS && err != FAILURE && err != ERROR)
		print_error(param, err);
	if (shell && shell->cmds)
		close_fds(shell, NULL);
	if (is_exit)
		clean_and_exit(shell);
}