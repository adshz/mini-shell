/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   debug_heredoc.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: evmouka <evmouka@student.42london.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/13 16:19:36 by evmouka           #+#    #+#             */
/*   Updated: 2025/02/13 16:23:04 by evmouka          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "executor/executor.h"

void	debug_heredoc_state(t_ast_node *node, const char *line, const char *stage) {
    char	debug_buf[1024];
    int		len;

	len = 0;
	len += snprintf(debug_buf + len, sizeof(debug_buf) - len,
			"\n=== Heredoc Debug (%s) ===\n", stage);
	if (node && node->right && node->right->value) {
		len += snprintf(debug_buf + len, sizeof(debug_buf) - len,
						"Delimiter: '%s'\n", node->right->value);
	} else {
		len += snprintf(debug_buf + len, sizeof(debug_buf) - len,
				"ERROR: Invalid node structure\n");
	}
	if (line) {
		len += snprintf(debug_buf + len, sizeof(debug_buf) - len,
				"Input line: '");
			for (int i = 0; line[i]; i++) {
			if (line[i] >= 32 && line[i] <= 126) {
				len += snprintf(debug_buf + len, sizeof(debug_buf) - len, 
								"%c", line[i]);
			} else {
				len += snprintf(debug_buf + len, sizeof(debug_buf) - len,
						"\\x%02x", (unsigned char)line[i]);
			}
		}
		len += snprintf(debug_buf + len, sizeof(debug_buf) - len, "'\n");
	}
	write(STDERR_FILENO, debug_buf, len);
}
