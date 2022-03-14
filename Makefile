# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: ebellon <ebellon@student.42lyon.fr>        +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2022/03/14 20:11:26 by ebellon           #+#    #+#              #
#    Updated: 2022/03/14 20:27:09 by ebellon          ###   ########lyon.fr    #
#                                                                              #
# **************************************************************************** #


#========================================#
#=============== VARIABLES ==============#
#========================================#

#~~~~ Output ~~~~#

NAME		=	minishell

#~~~~ Paths ~~~~#

VPATH		=	src/:src/utils/:src/signal/:src/expansions/:src/exec/:src/builtins/:src/redirections/:src/shell
PATH_INC	=	inc/
PATH_OBJ	=	bin/

#~~~~ Files ~~~~#
 
SRC			=	environement.c \
				env_utils.c \
				libft.c \
				libft_bis.c \
				libft_third.c \
				main.c \
				expand_variables_utils.c \
				expand_variables.c \
				expands_quotes.c \
				expand.c \
				quote_utils.c\
				expands_words.c \
				redir_in.c \
				redir_out_simple.c \
				redir_out_double.c \
				heredoc.c \
				signal.c \
				pipex_utils.c \
				pipex.c \
				bi_cd.c \
				bi_pwd.c \
				bi_export.c \
				bi_unset.c \
				bi_env.c \
				bi_exit.c \
				bi_echo.c \
				error.c \
				pipex_utils_second.c \
				pipex_trash_second.c \
				pipex_trash.c \
				main_utils.c \
				utils.c

OBJ			=	$(addprefix $(PATH_OBJ), $(SRC:.c=.o))

INC			=	$(addprefix $(PATH_INC), minishell.h)

#~~~~ Macros ~~~~#

CC			=	gcc
CFLAG		=	-Wall -Wextra -Werror
INCLUDES	=	-I $(PATH_INC)
RM			=	rm -rf

#========================================#
#=============== TARGETS ================#
#========================================#

#~~~~ Main Rules ~~~~#

all :			$(NAME)

$(NAME) :		$(OBJ) $(LIB)
				$(CC) $(CFLAG) $(OBJ) -lreadline -o $(NAME) $(INCLUDES)

re :			fclean all

#~~~~ Compilation Rules ~~~~#

$(PATH_OBJ)%.o :	%.c $(INC)
					@mkdir -p $(dir $@);
					$(CC) $(CFLAG) -c $< -o $@ $(INCLUDES)

#~~~~ Cleaning Rules ~~~~#

clean :
				$(RM) $(PATH_OBJ)

fclean :		
				$(RM) $(PATH_OBJ) $(NAME)

.PHONY:		all clean fclean re 