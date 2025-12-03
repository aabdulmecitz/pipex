NAME			= pipex
NAME_BONUS		= pipex

GREEN			= \033[0;32m
RED				= \033[0;31m
RESET			= \033[0m

LIBFT 			= lib/libft/libft.a

CC 				= cc

REMOVE 			= rm -f

SRCS_DIR		= ./src/

SRCS 			= $(addprefix $(SRCS_DIR),\
				pipex.c utils.c gc.c error.c)

BONUS_SRC		= $(addprefix $(SRCS_DIR),\
				pipex_bonus.c utils_bonus.c gc_bonus.c error_bonus.c)

${NAME}: 		
				${CC} -Wall -Wextra -Werror ${SRCS} ${LIBFT} -g -o ${NAME}
				@echo "$(NAME): $(GREEN)$(NAME) was compiled.$(RESET)"
				@echo

all:			${NAME} ${LIBFT} 
bonus:			${NAME_BONUS} ${LIBFT} 

${NAME_BONUS}: 		
				${CC}  -Wall -Wextra -Werror  ${BONUS_SRC} ${LIBFT} -g -o ${NAME_BONUS}
				@echo "$(NAME_BONUS): $(GREEN)$(NAME_BONUS) was compiled.$(RESET)"
				@echo

${LIBFT}:
				@echo
				make bonus -C lib/libft

clean:
				make clean -C lib/libft
				@echo

fclean:
				${REMOVE} ${NAME} ${NAME_BONUS}
				@echo "${NAME}: ${RED}${NAME} and ${NAME_BONUS} were deleted${RESET}"
				@echo
push:
	git add .
	git commit -m "commit"
	git push

re:				fclean all

re_bonus:		fclean all_bonus

compile_libs:
	@make -sC lib/libft
	@make clean -sC lib/libft


update:
	git submodule update --init --recursive --remote

valgrind:
	$(VALGRIND) ./pipex txt1.txt "ls -l" "wc -l" txt2.txt


.PHONY:			all clean fclean re rebonus valgrind run makefile

#./pipex txt1.txt "ls -l" "wc -l" txt2.txt
#./pipex txt1.txt "ls -l" "wc -l" "cat -e" txt2.txt
#./pipex here_doc "ls -l" "wc -l" txt2.txt