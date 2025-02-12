#ifndef CD_TESTS_H
# define CD_TESTS_H

# include "../../framework/test_framework.h"

// Basic CD tests
bool test_cd_to_root(char **environ);
bool test_cd_home(char **environ);
bool test_cd_desktop(char **environ);

// Special CD tests
bool test_cd_previous(char **environ);
bool test_cd_with_args(char **environ);

// Error case CD tests
bool test_cd_custom_home(char **environ);
bool test_cd_nonexistent(char **environ);

// Test suite declaration
extern t_test_case cd_test_cases[];
extern const int cd_test_count;

#endif 