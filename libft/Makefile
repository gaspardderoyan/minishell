NAME = libft.a

HEADER = libft.h

CC = cc
CFLAGS = -Wall -Wextra -Werror
AR = ar rcs
RM = rm -f

SRCS_LIBFT =	ft_atoi.c ft_bzero.c ft_calloc.c ft_isalnum.c \
				ft_isalpha.c ft_isascii.c ft_isdigit.c ft_isprint.c ft_itoa.c \
				ft_memchr.c ft_memcmp.c ft_memcpy.c ft_memmove.c ft_memset.c \
				ft_putchar_fd.c ft_putendl_fd.c ft_putnbr_fd.c ft_putstr_fd.c \
				ft_split.c ft_strchr.c ft_strdup.c ft_striteri.c ft_strjoin.c \
				ft_strlcat.c ft_strlcpy.c ft_strlen.c ft_strmapi.c ft_strncmp.c \
				ft_strnstr.c ft_strrchr.c ft_strtrim.c ft_substr.c ft_tolower.c \
				ft_toupper.c ft_free_array.c ft_lstnew.c ft_lstadd_front.c \
				ft_lstsize.c ft_lstlast.c ft_lstadd_back.c ft_lstdelone.c \
				ft_lstclear.c ft_lstiter.c ft_lstmap.c

SRCS_PRINTF = 	ft_printf.c print_cs.c print_nbr.c print_ptr.c \
				ft_dprintf.c dprint_cs.c dprint_nbr.c dprint_ptr.c

SRCS_GNL = 		get_next_line.c get_next_line_utils.c \
				get_next_line_bonus.c get_next_line_utils_bonus.c

SRCS = $(SRCS_LIBFT) $(SRCS_PRINTF) $(SRCS_GNL)

OBJS = $(SRCS:.c=.o)

%.o: %.c $(HEADER)
	@$(CC) $(CFLAGS) -c $< -o $@

$(NAME): $(OBJS)
	@$(AR) $@ $^

all: $(NAME)

clean:
	@$(RM) $(OBJS)

fclean: clean
	@$(RM) $(NAME)

re: fclean all

.PHONY: bonus all clean fclean re
