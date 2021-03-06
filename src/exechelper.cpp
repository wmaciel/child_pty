#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <sys/ioctl.h>
#include <fcntl.h>
#include <arpa/inet.h>

int main(int argc, char *argv[]) {
	int master_fd, state_fd;
	uint32_t rv;

	if(argc < 4)
		return EXIT_FAILURE;
	state_fd = atoi(argv[1]);
	master_fd = atoi(argv[2]);
	if (fcntl(state_fd, F_SETFD, fcntl(state_fd, F_GETFD) | FD_CLOEXEC) >= 0 &&
			ioctl(STDIN_FILENO, TIOCSCTTY, NULL) >= 0 &&
			close(master_fd) >= 0) {
		argc -= 3;
		memmove(argv, argv + 3, argc * sizeof(char*));
		argv[argc] = NULL;
		execvp(argv[0], argv);
	}
	rv = htonl(errno);
	write(state_fd, &rv, sizeof(rv));
	pause();
	return EXIT_FAILURE;
}
