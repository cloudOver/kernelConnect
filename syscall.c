/**
Copyright (c) 2015 Maciej Nabozny

This file is part of KernelConnect project.

KernelConnect is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

KernelConnect is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with KernelConnect.  If not, see <http://www.gnu.org/licenses/>.
*/

#include <syscall.h>

void syscall_init(void) {
    printk(KERN_INFO "syscall_init: eplacing syscalls\n");
    cloudover_set__open((void *)cloudover_open);
    cloudover_set__close((void *)cloudover_close);
    cloudover_set__truncate((void *)cloudover_truncate);
    cloudover_set__access((void *)cloudover_access);
}

void syscall_cleanup(void) {
    cloudover_set__open(NULL);
    cloudover_set__close(NULL);
    cloudover_set__truncate(NULL);
    cloudover_set__access(NULL);
}
