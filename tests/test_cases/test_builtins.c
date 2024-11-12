/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test_builts.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: szhong <szhong@student.42london.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/12 22:25:48 by szhong            #+#    #+#             */
/*   Updated: 2024/11/12 22:31:10 by szhong           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "ft_test.h"
#include "libft.h"
#include "shell.h"
#include <unistd.h>
#include <stdlib.h>
#include <sys/stat.h>

/* Test utilities */
static char	*get_safe_dir(void)
{
	char	*dir;

	dir = getcwd(NULL, 0);
	if (!dir)
	{
		ft_printf("Error: Failed to get current directory\n");
		return (NULL);
	}
	return (dir);
}

static int	cleanup_test_dir(const char *dir_path)
{
	if (rmdir(dir_path) != 0)
	{
		ft_printf("Warning: Failed to remove test directory: %s\n", dir_path);
		return (0);
	}
	return (1);
}

static void test_pwd_basic(t_test_result *result)
{
	char	*expected;
	char	*actual;

	ft_printf("Running basic PWD test...\n");
	expected = get_safe_dir();
	if (!expected)
	{
		assert_true(result, 0, "Failed to get current directory");
		return ;
	}
	actual = ft_pwd();
	if (!actual)
	{
		free(expected);
		assert_true(result, 0, "ft_pwd returned NULL");
		return ;
	}
	assert_str_eq(result, expected, actual, "pwd should return current directory");
	free(expected);
	free(actual);
}

static void	test_pwd_after_chdir(t_test_result *result)
{
	char	*original_dir;
	char	*new_dir;
	char	*pwd_result;

	ft_printf("Running PWD after chdir test..\n");
	original_dir = get_safe_dir();
	if (!original_dir || chdir("..") != 0)
	{
		assert_true(result, 0, "Test setup failed");
		return ;
	}
	new_dir = getcwd(NULL, 0);
	if (!new_dir)
	{
		free(original_dir);
		assert_true(result, 0, "Failed to get new directory");
		return ;
	}
	pwd_result = ft_pwd();
	if (!pwd_result)
	{
		free(original_dir);
		free(new_dir);
		assert_true(result, 0, "ft_pwd returned NULL after chdir");
		return ;
	}
	assert_str_eq(result, new_dir, pwd_result, "pwd should reflect directory change");
	if (chdir(original_dir) != 0)
		ft_printf("Warning: Failed to restore original directory\n");	
	free(original_dir);
	free(new_dir);
	free(pwd_result);
}

static void	test_pwd_no_permissions(t_test_result *result)
{
	char	*pwd_result;
	char	*temp_dir;
	char	*original_dir;

	ft_printf("Running PWD no permissions test...\n");
	temp_dir = "/tmp/test_pwd";
	original_dir = get_safe_dir();
	if (!original_dir)
	{
		assert_true(result, 0, "Failed to get original directory");
		return ;
	}
	if (mkdir(temp_dir, 0000) != 0)
	{
		free(original_dir);
		assert_true(result, 0, "Failed to create test directory");
		return ;
	}
	if (chdir(temp_dir) == 0)
	{
		pwd_result = ft_pwd();
		assert_true(result, pwd_result != NULL, 
			"pwd should handle no-permission directory");
		free(pwd_result);
	}
	else
		ft_printf("Warning: Failed to change to test directory\n");
	if (chdir(original_dir) != 0)
		ft_printf("Warning: Failed to restore original directory\n");
	cleanup_test_dir(temp_dir);
	free(original_dir);
}

void	register_builtin_tests(t_test_case **suite)
{
	test_add(suite, test_create("PWD Basic Test", test_pwd_basic));
	test_add(suite, test_create("PWD After chdir Test", test_pwd_after_chdir));
	test_add(suite, test_create("PWD No Permissions Test", 
		test_pwd_no_permissions));
}
