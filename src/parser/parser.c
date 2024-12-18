/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: szhong <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/18 15:51:59 by szhong            #+#    #+#             */
/*   Updated: 2024/12/18 15:51:59 by szhong           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "parser.h"
#include "libft.h"

t_ast_node *create_ast_node(t_ast_type type, char *value)
{
    t_ast_node *node;

    node = malloc(sizeof(t_ast_node));
    if (!node)
        return (NULL);
    node->type = type;
    node->value = value ? ft_strdup(value) : NULL;
    node->args = NULL;
    node->left = NULL;
    node->right = NULL;
    return (node);
}

void free_ast(t_ast_node *node)
{
    if (!node)
        return;
    free_ast(node->left);
    free_ast(node->right);
    free(node->value);
    if (node->args)
    {
        char **tmp = node->args;
        while (*tmp)
            free(*tmp++);
        free(node->args);
    }
    free(node);
}

t_ast_type get_ast_type(t_token_type type)
{
    if (type == TOKEN_PIPE)
        return (AST_PIPE);
    if (type == TOKEN_REDIRECT_IN)
        return (AST_REDIR_IN);
    if (type == TOKEN_REDIRECT_OUT)
        return (AST_REDIR_OUT);
    if (type == TOKEN_APPEND)
        return (AST_REDIR_APPEND);
    if (type == TOKEN_HEREDOC)
        return (AST_HEREDOC);
    if (type == TOKEN_AND)
        return (AST_AND);
    if (type == TOKEN_OR)
        return (AST_OR);
    return (AST_COMMAND);
}

t_ast_node *parse_command(t_token **tokens)
{
    t_ast_node *node;
    t_token *current;
    int arg_count = 0;
    t_token *start = *tokens;

    // Count arguments
    current = *tokens;
    while (current && current->type == TOKEN_WORD)
    {
        arg_count++;
        current = current->next;
    }

    if (arg_count == 0)
        return NULL;

    // Create command node with first token as command name
    node = create_ast_node(AST_COMMAND, start->value);
    if (!node)
        return NULL;

    // Allocate arguments array (including command name)
    node->args = malloc(sizeof(char *) * (arg_count + 1));
    if (!node->args)
    {
        free_ast(node);
        return NULL;
    }

    // Fill arguments array starting with command name
    current = start;
    for (int i = 0; i < arg_count; i++)
    {
        node->args[i] = ft_strdup(current->value);
        if (!node->args[i])
        {
            // Cleanup on allocation failure
            while (--i >= 0)
                free(node->args[i]);
            free(node->args);
            free_ast(node);
            return NULL;
        }
        current = current->next;
    }
    node->args[arg_count] = NULL;

    // Update tokens pointer to next token after command
    *tokens = current;
    return node;
}

t_ast_node *parse_pipeline(t_token **tokens)
{
    t_ast_node *left;
    t_ast_node *right;
    t_token *current;

    left = parse_command(tokens);
    if (!left)
        return NULL;

    current = *tokens;
    if (!current || current->type != TOKEN_PIPE)
        return left;

    // Skip pipe token
    *tokens = current->next;

    // Parse right side of pipe
    right = parse_pipeline(tokens);
    if (!right)
    {
        free_ast(left);
        return NULL;
    }

    // Create pipe node
    t_ast_node *pipe_node = create_ast_node(get_ast_type(TOKEN_PIPE), NULL);
    if (!pipe_node)
    {
        free_ast(left);
        free_ast(right);
        return NULL;
    }

    pipe_node->left = left;
    pipe_node->right = right;
    return pipe_node;
}

t_ast_node *parse_expression(t_token **tokens)
{
    t_ast_node *left;
    t_ast_node *right;
    t_token *current;

    left = parse_pipeline(tokens);
    if (!left)
        return NULL;

    current = *tokens;
    while (current && (current->type == TOKEN_REDIRECT_IN || 
           current->type == TOKEN_REDIRECT_OUT || 
           current->type == TOKEN_APPEND || 
           current->type == TOKEN_HEREDOC))
    {
        t_token_type redir_type = current->type;
        *tokens = current->next;

        // Get redirection target
        current = *tokens;
        if (!current || current->type != TOKEN_WORD)
        {
            free_ast(left);
            return NULL;
        }

        // Create redirection node
        t_ast_node *redir_node = create_ast_node(get_ast_type(redir_type), NULL);
        if (!redir_node)
        {
            free_ast(left);
            return NULL;
        }

        // Create target node
        right = create_ast_node(AST_COMMAND, current->value);
        if (!right)
        {
            free_ast(left);
            free_ast(redir_node);
            return NULL;
        }

        redir_node->left = left;
        redir_node->right = right;
        left = redir_node;  // Update left for potential chaining

        *tokens = current->next;
        current = *tokens;
    }

    return left;
}

t_ast_node *parse(t_token *tokens)
{
    if (!tokens)
        return NULL;
    return parse_expression(&tokens);
}