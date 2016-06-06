CC=cc
AS=nasm
CFLAGS=-g $(ALLEGRO_FLAGS)
ALLEGRO_FLAGS=-lallegro -lallegro_color
ASFLAGS=-g -f elf64
SRC=main.c
ASM=mandelbrot.s
OBJ=$(SRC:.c=.o) $(ASM:.s=.o)

EXEC=mandelbrot

all: $(OBJ)
	$(CC) $(CFLAGS) $(OBJ) -o $(EXEC)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm *.o $(EXEC)

run: all
	./$(EXEC)

debug: all
	gdb $(EXEC)

.PHONY: all clean
