/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_echo.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: szhong <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/18 20:30:39 by szhong            #+#    #+#             */
/*   Updated: 2024/12/18 20:30:39 by szhong           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "builtins.h"
#include "libft.h"
#include "shell.h"
#include "parser.h"
#include "expander.h"
#include <unistd.h>
#include <stdio.h>

void ft_itoa_buf(int n, char *buf)
{
    int i;
    int is_negative;
    char tmp;
    int start;
    int end;

    i = 0;
    is_negative = 0;
    if (n < 0)
    {
        is_negative = 1;
        buf[i++] = '-';
        n = -n;
    }
    if (n == 0)
        buf[i++] = '0';
    while (n > 0)
    {
        buf[i++] = '0' + (n % 10);
        n /= 10;
    }
    buf[i] = '\0';
    
    // Reverse the digits (but not the negative sign)
    start = is_negative ? 1 : 0;
    end = i - 1;
    while (start < end)
    {
        tmp = buf[start];
        buf[start] = buf[end];
        buf[end] = tmp;
        start++;
        end--;
    }
}

char *expand_complex_variable(t_shell *shell, const char *arg)
{
	char	*result;
	char	*var_value;
	size_t	i;
	size_t	j;
	char	var_name[1024];
	int		in_single_quotes;
	int		in_double_quotes;
	int		was_quoted_var;
	int		prev_was_space;
	int		had_var_expansion;
	int		is_command_name;

	ft_putstr_fd("\n[DEBUG] expand_complex_variable input: '", STDERR_FILENO);
	ft_putstr_fd((char *)arg, STDERR_FILENO);
	ft_putendl_fd("'", STDERR_FILENO);

	if (!arg)
		return (NULL);
	if (arg[0] == '~')
		return (expand_tilde(shell, arg));
	ft_putstr_fd("[DEBUG][MEMORY] Allocating 4096 bytes for result in expand_complex_variable\n", STDERR_FILENO);
	result = malloc(4096);
	if (!result)
	{
		ft_putstr_fd("[DEBUG][MEMORY] Failed to allocate memory for result\n", STDERR_FILENO);
		return (NULL);
	}
	i = 0;
	j = 0;
	in_single_quotes = 0;
	in_double_quotes = 0;
	was_quoted_var = 0;
	prev_was_space = 0;
	had_var_expansion = 0;
	is_command_name = (ft_strchr(arg, '$') == arg + 1);  // Check if $ is second character

	// Check if the argument starts with a double quote
	if (arg[0] == '"')
		was_quoted_var = 1;

	ft_putendl_fd("[DEBUG] Starting character-by-character processing:", STDERR_FILENO);

	while (arg[i])
	{
		ft_putstr_fd("[DEBUG] Index ", STDERR_FILENO);
		ft_putnbr_fd(i, STDERR_FILENO);
		ft_putstr_fd(": Processing char: '", STDERR_FILENO);
		ft_putchar_fd(arg[i], STDERR_FILENO);
		ft_putstr_fd(", ASCII: ", STDERR_FILENO);
		ft_putnbr_fd(arg[i], STDERR_FILENO);
		ft_putstr_fd(", in_single_quotes: ", STDERR_FILENO);
		
		ft_putnbr_fd(in_single_quotes, STDERR_FILENO);
		ft_putstr_fd(", in_double_quotes: ", STDERR_FILENO);
		ft_putnbr_fd(in_double_quotes, STDERR_FILENO);
		ft_putchar_fd('\n', STDERR_FILENO);

		// For command name expansion, skip the first character
		if (is_command_name && i == 0)
		{
			i++;
			continue;
		}

		// Handle quote state changes
		if (arg[i] == '\'' && !in_double_quotes)
		{
			ft_putendl_fd("[DEBUG] Toggling single quotes", STDERR_FILENO);
			in_single_quotes = !in_single_quotes;
			i++;
			continue;
		}
		if (arg[i] == '"')
		{
			ft_putendl_fd("[DEBUG] Toggling double quotes", STDERR_FILENO);
			in_double_quotes = !in_double_quotes;
			was_quoted_var = 1;  // Set this when we enter double quotes
			i++;
			continue;
		}

		// Handle variable expansion
		if (arg[i] == '$' && !in_single_quotes)
		{
			ft_putendl_fd("[DEBUG] Found $ character", STDERR_FILENO);
			if (arg[i + 1] == '?')
			{
				char exit_status_str[16];
				ft_putendl_fd("[DEBUG] Expanding $?", STDERR_FILENO);
				ft_itoa_buf(shell->exit_status, exit_status_str);
				ft_putstr_fd("[DEBUG] Exit status: ", STDERR_FILENO);
				ft_putendl_fd(exit_status_str, STDERR_FILENO);
				ft_strlcpy(result + j, exit_status_str, ft_strlen(exit_status_str) + 1);
				j += ft_strlen(exit_status_str);
				i += 2;
				continue;
			}
			else if (arg[i + 1] && (ft_isalnum(arg[i + 1]) || arg[i + 1] == '_'))
			{
				size_t var_len = 0;
				i++;
				while (arg[i + var_len] && (ft_isalnum(arg[i + var_len]) || arg[i + var_len] == '_'))
					var_len++;
				if (var_len > 0)
				{
					ft_strlcpy(var_name, arg + i, var_len + 1);
					ft_putstr_fd("[DEBUG] Found variable: '", STDERR_FILENO);
					ft_putstr_fd(var_name, STDERR_FILENO);
					ft_putendl_fd("'", STDERR_FILENO);
					
					var_value = hashmap_get(shell->env, var_name);
					if (var_value)
					{
						had_var_expansion = 1;
						ft_putstr_fd("[DEBUG] Variable value: '", STDERR_FILENO);
						ft_putstr_fd(var_value, STDERR_FILENO);
						ft_putendl_fd("'", STDERR_FILENO);

						// For command name expansion, only take the first word
						if (is_command_name)
						{
							size_t k = 0;
							// Skip leading quotes
							if (var_value[k] == '"' || var_value[k] == '\'')
								k++;
							// Skip leading spaces
							while (var_value[k] && ft_isspace(var_value[k]))
								k++;
							// Copy first word (command)
							while (var_value[k] && !ft_isspace(var_value[k]) && var_value[k] != '"' && var_value[k] != '\'')
								result[j++] = var_value[k++];
							// Skip trailing quotes and spaces
							while (var_value[k] && (var_value[k] == '"' || var_value[k] == '\'' || ft_isspace(var_value[k])))
								k++;
							// Add a space if there are more arguments
							if (var_value[k])
								result[j++] = ' ';
							// Copy the rest of the arguments
							while (var_value[k])
							{
								if (var_value[k] != '"' && var_value[k] != '\'')
								{
									if (ft_isspace(var_value[k]))
									{
										if (!prev_was_space)
										{
											result[j++] = ' ';
											prev_was_space = 1;
										}
									}
									else
									{
										result[j++] = var_value[k];
										prev_was_space = 0;
									}
								}
								k++;
							}
						}
						else
						{
							// Process the variable value
							size_t k = 0;
							// Skip the outermost quotes of the stored value
							if (var_value[k] == '"' || var_value[k] == '\'')
								k++;

							// Copy the value, preserving spaces if in double quotes or was quoted
							while (var_value[k])
							{
								// Skip the closing quote if we're at the end
								if ((var_value[k] == '"' || var_value[k] == '\'') && 
									k == ft_strlen(var_value) - 1)
									break;

								// Always preserve spaces in double quotes
								if (in_double_quotes)
								{
									result[j++] = var_value[k];
								}
								else if (ft_isspace(var_value[k]))
								{
									if (!prev_was_space)
									{
										result[j++] = ' ';
										prev_was_space = 1;
									}
								}
								else
								{
									result[j++] = var_value[k];
									prev_was_space = 0;
								}
								k++;
							}
						}
					}
					else
					{
						ft_putendl_fd("[DEBUG] Variable not found", STDERR_FILENO);
						if (in_double_quotes || was_quoted_var)
							was_quoted_var = 1;
					}
					i += var_len;
					continue;
				}
			}
			else
			{
				ft_putendl_fd("[DEBUG] $ not followed by valid variable name", STDERR_FILENO);
				result[j++] = arg[i++];
				continue;
			}
		}

		// Handle regular characters
		if (ft_isspace(arg[i]))
		{
			if (!prev_was_space)
			{
				result[j++] = ' ';
				prev_was_space = 1;
			}
		}
		else
		{
			result[j++] = arg[i];
			prev_was_space = 0;
		}
		i++;
	}

	// Only trim trailing space if it wasn't from a variable expansion
	if (!had_var_expansion && j > 0 && result[j - 1] == ' ')
		j--;

	result[j] = '\0';
	
	ft_putstr_fd("[DEBUG] Final result: '", STDERR_FILENO);
	ft_putstr_fd(result, STDERR_FILENO);
	ft_putendl_fd("'", STDERR_FILENO);

	// If this was a quoted variable that expanded to empty, return an empty string
	// but not NULL so echo knows to preserve the space
	if (was_quoted_var && j == 0)
	{
		ft_putstr_fd("[DEBUG][MEMORY] Returning empty string for quoted var\n", STDERR_FILENO);
		result[0] = '\0';
		return result;
	}
	
	// If the result is empty and we didn't process a quoted variable, return NULL
	if (j == 0)
	{
		ft_putstr_fd("[DEBUG][MEMORY] Freeing empty result and returning NULL\n", STDERR_FILENO);
		free(result);
		return NULL;
	}
	
	ft_putstr_fd("[DEBUG][MEMORY] Returning result from expand_complex_variable\n", STDERR_FILENO);
	return (result);
}

static bool is_valid_n_flag(const char *arg)
{
    if (!arg || !*arg || arg[0] != '-')
        return false;
    
    int i = 1;
    while (arg[i])
    {
        if (arg[i] != 'n')
            return false;
        i++;
    }
    return i > 1;  // Must have at least one 'n'
}

int ft_echo(t_shell *shell, t_ast_node *node)
{
	int		i;
	int		n_flag;
	char	*expanded;
	int		first_arg;

	if (!node || !node->args)
		return (1);

	ft_putstr_fd("\n[DEBUG] ft_echo: Processing arguments\n", STDERR_FILENO);
	for (int j = 0; node->args[j]; j++)
	{
		ft_putstr_fd("[DEBUG] Arg ", STDERR_FILENO);
		ft_putnbr_fd(j, STDERR_FILENO);
		ft_putstr_fd(": '", STDERR_FILENO);
		ft_putstr_fd(node->args[j], STDERR_FILENO);
		ft_putendl_fd("'", STDERR_FILENO);
	}

	i = 1;
	n_flag = 0;
	first_arg = 1;
	while (node->args[i] && is_valid_n_flag(node->args[i]))
	{
		n_flag = 1;
		i++;
	}

	ft_putstr_fd("[DEBUG] Starting to process non-flag arguments from index: ", STDERR_FILENO);
	ft_putnbr_fd(i, STDERR_FILENO);
	ft_putchar_fd('\n', STDERR_FILENO);

	while (node->args[i])
	{
		ft_putstr_fd("[DEBUG] Processing argument: '", STDERR_FILENO);
		ft_putstr_fd(node->args[i], STDERR_FILENO);
		ft_putendl_fd("'", STDERR_FILENO);

		// If not the first argument, always print a space before the next argument
		if (!first_arg)
			ft_putchar_fd(' ', STDOUT_FILENO);

		expanded = expand_complex_variable(shell, node->args[i]);
		if (expanded)
		{
			ft_putstr_fd("[DEBUG] After expansion: '", STDERR_FILENO);
			ft_putstr_fd(expanded, STDERR_FILENO);
			ft_putendl_fd("'", STDERR_FILENO);

			ft_putstr_fd(expanded, STDOUT_FILENO);
			free(expanded);
		}
		// Even if expansion is empty, we've handled the space before it
		first_arg = 0;
		i++;
	}
	if (!n_flag)
	{
		ft_putendl_fd("[DEBUG] No -n flag, adding newline", STDERR_FILENO);
		ft_putchar_fd('\n', STDOUT_FILENO);
	}
	return (0);
}


