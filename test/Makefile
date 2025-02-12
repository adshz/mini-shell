NAME = mtest
DRAFT = dtest

CC = gcc
CFLAGS = -Wall -Wextra -Werror -g -Wno-unused-function

# Source directories
FRAMEWORK_DIR = framework
SUITES_DIR = suites
# Old way of finding test directories
#CD_TESTS_DIR = $(SUITES_DIR)/cd_tests
# HEREDOC_TESTS_DIR = $(SUITES_DIR)/heredoc_tests
# ECHO_TESTS_DIR = $(SUITES_DIR)/echo_tests/
# COMMAND_EXPANSION_TESTS_DIR = $(SUITES_DIR)/command_expansion_tests/
# Automatically find all test directories
TEST_DIRS := $(shell find $(SUITES_DIR) -type d)

# Source files
FRAMEWORK_SRC = $(FRAMEWORK_DIR)/test_framework.c \
                $(FRAMEWORK_DIR)/test_utils.c

# old way to get the test source files
# CD_TESTS_SRC = $(CD_TESTS_DIR)/cd_basic.c
# HEREDOC_TESTS_SRC = $(HEREDOC_TESTS_DIR)/heredoc_tests.c
# ECHO_TESTS_SRC = $(ECHO_TESTS_DIR)/echo_tests.c
# COMMAND_EXPANSION_TESTS_SRC = $(COMMAND_EXPANSION_TESTS_DIR)/command_name_expansion_test.c

# Find all test source files automaticallyA
TEST_SRCS := $(shell find $(SUITES_DIR) -name '*.c')
MAIN_SRC = main.c
DRAFT_SRC = test_main.c

# Object files
# old way to get the objects to be compiled from test source files
# OBJS = $(FRAMEWORK_SRC:.c=.o) $(CD_TESTS_SRC:.c=.o) $(HEREDOC_TESTS_SRC:.c=.o) $(MAIN_SRC:.c=.o) $(ECHO_TESTS_SRC:.c=.o) $(COMMAND_EXPANSION_TESTS_SRC:.c=.o)
# DRAFT_OBJ = $(DRAFT_SRC:.c=.o) $(TEST_GLOBALS_SRC:.c=.o)
OBJS = $(FRAMEWORK_SRC:.c=.o) $(TEST_SRCS:.c=.o) $(MAIN_SRC:.c=.o)
DRAFT_OBJ = $(DRAFT_SRC:.c=.o)

# Include directories - automatically include all test directories
INCLUDES = -I. \
	-I$(FRAMEWORK_DIR) \
	$(patsubst %,-I%,$(TEST_DIRS)) \
	-I../inc \
	-I../libft/inc \
	-I../src \
	-I../src/builtins \
	-I../src/core \
	-I../src/lexer \
	-I../src/parser \
	-I../src/utils

# Library paths
LIBFT_PATH = ../libft
SHELL_OBJ_PATH = ../obj

# OS detection
UNAME_S := $(shell uname -s)

# Readline paths based on OS
ifeq ($(UNAME_S),Darwin)
    # macOS
    READLINE_DIR := $(shell brew --prefix readline)
    INCLUDES += -I$(READLINE_DIR)/include
    LIBS = -L$(LIBFT_PATH) -lft -L$(READLINE_DIR)/lib -lreadline
else
    # Linux
    LIBS = -L$(LIBFT_PATH) -lft -lreadline
endif

# Shell Objects - find all object files automatically
SHELL_OBJS = $(shell find $(SHELL_OBJ_PATH) -name '*.o' ! -name 'main.o' ! -name 'test_main.o')

# Core objects
# CORE_OBJS = $(SHELL_OBJ_PATH)/core/core.o \
#             $(SHELL_OBJ_PATH)/core/ft_signals.o \
#             $(SHELL_OBJ_PATH)/core/shell_modes.o \
#             $(SHELL_OBJ_PATH)/core/init.o \
#             $(SHELL_OBJ_PATH)/core/init_env.o

# # Lexer objects
# LEXER_OBJS = $(SHELL_OBJ_PATH)/lexer/token_state_manager.o \
#              $(SHELL_OBJ_PATH)/lexer/token_state_transitions.o \
#              $(SHELL_OBJ_PATH)/lexer/token_type_handler.o \
#              $(SHELL_OBJ_PATH)/lexer/tokeniser.o \
#              $(SHELL_OBJ_PATH)/lexer/token_utils.o

# # Utils objects
# UTILS_OBJS = $(SHELL_OBJ_PATH)/utils/cleanup.o \
#              $(SHELL_OBJ_PATH)/utils/command_utils.o \
#              $(SHELL_OBJ_PATH)/utils/string_utils.o \
#              $(SHELL_OBJ_PATH)/utils/terminal_cleanup.o \
#              $(SHELL_OBJ_PATH)/utils/command_cleanup.o \
#              $(SHELL_OBJ_PATH)/utils/history.o \
#              $(SHELL_OBJ_PATH)/utils/error_handler.o

