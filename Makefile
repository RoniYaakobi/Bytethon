CC = gcc
CFLAGS = -Wall -I./include -g
SRC = src/utils.c src/general_management.c src/pointer_management.c src/interact_with_memory.c src/my_malloc.c src/my_free.c src/visualize.c src/cli.c src/main.c
OBJ = $(SRC:.c=.o)
EXE = main.exe

$(EXE): $(OBJ)
	$(CC) -o $(EXE) $(OBJ)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	@del /F /Q src\*.o
	@del /F /Q $(EXE)