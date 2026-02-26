# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: zcadinot <zcadinot@student.42lehavre.      +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2026/02/26 13:17:11 by zcadinot          #+#    #+#              #
#    Updated: 2026/02/26 14:16:08 by zcadinot         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = ft_connect

CXX = c++
CXXFLAGS = -Wall -Wextra -Werror -std=c++17
INCLUDES = -I src/includes

SRC = main.cpp \
      src/file/file.cpp \
      src/notify/notify.cpp

OBJ_DIR = .obj
OBJ = $(addprefix $(OBJ_DIR)/, $(SRC:.cpp=.o))

all: $(NAME)

$(NAME): $(OBJ)
	$(CXX) $(CXXFLAGS) $(INCLUDES) $(OBJ) -o $(NAME)

$(OBJ_DIR)/%.o: %.cpp
	@mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) $(INCLUDES) -c $< -o $@

clean:
	rm -rf $(OBJ_DIR)

fclean: clean
	rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re
