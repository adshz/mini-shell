/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor_type.h                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: szhong <szhong@student.42london.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/26 21:48:31 by szhong            #+#    #+#             */
/*   Updated: 2025/02/26 21:49:44 by szhong           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#ifndef EXECUTOR_TYPE_H
# define EXECUTOR_TYPE_H

# define WRITE_END 1
# define READ_END 0

typedef enum e_pipe_role
{
	PIPE_WRITER,
	PIPE_READER
}	t_pipe_role

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
	char		*prob;
}	t_err;

// going to executor_types.h
typedef struct s_path
{
	t_err	err;
	char	*path;
}	t_path;
#endif
