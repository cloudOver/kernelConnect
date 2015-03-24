#include <testthread.h>

static int thread_count = 0;
char str[] = "Hello world!";

struct task_struct *test_init() {
    static struct task_struct *test_thread;

    thread_count += 1;
    test_thread = kthread_run(test_send, (void *)thread_count, "kernelConnect_test_%d", thread_count);
    return test_thread;
}

void test_send(void *data) {
    while (!kthread_should_stop()) {
        int len = strlen(str);

        printk(KERN_INFO "test_send: sending message (%d bytes, at %p)\n", len, str);
        struct message *msg = message_new((void *) str, len);
        message_send(msg);

        printk(KERN_INFO "test_send: message has been sent (%u bytes, at %p)\n", msg->size, msg->data);


        struct message *incoming = message_get();
        if (incoming != NULL) {
            printk(KERN_INFO "test_send: received new message for pid: %d, data at: %p\n", incoming->pid, incoming->data);
        }
        msleep(5000);
    }
    // kthread_should_stop...
    return;
}

void test_cleanup(struct task_struct *thread) {
    kthread_stop(thread);
}
