NAME = webserv

DIR_SRCS	=	srcs/
DIR_OBJS	=	obj/
DIR_NET		=	network/
DIR_UTI		=	utils/
DIR_REQ		=	HTTPProtocol/
DIR_MET		=	HTTPMethod/
DIR_TEST	=	clientTest/
DIR_CONF	=	Configuration/

CONFIG		=	Config.cpp	RouteConfig.cpp	ServerConfig.cpp
NETWORK		=	ASocket.cpp BindSocket.cpp ListenSocket.cpp Server.cpp Client.cpp Manager.cpp 
UTILS		=	utils.cpp FileToVar.cpp VarToFile.cpp
METHOD		=	DELETE.cpp GET.cpp POST.cpp
PROTOCOL	=	HTTPReponse.cpp HTTPRequest.cpp HTTPRequestHandler.cpp

SRCS		=	$(addprefix $(DIR_UTI), $(UTILS))		\
				$(addprefix $(DIR_REQ), $(PROTOCOL))	\
				$(addprefix $(DIR_MET), $(METHOD))		\
				$(addprefix $(DIR_NET), $(NETWORK))		\
				$(addprefix $(DIR_CONF), $(CONFIG))		\
				main.cpp
PATH_SRCS	=	$(addprefix $(DIR_SRCS), $(SRCS))				

OBJS		=	$(addprefix $(DIR_OBJS), $(SRCS:.cpp=.o))

INCLUDE = -I . -I includes/ -I $(DIR_SRCS)$(DIR_MET) -I $(DIR_SRCS)$(DIR_NET) -I $(DIR_SRCS)$(DIR_REQ) -I $(DIR_SRCS)$(DIR_UTI)

C_GREEN = \033[0;32m

C_RED = \033[0;31m

C_BLUE = \033[0;34m

C_BPURP = \033[1;35m

C_END=\033[0m

CC = c++

CFLAGS = -Wall -Wextra -Werror -std=c++98 #-g3 -fsanitize=address

all: $(NAME)

$(NAME):  $(OBJS)
	@$(CC) $(CFLAGS) -o $(NAME) $(OBJS)
	@echo "$(C_GREEN)$(NAME) compiled with $(C_BPURP)$(CFLAGS)$(C_END)"


$(DIR_OBJS)%.o: $(DIR_SRCS)%.cpp
	@mkdir -p $(dir $@)
	@$(CC) $(CFLAGS) $(INCLUDE) -c $< -o $@

# Include dependency files
-include $(OBJS:.o=.d)

clean:
	@rm -rf $(DIR_OBJS)
	@echo "$(C_RED)$(NAME) cleanse$(C_END)"

fclean: clean
	@rm -rf $(NAME)

re: fclean all

debug: CFLAGS += -fsanitize=address -g3
debug: re

add:
	@if [ -z "$(MSG)" ]; then \
		read -p "Enter commit message: " msg; \
		git add Makefile $(PATH_SRCS) webdata error/*.html */*.hpp */*/*.hpp CGIScript/ includes *.conf .gitignore; \
		git commit -m "$$msg"; \
	else \
		git add Makefile $(PATH_SRCS) webdata error/*.html */*.hpp */*/*.hpp CGIScript/ includes *.conf .gitignore; \
		git commit -m "$(MSG)"; \
	fi; \
	git push

.PHONY: all clean fclean re add debug
