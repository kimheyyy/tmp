CXX = c++

CXXFLAGS = -std=c++98
OPT = -Wall -Werror -Wextra

NAME = ircserv

SRC = main.cpp \
      Server.cpp \
      Client.cpp \
      Command.cpp \
      Channel.cpp

SRC_CMD = commands/pass.cpp \
          commands/mode.cpp \
          commands/op.cpp   \
          commands/part.cpp \
          commands/notice.cpp \
          commands/nick.cpp \
          commands/user.cpp \
          commands/join.cpp \
          commands/ping.cpp \
          commands/invite.cpp \
          commands/kick.cpp \
          commands/privmsg.cpp \
          commands/quit.cpp \
          commands/list.cpp \
          commands/topic.cpp \
          commands/connect.cpp

DEPS = Server.hpp \
       Client.hpp \
       Channel.hpp \
       Define.hpp \
       Command.hpp

SRCS = $(SRC) $(SRC_CMD)

OBJS = $(SRCS:.cpp=.o)

# Color codes
RED = \033[0;31m
GREEN = \033[0;32m
YELLOW = \033[1;33m
NC = \033[0m # No Color

all: before_compile $(NAME)

before_compile:
	@echo "$(YELLOW)Building... $(NC)"

$(NAME): $(OBJS) $(DEPS)
	@$(CXX) $(CXXFLAGS) $(OBJS) -o $(NAME) $(OPT)
	@echo "$(GREEN)Build complete!$(NC)"

%.o: %.cpp $(DEPS)
	@$(CXX) $(CXXFLAGS) $(OPT) -c $< -o $@

clean:
	@echo "$(RED)Removing object files...$(NC)"
	@rm -f $(OBJS)

fclean: clean
	@echo "$(RED)Removing binary...$(NC)"
	@rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re
