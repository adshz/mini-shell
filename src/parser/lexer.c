// In your tokenization/lexing phase

static t_token *create_token(char *value, t_token_type type, size_t len) {
    t_token *token = malloc(sizeof(t_token));
    if (!token)
        return NULL;
    
    token->value = value;
    token->type = type;
    token->len = len;
    return token;
}

static char *extract_quoted_string(const char *input, size_t *pos) {
    size_t start = *pos + 1;  // Skip the opening quote
    size_t end = start;
    
    // Find the closing quote
    while (input[end] && input[end] != '\'')
        end++;
        
    if (!input[end]) {
        // Handle unclosed quote error
        return NULL;
    }
    
    // Calculate length without quotes
    size_t len = end - start;
    char *result = malloc(len + 1);
    if (!result)
        return NULL;
        
    // Copy the string without quotes
    ft_strncpy(result, input + start, len);
    result[len] = '\0';
    
    *pos = end + 1;  // Move position past the closing quote
    return result;
}

t_token *get_next_token(const char *input, size_t *pos) {
    while (input[*pos] && ft_isspace(input[*pos]))
        (*pos)++;
        
    if (!input[*pos])
        return NULL;
        
    if (input[*pos] == '\'') {
        char *quoted_str = extract_quoted_string(input, pos);
        if (!quoted_str)
            return NULL;  // Handle error
            
        return create_token(quoted_str, TOKEN_WORD, ft_strlen(quoted_str));
    }
    
    // Handle other token types...
} 