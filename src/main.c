/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: szhong <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/08 14:42:19 by szhong            #+#    #+#             */
/*   Updated: 2024/12/09 17:18:33 by szhong           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "shell.h"

int	g_signal

void	handle_signint(int sig)
{
	sig = g_signal;
	write(STDOUT_FILENO, "\n", 1);
	rl_on_new_line();
	rl_replace_line("", 0);
	rl_redisplay();
}

void	handle_sigquit(int sig)
{
	g_signal = sig;
	write(1, "\n", 1);
}

void	setup_signals(void)
{
	// set up signal handlers
	signal(SIGINT, handle_sigint); // Handle Ctrl + C;
	signal(SIGQUIT, handle_sigquit); // Handle Ctrl + \;
	signal(STGTSTP, SIG_IGN);
}

void	init_builtins(t_shell *shell)
{
	shell->builtins = malloc(sizeof(t_builtin_table));
	if (!shell->builtins)
		return ;
	hashmap_insert(shell->builtins->commands, "cd", ft_cd);
	hashmap_insert(shell->builtins->commands, "echo", ft_echo);
	hashmap_insert(shell->builtins->commands, "pwd", ft_pwd);
	hashmap_insert(shell->builtins->commands, "export", ft_export);
	hashmap_insert(shell->builtins->commands, "unset", ft_unset);
	hashmap_insert(shell->builtins->commands, "env", ft_env);
	hashmap_insert(shell->builtins->commands, "exit", ft_exit);
}

void	init_shell(t_shell *shell, char **envp)
{
	char	*equals;

	shell->env = hashmap_create(1024); // Create a hash table
	shell->history = NULL; // Start with empty history
	shell->last_status = 0; //initial status
	shell->running = 1; // indicating shell running or not
	shell->shell_pid = getpid(); //get shell process id
	shell->builtins = NULL;
	while (*envp) // convert environment variables to hash table
	{
		equals = ft_strchr(*envp, '=');
		if (equals)
		{
			*equals = '\0'; // split the string
			hashmap_insert(shell->env, *envp, equals + 1);
			*equals = '='; // Restore string
		}
		envp++;
	}
	init_builtins(shell);
}

// MurmurHash 2 Implementation
unsigned long	hash_function(const char *key, unsigned int size)
{
	const unsigned int	m;
	const int			r;
	unsigned int		h;
	unsigned int		k;
	int					len;

	m = 0x5bd1e995;
	r = 24;
	h = 0;
	len = ft_strlen(key);
	// Mix 4 bytes at a time into the hash
	while(len >= 4)
	{
		k = *(unsigned int *)key;
		k ^= k >> r;
		k *= m;
		h *= m;
		h ^= k;
		key += 4;
		len -= 4;
	}
	// handle the last few bytes
	if (len > 2)
		h ^= key[2] << 16;
	if (len > 1)
		h ^= key[1] << 8;
	if (len > 0)
	{
		h ^= key[0];
		h *= m;
	}
	// do a few final mixes;
	h ^= h >> 13;
	h *= m;
	h ^= h >> 15;
	return (h % size);
}

t_hash_item	*create_hash_item(const char *key, void *value)
{
	t_hash_item	*item;

	item = malloc(sizeof(t_hash_item));
	if (!item)
		return (NULL);
	item->key = ft_strdup(key);
	item->value = value; // directly store function pointers
	item->next = NULL;
	return (item);
}

// insert hash table
int	hashmap_insert(t_hashmap *map, const char *key, void *value)
{
	unsigned long	index;
	t_hash_item		*current;
	t_hash_item		*new_item;

	if (!map || !key || !value)
		return (0);
	index = hash_function(key, map->size);
	current = map->item[index];
	while (current)
	{
		if (ft_strcmp(current->key, key) == 0)
		{
			current_value = value;
			return (1);
		}
		current = current->next;
	}
	//  Insert at beginning of chain (handling collisions
	new_item = create_hash_item(key, value);
	new_item->next = map->item[index];
	map->count++;
	return (1);
}

// Different approach to command execution
void	execute_command(t_shell *shell, t_command *cmd)
{
	t_hash_item	*builtin;
	int			(*builtin_func)(t_shell *, t_command *);

	builtin	= hashmao_search(shell->builtins->commands, cmd->name);
	if (builtin)
	{
		// Execute builtin directly
		built_func = value;
		shell->last_status = builtin_func(shell, cmd);
	}
	else
		execute_external(shell, cmd);
}

// prompt.c
char	*display_prompt(t_shell *shell)
{
	char	*input;

	input = readline(">>minishell$ ");
	if (!input)
	{
		ft_printf("\n");
		shell->running = 0;
		return (NULL);
	}
	if (input[0] == '\0')
	{
		free(input);
		return (NULL);
	}
	return (input);
}

// parser.c
t_command	*parse_command(char *input)
{
	t_command	*cmd;
	char		**args;

	cmd = malloc(sizeof(t_command));
	if (!cmd)
		return (NULL);
	// Initialise command struct
	cmd->pipe_in = 0;
	cmd->pipe_out = 0;
	cmd->type = EXTERNAL; // EXTERNAL means not builtin
	// split input into arguments
	args = ft_split(input, ' ');
	if (!args)
	{
		free(cmd);
		return (NULL);
	}
	cmd->name = ft_strdup(args[0]);
	cmd->args = args;
	return (cmd);
}

void	free_cmd(t_command *cmd)
{
	int	i;

	if (!cmd)
		return ;
	if (cmd->name)
		free(cmd->name);
	if (cmd->args)
	{
		i = 0;
		while (cmd ->args[i])
		{
			free(cmd->args[i]);
			i++;
		}
		free(cmd->args);
	}
	free(cmd);
}

// history.c
void	add_to_history(t_shell *shell, const char *input)
{
	t_list	*new_entry;

	// Forbid empty commands or NULL input
	if (!input || !input[0])
		return ;
	new_entry = malloc(sizeof(t_list));
	if (!new_entry)
		return ;
	new_entry->content = ft_strdup(input);
	if (!new_entry->content)
	{
		free(new_entry);
		return ;
	}
	add_history(input);
	// add to our history list
	new_entry->next = shell->history;
	shell->history = new_entry;
}

int	main(int argc, char *argv[], char *envp[])
{
	t_command	*cmd;
	t_shell		*shell;
	char		*input;

	if (argc != 1)
		return (1);
	init_shell(shell, envp);
	setup_signals();
	while (shell->running)
	{
		input = display_prompt(shell); //display_prompt is handling Ctrl + D
		if (!input)
			break ;
		cmd = parse_command(input);
		if (cmd)
		{
			execute_command(shell, cmd);
			free_cmd(cmd);
		}
		add_to_history(shell, input);
		free(input);
	}
	cleanup_shell(shell);
	return (shell->last_status);
}
