BINS=true false echo

all: $(BINS)

$(BINS):
	$(CC) -shared -fPIC -o $@ $@.c

clean:
	rm -f $(BINS)
