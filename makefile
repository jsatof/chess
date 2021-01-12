CC = gcc 
FLAGS = -g 

INCS = -I$(SDL_INC_PATH)
LIBS = -L$(SDL_LIB_PATH) -lSDL2

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