# # Parser objects
# PARSER_OBJS = $(SHELL_OBJ_PATH)/parser/command_utils.o \
#               $(SHELL_OBJ_PATH)/parser/ast_builder.o \
#               $(SHELL_OBJ_PATH)/parser/pipeline_parser.o \
#               $(SHELL_OBJ_PATH)/parser/command_init.o \
#               $(SHELL_OBJ_PATH)/parser/redirection_parser.o \
#               $(SHELL_OBJ_PATH)/parser/parser.o \
#               $(SHELL_OBJ_PATH)/parser/redirection_utils.o \
#               $(SHELL_OBJ_PATH)/parser/ast_free.o \
#               $(SHELL_OBJ_PATH)/parser/redirection_init.o \
#               $(SHELL_OBJ_PATH)/parser/command_parser.o

# # Builtins objects
# BUILTINS_OBJS = $(SHELL_OBJ_PATH)/builtins/ft_unset.o \
#                 $(SHELL_OBJ_PATH)/builtins/ft_echo/echo_variable_handler.o \
#                 $(SHELL_OBJ_PATH)/builtins/ft_echo/echo_number_utils.o \
#                 $(SHELL_OBJ_PATH)/builtins/ft_echo/echo_variable_expander.o \
#                 $(SHELL_OBJ_PATH)/builtins/ft_echo/echo_character_handler.o \
#                 $(SHELL_OBJ_PATH)/builtins/ft_echo/ft_echo.o \
#                 $(SHELL_OBJ_PATH)/builtins/ft_echo/echo_command_handler.o \
#                 $(SHELL_OBJ_PATH)/builtins/ft_echo/echo_state_handler.o \
#                 $(SHELL_OBJ_PATH)/builtins/builtin_utils.o \
#                 $(SHELL_OBJ_PATH)/builtins/ft_exit.o \
#                 $(SHELL_OBJ_PATH)/builtins/ft_pwd.o \
#                 $(SHELL_OBJ_PATH)/builtins/ft_cd/cd_path_handler.o \
#                 $(SHELL_OBJ_PATH)/builtins/ft_cd/cd_env_handler.o \
#                 $(SHELL_OBJ_PATH)/builtins/ft_cd/cd_executor.o \
#                 $(SHELL_OBJ_PATH)/builtins/ft_cd/cd_utils.o \
#                 $(SHELL_OBJ_PATH)/builtins/ft_env.o \
#                 $(SHELL_OBJ_PATH)/builtins/builtins_executor.o \
#                 $(SHELL_OBJ_PATH)/builtins/ft_export.o \
#                 $(SHELL_OBJ_PATH)/builtins/builtins.o

# # Expander objects
# EXPANDER_OBJS = $(SHELL_OBJ_PATH)/expander/expander_utils.o \
#                 $(SHELL_OBJ_PATH)/expander/special_expansions.o \
#                 $(SHELL_OBJ_PATH)/expander/expand_variables/simple_variable_expander.o \
#                 $(SHELL_OBJ_PATH)/expander/expand_variables/command_expander.o \
#                 $(SHELL_OBJ_PATH)/expander/expand_variables/tilde_expander.o \
#                 $(SHELL_OBJ_PATH)/expander/quote_handling.o

