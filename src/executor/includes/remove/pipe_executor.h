/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe_executor.h                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: szhong <szhong@student.42london.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/18 02:15:40 by szhong            #+#    #+#             */
/*   Updated: 2025/02/18 02:15:43 by szhong           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#ifndef PIPE_EXECUTOR_H
# define PIPE_EXECUTOR_H
/* Pipe execution */
int		execute_pipe(t_shell *shell, t_ast_node *node);
void	close_pipe(int pipefd[2]);
int		setup_pipe(int pipe_fd[2]);
/* Child process handling */
int		create_left_child(t_shell *shell, t_ast_node *node,
			int pipe_fd[2], pid_t *pid);
int		create_right_child(t_shell *shell, t_ast_node *node,
			int pipe_fd[2], pid_t *pid);
int		create_pipe_children(t_shell *shell, t_ast_node *node,
			int pipe_fd[2], pid_t *pids);
void	wait_for_children(pid_t *pids, int *statuses);
/* Child process execution */
void	execute_left_child(t_shell *shell, t_ast_node *node, int pipe_fd[2]);
void	execute_right_child(t_shell *shell, t_ast_node *node, \
						int pipe_fd[2]);
int		execute_left_process(t_shell *shell, t_ast_node *node, \
						int pipe_fd[2]);
int		execute_right_process(t_shell *shell, t_ast_node *node, \
						int pipe_fd[2], int *right_status);
/* Error handling */
int		handle_fork_error(int pipe_fd[2]);
int		handle_child_exit_status(int left_status, int right_status);

#endif 
