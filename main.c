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

#include <linux/module.h>
#include <linux/init.h>
#include <linux/kthread.h>

MODULE_AUTHOR("Maciej Nabozny <mn@mnabozny.pl>");
MODULE_DESCRIPTION("CloudOver kernel driver");
MODULE_LICENSE("GPL");
MODULE_VERSION("0.1");

#include <processbuffer.h>
#include <clouddev.h>
#include <testthread.h>
#include <syscall.h>

static struct task_struct *thread = NULL;

int kernelConnect_init(void) {
    device_init();
    thread = test_init();

    return 0;
}

void kernelConnect_exit(void) {
    device_cleanup();
    test_cleanup(thread);
}


module_init(kernelConnect_init);
module_exit(kernelConnect_exit);
