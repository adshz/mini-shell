/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test_main.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: szhong <szhong@student.42london.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/12 22:31:20 by szhong            #+#    #+#             */
/*   Updated: 2024/11/12 22:31:58 by szhong           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "ft_test.h"
#include "shell.h"
#include "libft.h"

/* Import test function declarations */
//void	register_builtin_tests(t_test_case **suite);
void	register_hashtable_tests(t_test_case **suite);
// Add other test registrations as needed

int	main(void)
{
	t_test_case	*suite;

	suite = NULL;
	ft_printf("Test runner starting..\n");
	ft_printf("Registering tests...\n");
	/* Register all test categories */
	//register_builtin_tests(&suite);
	register_hashtable_tests(&suite);
	// Register other test categories
	
	if (!suite)
	{
		ft_printf("Error: No tests registered\n");
		return (1);
	}
	ft_printf("Running tests...\n");
	/* Run all tests */
	test_run_suite(suite);
	
	/* Cleanup */
	test_cleanup(suite);
	ft_printf("Tests completed.\n");
	return (0);
}
