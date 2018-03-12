OBJ=main.o forkless.o
CFLAGS+=-I.

all: $(OBJ)
	$(CC) $(OBJ)
	./a.out

%.o: %.c
	$(CC) -c $<
