#ifndef TESTTHREAD_H
#define TESTTHREAD_H

#include <processbuffer.h>
#include <linux/kthread.h>

struct task_struct *test_init();

void test_send(void *data);

void test_cleanup(struct task_struct *thread);

#endif // TESTTHREAD_H
