/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shell.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: szhong <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/08 14:44:07 by szhong            #+#    #+#             */
/*   Updated: 2024/11/08 14:45:00 by szhong           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#ifndef SHELL_H
# define SHELL_H

// TODO
typedefenum {
    ERR_NONE,
    ERR_FILE,      // For file operations
    ERR_MEMORY     // For malloc fails
} t_error_type;

// TODO
//void  handle_error(t_error_type type, const char *msg);


#endif
