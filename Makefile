SRCS = ft_split.c ft_strjoin.c philo.c monitor.c
OBJS = ${SRCS:.c=.o}
NAME = philo
CC = cc
RM = rm -f
CFLAGS = -Wall -Wextra -Werror
${NAME}: ${OBJS}
	${CC} ${CFLAGS} ${OBJS} -o ${NAME}
all : ${name}
clean:
	${RM} ${OBJS}
fclean: clean
	${RM} ${NAME}
re: clean all
.PHONY: all clean fclean re
.SECONDARY: ${OBJS}