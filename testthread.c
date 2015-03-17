#include <testthread.h>

static int thread_count = 0;

struct task_struct *test_init() {
    static struct task_struct *test_thread;

    thread_count += 1;
    test_thread = kthread_run(test_send, (void *)thread_count, "kernelConnect_test_%d", thread_count);
    return test_thread;
}

void test_send(void *data) {
    printk(KERN_INFO "test_send: sending message\n");
    char str[] = "Hello world!";
    struct message *msg = message_new((void *) str, strlen(str));
    message_send(msg);

    printk(KERN_INFO "test_send: message has been sent\n");

    // kthread_should_stop...
    return;
}

void test_cleanup(struct task_struct *thread) {
    kthread_stop(thread);
}
