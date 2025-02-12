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
# include <readline/readline.h>
# include <readline/history.h>
# include "libft.h"
# include "types.h"
# include "errors.h"
# include "../src/core/core.h"
# include "../src/lexer/lexer.h"
# include "../src/parser/parser.h"
# include "../src/executor/executor.h"
# include "../src/hashtable/hashtable.h"
# include "../src/expander/expander.h"
# include "../src/builtins/builtins.h"
# include "../src/utils/utils.h"

#endif
