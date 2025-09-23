NAME = minishell

SRC_PATH = src/
OBJ_PATH = build/
LIBFT_PATH = libft/
LIBFT_AR = $(LIBFT_PATH)libft.a

SRC = \
	  main.c \
	  lexing/lexer.c\
	  lexing/lexer_utils.c \
	  lexing/lexer_word.c\
	  lexing/string_builder.c \
	  lexing/token_utils.c \
	  parsing/argv_builder.c\
	  parsing/parsing_error.c \
	  parsing/parser.c\
	  expand/expand.c\
	  expand/expand_word.c\
	  expand/expand_dollar.c\
	  execution/lst_cmd.c \
	  execution/lst_cmd2.c \
	  execution/execution_node.c \
	  execution/multipipe.c \
	  execution/redirection_multipipe.c \
	  execution/lst_redir.c \
	  execution/lst_redir2.c \
	  execution/heredoc.c \
	  execution/heredoc2.c \
	  execution/execution.c \
	  execution/redirection_verification.c \
	  execution/banner.c \
	  execution/signals.c \
	  execution/ctl.c \
	  execution/utils.c \
	  env/getenv.c \
	  env/env_utils.c \
	  env/lst_env.c \
	  env/lst_env_two.c \
	  builtin/unset.c \
	  builtin/env.c \
	  builtin/my_echo.c \
	  builtin/pwd.c \
	  builtin/cd.c \
	  builtin/export.c \
	  builtin/exit.c \
	  builtin/launcher.c \

SRCS = $(addprefix $(SRC_PATH), $(SRC))
OBJ = $(addprefix $(OBJ_PATH), $(SRC:.c=.o))
DPD = $(addprefix $(OBJ_PATH), $(SRC:.c=.d))

CC = cc
CFLAGS = -Wall -Wextra -Werror -MMD -g #-fsanitize=address,leak
INCS = includes/minishell.h includes/parsing.h includes/parsing_helpers.h
INC = -Iincludes -I$(LIBFT_PATH)/includes

PURPLE = \033[0;35m
NC = \033[0m

all: $(LIBFT_AR) $(NAME)

$(NAME): $(OBJ) $(INCS)
	@echo "\n$(PURPLE)  Compiling Minishell...$(NC)"
	@$(CC) $(CFLAGS) $(OBJ) $(LIBFT_AR) $(INC) -lreadline -o $(NAME)
	@echo "\n$(PURPLE)  Ready ✓$(NC)\n"

-include $(DPD)

$(OBJ_PATH)%.o: $(SRC_PATH)%.c
	@mkdir -p $(@D)
	@$(CC) $(CFLAGS) $(INC) -c $< -o $@

$(LIBFT_AR):
	@echo "\n$(PURPLE)  Compiling LIBFT...$(NC)"
	@make -s -C $(LIBFT_PATH)
	@echo "\n$(PURPLE)  Done ✓$(NC)"

clean:
	@echo "\n$(PURPLE)  Cleaning object files... $(NC)"
	@rm -rf $(OBJ_PATH)
	@rm -rf $(OBJ) $(OBJ_BON)
	@rm -rf $(DPD) $(OBJ_DPD)
	@make -s clean -C $(LIBFT_PATH)
	@echo "\n$(PURPLE)  Done ✓$(NC)"

fclean: clean
	@echo "\n$(PURPLE)  Cleaning Minishell...$(NC)"
	@$(RM) $(NAME)
	@$(RM) $(NAME_BON)
	@make -s fclean -C $(LIBFT_PATH)
	@echo "\n$(PURPLE)  Done ✓$(NC)"

dev: all clean
 
CHAT = { ignore_readline_leaks Memcheck:Leak ... obj:*/libreadline.so.* } { ignore_bin_functions Memcheck:Leak ... obj:/usr/bin/* } { ncurses_termcap Memcheck:Leak match-leak-kinds:reachable fun:rl_make_bare_keymap fun:rl_generic_bind fun:rl_parse_and_bind obj:/usr/lib/x86_64-linux-gnu/libreadline.so.8.2 fun:rl_initialize fun:readline }

ignore:
	@for i in $(CHAT); do \
   		echo $$i >> ignore.supp; \
    done

val:
	@make -s dev
	@if ! [ -f "ignore.supp" ]; then make ignore; fi
	@valgrind --suppressions=./ignore.supp --leak-check=full --show-leak-kinds=all --track-origins=yes --trace-children=yes --track-fds=yes -s ./minishell

re: fclean all

.PHONY: all clean fclean re dev val ignore
