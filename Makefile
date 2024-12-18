# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: szhong <marvin@42.fr>                      +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/11/08 12:46:50 by szhong            #+#    #+#              #
#    Updated: 2024/12/09 15:06:20 by szhong           ###   ########.fr        #
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
CFLAGS		:=	-Wall -Werror -Wextra -g3 -D_GNU_SOURCE
INCLUDE		:= -I./inc -I./libft/inc -I/usr/local/opt/readline/include
SRC_DIR		:=	./src
OBJ_DIR		:=	./obj

SRCS		:=	\
				main.c \
				init/init.c \
				lexer/tokeniser.c \
				parser/parser.c \
				signals/ft_signals.c \
				hashtable/ft_hashtable.c \
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
				builtins/ft_exit.c

OBJS		:=	$(addprefix $(OBJ_DIR)/, $(patsubst %.c, %.o, $(SRCS)))
LIBFT_PATH	:=	./libft
ALL_LIBS	:=	-L$(LIBFT_PATH) -lft -L/usr/local/opt/readline/lib -lreadline
MKFL		:=	--no-print-directory

all: $(NAME) 

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
