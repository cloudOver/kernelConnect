#include "syscall.h"

void syscall_init() {
    cloudover_set__open((void *)cloudover_open);
    cloudover_set__close((void *)cloudover_close);
    cloudover_set__truncate((void *)cloudover_truncate);
    cloudover_set__access((void *)cloudover_access);
}
