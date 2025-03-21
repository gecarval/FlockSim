# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: gecarval <gecarval@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/01/20 08:19:33 by gecarval          #+#    #+#              #
#    Updated: 2025/02/12 16:00:00 by gecarval         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

CXX = c++
CXXFLAGS = -Wall -Wextra -Werror -g
STDRULE = 
NAME = flocksim
DEBUG = valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes --suppressions=./valgrind.supp -s
SRC =	imgui_demo.cpp						\
		imgui_tables.cpp					\
		rlImGui.cpp							\
		imgui.cpp							\
		imgui_draw.cpp						\
		imgui_widgets.cpp					\
		./srcs/classes/Boid.cpp				\
		./srcs/classes/Flock.cpp			\
		./srcs/classes/SpatialHashing.cpp	\
		./srcs/input.cpp					\
		./srcs/menu.cpp						\
		./srcs/update.cpp					\
		./srcs/main.cpp


INCS = -I./includes/classes/ -L./includes/classes/ -I./includes -L./includes/ -lraylib -lGL -lm -lpthread -ldl -lrt -lX11
OBJ = $(SRC:.cpp=.o)

all: $(NAME)

$(NAME): $(OBJ)
	$(CXX) $(CXXFLAGS) $(STDRULE) $(OBJ) -o $(NAME) $(INCS)

%.o: %.cpp
	$(CXX) $(CXXFLAGS) $(STDRULE) -c $< -o $@ $(INCS)

clean:
	rm -f $(OBJ)

fclean: clean
	rm -f $(NAME)

re: fclean all

run: all
	./$(NAME)

debug:
	$(DEBUG) ./$(NAME)

.PHONY: all clean fclean re run debug
