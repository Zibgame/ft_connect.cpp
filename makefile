# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: zcadinot <zcadinot@student.42lehavre.      +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2026/02/26 13:17:11 by zcadinot          #+#    #+#              #
#    Updated: 2026/03/08 23:26:26 by zcadinot         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = ft_connect
TRUE_BIN = src/persistance/true

CXX = c++
CXXFLAGS = -Wall -Wextra -Werror -std=c++17
INCLUDES = -I src/includes

SRC = main.cpp \
      src/file/file.cpp \
      src/logs/logs.cpp \
      src/persistance/persistance.cpp \
      src/persistance/remove_pr.cpp \
      src/notify/notify.cpp

TRUE_SRC = src/persistance/true.cpp

OBJ_DIR = .obj
OBJ = $(addprefix $(OBJ_DIR)/, $(SRC:.cpp=.o))

all: $(NAME) $(TRUE_BIN)

$(NAME): $(OBJ)
	$(CXX) $(CXXFLAGS) $(INCLUDES) $(OBJ) -o $(NAME)

$(TRUE_BIN): $(TRUE_SRC)
	$(CXX) $(CXXFLAGS) $(INCLUDES) $(TRUE_SRC) -o $(TRUE_BIN)
	chmod 755 $(TRUE_BIN)

$(OBJ_DIR)/%.o: %.cpp
	@mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) $(INCLUDES) -c $< -o $@

clean:
	rm -rf $(OBJ_DIR)

fclean: clean
	rm -f $(NAME) $(TRUE_BIN)

re: fclean all

.PHONY: all clean fclean re
