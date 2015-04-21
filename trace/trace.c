#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <time.h>
#include <sys/ioctl.h>

int main(int argc, char *argv[]) {
	int pid = fork();
	if (pid > 0) {
		fprintf(stderr, "Tracing pid %d\n", pid);
		int cloud = open("/dev/cloudover", O_RDONLY);
		if (cloud < 0) {
			fprintf(stderr, "Cannot open cloudover dev\n");
			return 1;
		}
		ioctl(cloud, 0, pid);
		close(cloud);
	} else if (pid == 0) {
		sleep(2);
		execl("/bin/cat", "/bin/cat", "/etc/passwd", 0x00);
	}

	return 0;
}
