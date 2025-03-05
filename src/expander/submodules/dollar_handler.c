char	*dollar_handler(t_shell *shell, char *str, size_t *i)
{
	char	*ret;

	(*i)++;
	if (!str[*i] || str[*i] == ' ' || str[*i] == '"' || str[*i] == '\'')
		return (ft_strdup("$"));
	if (str[*i] == '?')
	{
		(*i)++;
		ret = ft_itoa(shell->exit_status);
		return (ret);
	}
	ret = get_env_var(shell, str, i);
	return (ret);
}

char	*get_env_var(t_shell *shell, char *str, size_t *i)
{
	size_t	start;
	char	*key;
	char	*value;

	start = *i;
	while (str[*i] && (ft_isalnum(str[*i]) || str[*i] == '_'))
		(*i)++;
	key = ft_substr(str, start, *i - start);
	ft_putstr_fd("DEBUG [dollar]: Looking up env var with key: '", 2);
	ft_putstr_fd(key ? key : "(null)", 2);
	ft_putstr_fd("'\n", 2);
	if (!key)
		return (NULL);
	value = hashmap_get(shell->env, key);
	ft_memory_delone(key);
	if (!value)
	{
		ft_putstr_fd("DEBUG [dollar]: No value found for key, returning empty string\n", 2);
		return (ft_strdup(""));
	}
	ft_putstr_fd("DEBUG [dollar]: Found value: '", 2);
	ft_putstr_fd(value, 2);
	ft_putstr_fd("'\n", 2);
	return (ft_strdup(value));
} 
