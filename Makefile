NAME			= pipex
NAME_BONUS		= pipex_bonus

GREEN			= \033[0;32m
RED				= \033[0;31m
RESET			= \033[0m

LIBFT 			= lib/libft/libft.a

CC 				= cc

CFLAGS 			= -Wall -Wextra -Werror

REMOVE 			= rm -f

SRC_DIR			= src
BONUS_DIR		= bonus

SRCS 			= $(addprefix ${SRC_DIR}/,pipex.c utils.c utils2.c gc.c error.c)
BONUS 			= $(addprefix ${BONUS_DIR}/,pipex_bonus.c utils_bonus.c utils2_bonus.c \
				  gc_bonus.c error_bonus.c)

OBJS			= ${SRCS:.c=.o}
OBJS_BONUS		= ${BONUS:.c=.o}

${NAME}: ${OBJS} ${LIBFT}
			${CC} ${CFLAGS} -I ${SRC_DIR} ${OBJS} ${LIBFT} -g -o ${NAME}
			@echo "$(NAME): $(GREEN)$(NAME) was compiled.$(RESET)"
			@echo

all:			${NAME}

bonus:			${NAME_BONUS}

${NAME_BONUS}:	${OBJS_BONUS} ${LIBFT}
			${CC} ${CFLAGS} -I ${BONUS_DIR} ${OBJS_BONUS} ${LIBFT} -g -o ${NAME_BONUS}
			@echo "$(NAME_BONUS): $(GREEN)$(NAME_BONUS) was compiled.$(RESET)"
			@echo

run:			all
			./${NAME} infile "cat -e" "wc -l" outfile

run_bonus:		bonus
			./${NAME_BONUS} infile "cat -e" "wc -l" outfile

%.o:			%.c
			${CC} ${CFLAGS} -I ${SRC_DIR} -I ${BONUS_DIR} -c $< -o $@

${LIBFT}:
			@echo
			make bonus -C lib/libft

clean:
			${REMOVE} ${OBJS} ${OBJS_BONUS}
			make clean -C lib/libft
			@echo

fclean:
			${REMOVE} ${NAME} ${NAME_BONUS} ${OBJS} ${OBJS_BONUS}
			@echo "${NAME}: ${RED}${NAME} was deleted${RESET}"
			@echo

re:				fclean all

bonus_re:		fclean bonus

valgrind:
	$(VALGRIND) ./pipex txt1.txt "ls -l" "wc -l" txt2.txt

.PHONY:			all bonus clean fclean re bonus_re run run_bonus valgrind

.PHONY:			all clean fclean re valgrind