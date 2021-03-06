#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <time.h>
#include <sys/ioctl.h>


int main(int argc, char *argv[]) {
    int cloud_pid = fork();

    if (cloud_pid > 0) {
        fprintf(stderr, "Tracing pid %d\n", cloud_pid);
		int cloud = open("/dev/cloudover", O_RDONLY);
		if (cloud < 0) {
			fprintf(stderr, "Cannot open cloudover dev\n");
			return 1;
		}

        ioctl(cloud, 0, cloud_pid);
		close(cloud);
    } else if (cloud_pid == 0) {
        sleep(20);
        fprintf(stderr, "go!\n");
		execl("/bin/cat", "/bin/cat", "/etc/passwd", 0x00);
	}

	return 0;
}
