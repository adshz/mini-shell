#include "./echo.h"

void copy_command_word(char *result, const char *value, t_expansion_state *state)
{
    size_t k = 0;
    
    // Skip leading quotes and spaces
    while (value[k] && (value[k] == '"' || value[k] == '\'' || ft_isspace(value[k])))
        k++;
    
    // Copy the command word
    while (value[k] && !ft_isspace(value[k]) && value[k] != '"' && value[k] != '\'')
        result[state->j++] = value[k++];
    
    // Skip any spaces or quotes between command and args
    while (value[k] && (value[k] == '"' || value[k] == '\'' || ft_isspace(value[k])))
        k++;
    
    // If there are arguments, add a space and copy them
    if (value[k])
    {
        result[state->j++] = ' ';
        while (value[k])
        {
            if (value[k] != '"' && value[k] != '\'')
            {
                if (ft_isspace(value[k]))
                {
                    if (!state->prev_was_space)
                    {
                        result[state->j++] = ' ';
                        state->prev_was_space = 1;
                    }
                }
                else
                {
                    result[state->j++] = value[k];
                    state->prev_was_space = 0;
                }
            }
            k++;
        }
    }
}

void copy_command_args(char *result, const char *value, t_expansion_state *state)
{
    size_t k = 0;
    
    while (value[k])
    {
        if (value[k] != '"' && value[k] != '\'')
        {
            if (ft_isspace(value[k]))
            {
                if (!state->prev_was_space)
                {
                    result[state->j++] = ' ';
                    state->prev_was_space = 1;
                }
            }
            else
            {
                result[state->j++] = value[k];
                state->prev_was_space = 0;
            }
        }
        k++;
    }
}

void copy_variable_value(char *result, const char *value, t_expansion_state *state)
{
    size_t k = 0;
    
    if (value[k] == '"' || value[k] == '\'')
        k++;
    while (value[k])
    {
        if ((value[k] == '"' || value[k] == '\'') && k == ft_strlen(value) - 1)
            break;
        if (state->in_double_quotes || state->in_single_quotes)
            result[state->j++] = value[k];
        else if (ft_isspace(value[k]))
        {
            if (!state->prev_was_space)
            {
                result[state->j++] = ' ';
                state->prev_was_space = 1;
            }
        }
        else
        {
            result[state->j++] = value[k];
            state->prev_was_space = 0;
        }
        k++;
    }
} 