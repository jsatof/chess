CC = gcc 
FLAGS = -g 

INCS = -I/usr/include/SDL2
LIBS = -L/usr/lib -lSDL2

EXEC = run
SRCS = $(shell find *.c)
OBJS = $(SRCS:%.c=%.o)

all: $(EXEC)



# link phase
$(EXEC): $(OBJS)
	$(CC) $(FLAGS) $(OBJS) -o $(EXEC) $(LIBS)

# compile phase
$(OBJS): $(SRCS)
	$(CC) $(INCS) $(FLAGS) -c $^

clean:
	rm -rf $(EXEC) $(OBJS)
