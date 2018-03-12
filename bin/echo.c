#include <unistd.h>
#include <string.h>

int main(int argc, char **argv) {
	int i;
	write(1, "BIN/MAIN\n", 9);
	for (i = 0; i < argc; i++) {
		const char *arg = argv[i];
		write (1, arg, strlen (arg));
		if (i + 1 < argc) {
			write (1, " ", 1);
		}
	}
	write (1, "\n", 1);
	return 4;
}
