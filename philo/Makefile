CC = cc
CFLAGS = -Wall -Wextra -Werror -g

NAME = philo

PFLAGS = -pthread

SRCS = main.c utils.c list_utils.c

OBJS = $(SRCS:.c=.o)

all: $(NAME)

$(NAME): $(OBJS)
	@$(CC) $(CFLAGS) $(PFLAGS) $(OBJS) -o $(NAME)
	@clear
	@echo "\n\033[1;36mprogram ready\033[0m\n"

%.o: %.c
	@$(CC) -c $(CFLAGS) $< -o $@

clean:
	@rm -rf $(OBJS)
	@clear
	@echo "\n\033[32mdone\n\033[0m"

fclean: clean
	@rm -f $(NAME)
	@clear
	@echo "\n\033[32mdone\n\033[0m"

leaks: 
	
re: fclean all

.PHONY: all clean fclean re
