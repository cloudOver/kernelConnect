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

#include <testthread.h>

static int thread_count = 0;
char str[] = "Hello world!";

struct task_struct *test_init(void) {
    struct task_struct *test_thread;

    thread_count += 1;
    test_thread = kthread_run(test_send, (void *)thread_count, "kernelConnect_test_%d", thread_count);
    return test_thread;
}

int test_send(void *data) {
    struct message *incoming;
    struct message *msg;
    int len;

    while (!kthread_should_stop()) {
        len = strlen(str);

        printk(KERN_INFO "test_send: sending message (%d bytes, at %p)\n", len, str);
        msg = message_new((void *) str, len);
        message_send(msg);

        printk(KERN_INFO "test_send: message has been sent (%lu bytes, at %p)\n", msg->size, msg->data);


        incoming = message_get();
        if (incoming != NULL) {
            printk(KERN_INFO "test_send: received new message for pid: %d, data at: %p\n", incoming->pid, incoming->data);
        }
        msleep(5000);
    }
    // kthread_should_stop...
    return 0;
}

void test_cleanup(struct task_struct *thread) {
    kthread_stop(thread);
}
