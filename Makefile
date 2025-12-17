CC = cc
CFLAGS = -Wall -Wextra -Werror -Ilibft -Iincludes -g3

SRC_DIR = src
OBJ_DIR = obj

LIBFT_DIR = libft
LIBFT= $(LIBFT_DIR)/libft.a

NAME = minishell

SRC_FILES = main.c \
			parsing/token_utils.c \
			parsing/token_utils_2.c \
			parsing/lexer.c \
			parsing/expander.c \
			parsing/expander_utils.c \
			parsing/redir_utils.c \
			parsing/cmd_utils.c \
			parsing/parser.c \
			parsing/parser_utils.c \
			parsing/lexer_utils.c \
			parsing/errors.c \
			parsing/check_syntax.c \
			parsing/utils_to_delete.c \
			exec/child.c \
			exec/cleanup_utils.c \
			exec/env.c \
			exec/path.c \
			exec/pipe_utils.c \
			exec/pipeline.c

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

# Compile the object files for main sources
$(OBJ_DIR)/%.o : $(SRC_DIR)/%.c
	@mkdir -p $(dir $@) # @ so not printed
	$(CC) $(CFLAGS)  -c $< -o $@

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

