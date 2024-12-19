# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: szhong <marvin@42.fr>                      +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/11/08 12:46:50 by szhong            #+#    #+#              #
#    Updated: 2024/12/19 13:53:29 by szhong           ###   ########.fr        #
#                                                                              #
# **************************************************************************** #
#Define colour
DF = \033[0;39m
GRAY = \033[0;90m
RED = \033[0;91m
GREEN = \033[0;92m
MAGENTA = \033[0;95m
YELLOW = \033[0;93m
CYAN =\033[0;96m
WHITE = \033[0;97m
BOLD = \033[91m
ORANGE = \033[38;5;208m

NAME		:=	minishell
CC			:=	cc
CFLAGS		:=	-Wall -Werror -Wextra -g3
INCLUDE		:= -I./inc -I./libft/inc
SRC_DIR		:=	./src
OBJ_DIR		:=	./obj

SRCS		:=	\
				main.c \
				init/init.c \
				lexer/tokeniser.c \
				parser/parser.c \
				signals/ft_signals.c \
				executor/executor.c \
				executor/pipe_handler.c \
				executor/redirections.c \
				utils/ft_errors.c \
				utils/string_utils.c \
				utils/cleanup.c \
				builtins/builtins.c \
				builtins/ft_pwd.c \
				builtins/ft_echo.c \
				builtins/ft_cd.c \
				builtins/ft_export.c \
				builtins/ft_unset.c \
				builtins/ft_env.c \
				builtins/ft_exit.c \
				hashtable/hashtable_create.c \
				hashtable/hashtable_destroy.c \
				hashtable/hashtable_env_conversion.c \
				hashtable/hashtable_get.c \
				hashtable/hashtable_insert.c \
				hashtable/hashtable_iterate.c \
				hashtable/hashtable_murmurhash2.c \
				hashtable/hashtable_remove.c \
				hashtable/hashtable_search.c \
				hashtable/hashtable_set.c \
				hashtable/hashtable_size.c

OBJS		:=	$(addprefix $(OBJ_DIR)/, $(patsubst %.c, %.o, $(SRCS)))
LIBFT_PATH	:=	./libft
ALL_LIBS	:=	-L$(LIBFT_PATH) -lft -L/usr/local/opt/readline/lib -lreadline
MKFL		:=	--no-print-directory

all: banner $(NAME)

banner:
	    @echo "$(BOLD) $(GREEN)"
		@echo " ____________________________________________ "
		@echo "|     ____       _____ __  __________    __  | "
		@echo "|    / __ \     / ___// / / / ____/ /   / /  | "
		@echo "|   / / / /_____\__ \/ /_/ / __/ / /   / /   | "
		@echo "|  / /_/ /_____/__/ / __  / /___/ /___/ /___ | "
		@echo "| /_____/     /____/_/ /_/_____/_____/_____/ | "
		@echo "|____________________________________________| "
		@echo "                                              "
		@echo "$(RED)minihell   (.)(.)                       "
		@echo "        \\ /  ()  \\                          "
		@echo "        _ \\ '--' / _                         "
		@echo "       { '-\`\"\"\"\"\"\`-' }                 "
		@echo "        \`\"/      \\\"\`                     "
		@echo "          \\      /                           "
		@echo "         _/  /\\  \\_                         "
		@echo "        {   /  \\   }                          "
		@echo "         \`\"\`    \`\"\`                     "
		@echo "$(DF)"


$(NAME): LIBFT $(OBJS)
	@$(CC) $(CFLAGS) $(OBJS) $(ALL_LIBS) -o $(NAME)
	@echo "${YELLOW}[MINISHELL] ${GREEN}Build Completed${DF}"

LIBFT:
	@make $(MKFL) -C $(LIBFT_PATH) all

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	@mkdir -p $(@D)
	@$(CC) $(CFLAGS) $(INCLUDE) -c $< -o $@



clean:
	@make $(MKFL) -C $(LIBFT_PATH) clean
	@rm -rf $(OBJ_DIR)
	@echo "${CYAN}[MINISHELL] Objects Removed${CYAN}"

fclean: clean
	@make $(MKFL) -C $(LIBFT_PATH) fclean
	@rm -rf $(NAME)
	@echo "${CYAN}[MINISHELL] Everything Removed${CYAN}"
	@echo "${BOLD}${ORANGE}======== PROJECT RESET ========${DF}"

re:	fclean all

test: LIBFT
	@echo "${YELLOW}Running some tests${DF}"
	@make $(MKFL) -C tests test || exit 1
	@echo "${GREEN}Tests completed.${DF}"

.PHONY: all clean fclean re
