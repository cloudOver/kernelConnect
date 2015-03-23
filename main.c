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
