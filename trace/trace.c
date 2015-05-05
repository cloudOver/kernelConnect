#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <time.h>
#include <sys/ioctl.h>

struct co_pid {
    int cloud_pid;
    int client_pid;
}

int main(int argc, char *argv[]) {
    struct co_pid info;
    int cloud_pid = fork();
    int client_pid = atoi(argv[1]);

    info.client_pid = client_pid;
    info.cloud_pid = cloud_pid;

    if (cloud_pid > 0) {
        fprintf(stderr, "Tracing pid %d as application %d\n", cloud_pid, client_pid);
		int cloud = open("/dev/cloudover", O_RDONLY);
		if (cloud < 0) {
			fprintf(stderr, "Cannot open cloudover dev\n");
			return 1;
		}

        ioctl(cloud, 0, &info);
		close(cloud);
    } else if (cloud_pid == 0) {
		sleep(2);
		execl("/bin/cat", "/bin/cat", "/etc/passwd", 0x00);
	}

	return 0;
}