# # Executor objects
# EXECUTOR_OBJS = $(SHELL_OBJ_PATH)/executor/env_executor/env_array_creator.o \
#                 $(SHELL_OBJ_PATH)/executor/env_executor/env_string_utils.o \
#                 $(SHELL_OBJ_PATH)/executor/env_executor/env_variable_handler.o \
#                 $(SHELL_OBJ_PATH)/executor/command_executor/command_process_handler.o \
#                 $(SHELL_OBJ_PATH)/executor/command_executor/command_error_handler.o \
#                 $(SHELL_OBJ_PATH)/executor/command_executor/command_executor.o \
#                 $(SHELL_OBJ_PATH)/executor/command_expander/variable_expander.o \
#                 $(SHELL_OBJ_PATH)/executor/command_expander/command_name_handler.o \
#                 $(SHELL_OBJ_PATH)/executor/command_expander/path_handler.o \
#                 $(SHELL_OBJ_PATH)/executor/redirections_executor/heredoc_content_handler.o \
#                 $(SHELL_OBJ_PATH)/executor/redirections_executor/redirection_filename_expander.o \
#                 $(SHELL_OBJ_PATH)/executor/redirections_executor/redirection_utils.o \
#                 $(SHELL_OBJ_PATH)/executor/redirections_executor/redirection_setup.o \
#                 $(SHELL_OBJ_PATH)/executor/redirections_executor/heredoc_processor.o \
#                 $(SHELL_OBJ_PATH)/executor/redirections_executor/redirection_input_handler.o \
#                 $(SHELL_OBJ_PATH)/executor/redirections_executor/redirection_executor.o \
#                 $(SHELL_OBJ_PATH)/executor/redirections_executor/heredoc_io_handler.o \
#                 $(SHELL_OBJ_PATH)/executor/redirections_executor/redirection_filename_utils.o \
#                 $(SHELL_OBJ_PATH)/executor/redirections_executor/redirection_output_handler.o \
#                 $(SHELL_OBJ_PATH)/executor/pipe_handler/pipe_child.o \
#                 $(SHELL_OBJ_PATH)/executor/pipe_handler/pipe_utils.o \
#                 $(SHELL_OBJ_PATH)/executor/pipe_handler/pipe_setup.o \
#                 $(SHELL_OBJ_PATH)/executor/pipe_handler/pipe_executor.o \
#                 $(SHELL_OBJ_PATH)/executor/command_check/command_path_searcher.o \
#                 $(SHELL_OBJ_PATH)/executor/command_check/command_path_check.o \
#                 $(SHELL_OBJ_PATH)/executor/command_check/command_path_resolver.o \
#                 $(SHELL_OBJ_PATH)/executor/ast_executor/ast_executor.o \
#                 $(SHELL_OBJ_PATH)/executor/ast_executor/redirection_handler.o \
#                 $(SHELL_OBJ_PATH)/executor/ast_executor/node_handler.o

# # Hashtable objects
# HASHTABLE_OBJS = $(SHELL_OBJ_PATH)/hashtable/hashtable_murmurhash2.o \
#                  $(SHELL_OBJ_PATH)/hashtable/hashtable_iterate.o \
#                  $(SHELL_OBJ_PATH)/hashtable/hashtable_remove.o \
#                  $(SHELL_OBJ_PATH)/hashtable/hashtable_set.o \
#                  $(SHELL_OBJ_PATH)/hashtable/hashtable_get.o \
#                  $(SHELL_OBJ_PATH)/hashtable/hashtable_insert.o \
#                  $(SHELL_OBJ_PATH)/hashtable/hashtable_destory.o \
#                  $(SHELL_OBJ_PATH)/hashtable/hashtable_size.o \
#                  $(SHELL_OBJ_PATH)/hashtable/hashtable_create.o \
#                  $(SHELL_OBJ_PATH)/hashtable/hashtable_search.o \
#                  $(SHELL_OBJ_PATH)/hashtable/hashtable_env_conversion.o \
#                  $(SHELL_OBJ_PATH)/hashtable/hashtable_collision.o

# # All shell objects combined
# SHELL_OBJS = $(CORE_OBJS) \
#              $(LEXER_OBJS) \
#              $(UTILS_OBJS) \
#              $(PARSER_OBJS) \
#              $(BUILTINS_OBJS) \
#              $(EXPANDER_OBJS) \
#              $(EXECUTOR_OBJS) \
#              $(HASHTABLE_OBJS)

# Make sure directories exist
$(shell mkdir -p $(dir $(OBJS)))

# Debug information
debug:
	@echo "Test directories found:"
	@echo "$(TEST_DIRS)"
	@echo "\nTest source files found:"
	@echo "$(TEST_SRCS)"
	@echo "\nObject files to build:"
	@echo "$(OBJS)"

all: $(NAME)

$(NAME): $(OBJS)
	@echo "${YELLOW}[FRAMEWORK TESTS] ${GREEN}Building...${DF}"
	@$(CC) $(OBJS) $(SHELL_OBJS) $(LIBS) $(LDFLAGS) -o $(NAME)
	@echo "${YELLOW}[FRAMEWORK TESTS] ${GREEN}Build Completed${DF}"

draft: $(DRAFT_OBJ)
	@echo "${YELLOW}[DRAFT TESTS] ${GREEN}Building...${DF}"
	@$(CC) $(DRAFT_OBJ) $(SHELL_OBJS) $(LIBS) $(LDFLAGS) -o $(DRAFT)
	@echo "${YELLOW}[DRAFT TESTS] ${GREEN}Build Completed${DF}"

%.o: %.c
	@$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

clean:
	@rm -f $(OBJS) $(DRAFT_OBJ)
	@echo "${YELLOW}[TESTS] ${GREEN}Cleaned object files${DF}"

fclean: clean
	@rm -f $(NAME) $(DRAFT)
	@echo "${YELLOW}[TESTS] ${GREEN}Cleaned everything${DF}"

re: fclean all

.PHONY: all clean fclean re draft debug 
