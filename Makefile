NAME			= pipex

GREEN			= \033[0;32m
RED				= \033[0;31m
RESET			= \033[0m

LIBFT 			= lib/libft/libft.a

CC 				= cc

REMOVE 			= rm -f

SRCS 			= pipex.c utils.c utils2.c gc.c error.c

OBJS			= ${SRCS:.c=.o}

${NAME}: ${OBJS} ${LIBFT}
			${CC} -Wall -Wextra -Werror ${OBJS} ${LIBFT} -g -o ${NAME}
			@echo "$(NAME): $(GREEN)$(NAME) was compiled.$(RESET)"
			@echo

all:			${NAME}

run:			all
			./${NAME} infile "cat -e" "wc -l" outfile

%.o:			%.c
			${CC} -Wall -Wextra -Werror -c $< -o $@

${LIBFT}:
			@echo
			make bonus -C lib/libft

clean:
			${REMOVE} ${OBJS}
			make clean -C lib/libft
			@echo

fclean:
			${REMOVE} ${NAME} ${OBJS}
			@echo "${NAME}: ${RED}${NAME} was deleted${RESET}"
			@echo

re:				fclean all

valgrind:
	$(VALGRIND) ./pipex txt1.txt "ls -l" "wc -l" txt2.txt

.PHONY:			all clean fclean re valgrind