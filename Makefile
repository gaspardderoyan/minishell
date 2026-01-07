CC = cc
CFLAGS = -Wall -Wextra -Werror -Ilibft -Iincludes -g3

SRC_DIR = src
OBJ_DIR = obj

LIBFT_DIR = libft
LIBFT= $(LIBFT_DIR)/libft.a

NAME = minishell

SRC_FILES = main.c \
			builtins/builtin_cd.c \
			builtins/builtin_exit.c \
			builtins/builtin_export.c \
			builtins/builtin_rest.c \
			builtins/builtin_unset.c \
			builtins/builtins_dispatch.c \
			builtins/builtins_exec.c \
			builtins/sort_export.c \
			env/env.c \
			exec/child.c \
			exec/heredoc.c \
			exec/heredoc_utils.c \
			exec/pipeline.c \
			exec/wait.c \
			parsing/expander/expander.c \
			parsing/expander/expander_helpers.c \
			parsing/lexer/token.c \
			parsing/lexer/lexer.c \
			parsing/lexer/lexer_helpers.c \
			parsing/parser/parser.c \
			parsing/parser/parser_helpers.c \
			parsing/parser/command.c \
			parsing/parser/redirection.c \
			parsing/syntax.c \
			parsing/errors.c \
			utils/signals.c \
			utils/signals_heredoc.c \
			utils/cleanup.c \
			utils/env.c \
			utils/errors.c \
			utils/path.c \
			utils/pipe.c \
			utils/memory.c

SRCS= $(addprefix $(SRC_DIR)/, $(SRC_FILES))

OBJS = $(addprefix $(OBJ_DIR)/, $(SRC_FILES:.c=.o))

.PHONY: all clean fclean libft re

################################################################################
# Main Build Targets
################################################################################

all: $(NAME)

# Build the main executable
$(NAME): $(OBJS) $(LIBFT) 
	$(CC) $(CFLAGS) $(OBJS) $(LIBFT) -o $(NAME) -lreadline
	@echo "$(NAME) is ready !"

# Compile the object files for main sources
$(OBJ_DIR)/%.o : $(SRC_DIR)/%.c
	@mkdir -p $(dir $@)
# printf plus puissant que echo format du texte propre
# %s : on insere une chaine de caracteres (string)
# 30 : reserve un espace fixe de 30 caracteres
# - : Aligne le texte a gauche (sans le moins, il s'aligne a droite)
	@printf "Compiling: %-30s\r" $(notdir $<)
	@$(CC) $(CFLAGS)  -c $< -o $@

################################################################################
# Helper Targets (clean, fclean, re)
################################################################################

# Build libft
$(LIBFT):
	$(MAKE) -C $(LIBFT_DIR) 

# Clean object files and libft's objs
clean:
	$(MAKE) -C $(LIBFT_DIR) clean
	rm -rf $(OBJ_DIR)

fclean: clean
	$(MAKE) -C $(LIBFT_DIR) fclean
	rm -f $(NAME)

# Rebuild the project
re: fclean all
