# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: bsuger <bsuger@student.42.fr>              +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/04/23 13:25:23 by bsuger            #+#    #+#              #
#    Updated: 2025/07/18 16:37:23 by bsuger           ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

FLAGS=-Wall -Wextra -Werror -g

header=libft.h

NAME=libft.a

SRCS=./ft_isalpha.c \
     ./ft_isdigit.c \
     ./ft_isalnum.c \
     ./ft_isalnum.c \
     ./ft_isascii.c \
     ./ft_isprint.c \
     ./ft_toupper.c \
     ./ft_tolower.c \
     ./ft_strlen.c \
     ./ft_putchar_fd.c \
     ./ft_putstr_fd.c \
     ./ft_putendl_fd.c \
     ./ft_putnbr_fd.c \
     ./ft_atoi.c \
     ./ft_strchr.c \
     ./ft_strrchr.c \
     ./ft_strnstr.c \
     ./ft_strncmp.c \
     ./ft_strdup.c \
     ./ft_memset.c \
     ./ft_bzero.c \
     ./ft_memcpy.c \
     ./ft_memmove.c \
     ./ft_calloc.c \
     ./ft_memchr.c \
     ./ft_memcmp.c \
     ./ft_strlcat.c \
     ./ft_strlcpy.c \
     ./ft_strmapi.c \
     ./ft_striteri.c \
     ./ft_substr.c \
     ./ft_strtrim.c \
     ./ft_itoa.c \
     ./ft_strjoin.c \
     ./ft_split.c \
     ./ft_strtok.c \
     ./ft_is_n.c \
     ./get_next_line.c \
     ./ft_atoi_base.c \
     ./ft_printf.c \
     ./ft_printf_utils.c \
     ./ft_strndup.c \
     ./ft_free_tab.c \
	 ./ft_free.c \
	 ./ft_realloc.c \
	 ./ft_strcmp.c \
	 ./ft_iswspace.c

BONUS_SRCS=./ft_lstnew_bonus.c \
	   ./ft_lstadd_front_bonus.c \
	   ./ft_lstsize_bonus.c \
	   ./ft_lstlast_bonus.c \
	   ./ft_lstadd_back_bonus.c \
	   ./ft_lstdelone_bonus.c \
	   ./ft_lstclear_bonus.c \
	   ./ft_lstiter_bonus.c \
	   ./ft_lstmap_bonus.c

OBJ=$(SRCS:.c=.o)

BONUS_OBJ=$(BONUS_SRCS:.c=.o)

all: $(NAME)

$(NAME): $(OBJ)
	   ar rcs $(NAME) $(OBJ)
%.o: %.c $(header)
	cc -I.  $(FLAGS) -c $< -o $@

bonus: $(BONUS_OBJ) $(OBJ)
	ar rcs $(NAME) $(OBJ) $(BONUS_OBJ)

BONUS_OBJ=$(BONUS_SRCS:.c=.o)

clean:
	/bin/rm -f $(OBJ) $(BONUS_OBJ)
fclean: clean
	/bin/rm -f $(NAME)
re: fclean all

.PHONY: clean fclean re 
