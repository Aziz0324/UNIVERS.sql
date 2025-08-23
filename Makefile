NAME	=	my_ls
CC	=	cc
CFLAGS	=	-Wall -Wextra -Werror
SRC	=	main.c util.c sort.c listdir.c
OBJ	=	$(SRC:.c=.o)

all:	$(NAME)

$(NAME):	$(OBJ)
	$(CC) $(CFLAGS) -o $(NAME) $(OBJ)

clean:
	rm -f $(OBJ)

fclean:	clean
	rm -f $(NAME)

re:	fclean all

asan:	fclean
	$(CC) -g3 -fsanitize=address -fno-omit-frame-pointer $(CFLAGS) -o $(NAME) $(SRC)