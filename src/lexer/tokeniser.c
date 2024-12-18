/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokeniser.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: szhong <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/18 15:51:23 by szhong            #+#    #+#             */
/*   Updated: 2024/12/18 15:51:23 by szhong           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "shell.h"

static int is_whitespace(char c)
{
    return (c == ' ' || c == '\t' || c == '\n');
}

static int is_special_char(char c)
{
    return (c == '|' || c == '<' || c == '>' || c == ';' || c == '&');
}

t_token *create_token(t_token_type type, const char *value)
{
    t_token *token;

    token = (t_token *)malloc(sizeof(t_token));
    if (!token)
        return (NULL);
    token->type = type;
    token->value = ft_strdup(value);
    if (!token->value)
    {
        free(token);
        return (NULL);
    }
    token->next = NULL;
    return (token);
}

static t_token_type get_token_type(const char *str)
{
    if (!str)
        return (TOKEN_EOF);
    if (str[0] == '|' && str[1] == '|')
        return (TOKEN_OR);
    if (str[0] == '&' && str[1] == '&')
        return (TOKEN_AND);
    if (str[0] == '|')
        return (TOKEN_PIPE);
    if (str[0] == '<' && str[1] == '<')
        return (TOKEN_HEREDOC);
    if (str[0] == '>' && str[1] == '>')
        return (TOKEN_APPEND);
    if (str[0] == '<')
        return (TOKEN_REDIRECT_IN);
    if (str[0] == '>')
        return (TOKEN_REDIRECT_OUT);
    if (str[0] == ';')
        return (TOKEN_SEMICOLON);
    if (str[0] == '\n')
        return (TOKEN_NEWLINE);
    return (TOKEN_WORD);
}

static size_t get_token_length(const char *input)
{
    size_t len;

    len = 0;
    if (is_special_char(input[0]))
    {
        if ((input[0] == '|' && input[1] == '|') ||
            (input[0] == '&' && input[1] == '&') ||
            (input[0] == '<' && input[1] == '<') ||
            (input[0] == '>' && input[1] == '>'))
            return (2);
        return (1);
    }
    while (input[len] && !is_whitespace(input[len]) && !is_special_char(input[len]))
        len++;
    return (len);
}

void free_tokens(t_token *tokens)
{
    t_token *next;

    while (tokens)
    {
        next = tokens->next;
        free(tokens->value);
        free(tokens);
        tokens = next;
    }
}

t_token *tokenise(const char *input)
{
    t_token *head;
    t_token *current;
    size_t  len;
    char    *value;

    head = NULL;
    current = NULL;
    while (input && *input)
    {
        // Skip whitespace
        while (*input && is_whitespace(*input))
            input++;
        if (!*input)
            break;

        // Get token length and create token
        len = get_token_length(input);
        value = ft_substr(input, 0, len);
        if (!value)
        {
            free_tokens(head);
            return (NULL);
        }

        // Create and link new token
        if (!head)
        {
            head = create_token(get_token_type(input), value);
            current = head;
        }
        else
        {
            current->next = create_token(get_token_type(input), value);
            current = current->next;
        }
        if (!current)
        {
            free_tokens(head);
            return (NULL);
        }

        // Move to next token
        input += len;
    }
    return (head);
}