/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_redirection.h                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: szhong <szhong@student.42london.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/19 18:45:57 by szhong            #+#    #+#             */
/*   Updated: 2025/02/19 18:46:20 by szhong           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#ifndef PARSER_REDIRECTION_H
# define PARSER_REDIRECTION_H

t_ast_node	*process_consecutive_redirections(t_ast_node *node, \
											t_token **tokens, \
											t_shell *shell);
t_ast_node	*process_redirection(t_token *current, t_ast_node *result);

#endif
