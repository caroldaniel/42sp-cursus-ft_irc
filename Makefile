# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: user42 <user42@student.42.fr>              +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/03/06 12:03:46 by cado-car          #+#    #+#              #
#    Updated: 2024/03/06 22:50:30 by user42           ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME		= ircserv

CC			= c++
CFLAGS		= -Wall -Wextra -Werror -std=c++98

RM			= rm -rf

OBJ_DIR		= obj/
SRC_DIR		= src/

SRC			= main.cpp \
			  Server.cpp \
			  utils.cpp

VPATH		= $(SRC_DIR) \
			$(SRC_DIR)utils \
			$(SRC_DIR)Server 
			
# **************************************************************************** #
# 								RULES									       #
# **************************************************************************** #

OBJ			= $(addprefix $(OBJ_DIR), $(notdir $(SRC:.cpp=.o)))

$(OBJ_DIR)%.o: %.cpp
			@printf "$(CY)Generating object files...$(RC)\n"
			mkdir -p $(OBJ_DIR)
			$(CC) $(CFLAGS) -c $< -o $@
			@printf "$(GR)Object files generated successfully!$(RC)\n"

$(NAME):	$(OBJ)
			@printf "$(CY)Compiling...$(RC)\n"
			$(CC) $(CFLAGS) -o $(NAME) $(OBJ)
			@printf "$(GR)Compiled successfully!$(RC)\n"

all: 		$(NAME)

re:			fclean all

clean:
			@printf "$(YE)Cleaning object files...$(RC)\n"
			$(RM) $(OBJ_DIR)
			@printf "$(GR)Object files cleaned successfully!$(RC)\n"

fclean:		clean
			@printf "$(YE)Cleaning binary files...$(RC)\n"
			$(RM) $(NAME)
			@printf "$(GR)Binary files cleaned successfully!$(RC)\n"

leak:		$(NAME)
			valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes --track-fds=yes ./$(NAME) 9000 123

.PHONY:		all re clean fclean


# **************************************************************************** #
# 								COLORS									       #
# **************************************************************************** #
GR	= \033[32;1m
RE	= \033[31;1m
YE	= \033[33;1m
CY	= \033[36;1m
RC	= \033[0m