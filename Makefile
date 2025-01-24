# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: gecarval <gecarval@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/01/20 08:19:33 by gecarval          #+#    #+#              #
#    Updated: 2025/01/24 16:06:58 by gecarval         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

CC = c++
CFLAGS = -Wall -Wextra -Werror
STDRULE = 
NAME = flocksim
DEBUG = valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes --suppressions=./valgrind.supp -s
SRC =	imgui_demo.cpp				\
		imgui_tables.cpp			\
		rlImGui.cpp					\
		imgui.cpp					\
		imgui_draw.cpp				\
		imgui_widgets.cpp			\
		./srcs/main.cpp				\
		./srcs/classes/Boid.cpp

INCS = -I./includes -L./includes/ -lraylib -lGL -lm -lpthread -ldl -lrt -lX11
OBJ = $(SRC:.cpp=.o)

all: $(NAME)

$(NAME): $(OBJ)
	$(CC) $(CFLAGS) $(STDRULE) $(OBJ) -o $(NAME) $(INCS)

%.o: %.cpp
	$(CC) $(CFLAGS) $(STDRULE) -c $< -o $@ $(INCS)

clean:
	rm -f $(OBJ)

fclean: clean
	rm -f $(NAME)

re: fclean all

run: all
	./$(NAME)

debug:
	$(DEBUG) ./$(NAME)

.PHONY: all clean fclean re start debug