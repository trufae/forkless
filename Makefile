OBJ=main.o forkless.o
CFLAGS+=-I.

all: $(OBJ) bin/echo
	$(CC) $(OBJ)
	$(MAKE) -C oth/msh
	$(MAKE) -C oth/ired
	./a.out

bin/echo:
	$(MAKE) -C bin

%.o: %.c
	$(CC) -c $<

clean:
	$(MAKE) -C bin clean
	rm a.out
