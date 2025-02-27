/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shell.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: evmouka <evmouka@student.42london.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/21 17:03:45 by szhong            #+#    #+#             */
/*   Updated: 2025/02/12 18:26:43 by evmouka          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SHELL_H
# define SHELL_H

# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>
# include <stdbool.h>
//# include <readline/readline.h>
//# include <readline/history.h>
# include "readline_wrapper.h"
# include "libft.h"
# include "types.h"
# include "errors.h"
# include "core/core.h"
# include "lexer/lexer.h"
# include "parser/parser.h"
# include "executor/executor.h"
# include "hashtable/hashtable.h"
# include "expander/expander.h"
# include "builtins/builtins.h"
# include "utils/utils.h"
# include "signals/ft_signals.h"
# include "memory_collector/memory_collector.h"
#endif
