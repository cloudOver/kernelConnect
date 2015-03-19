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
    printk(KERN_ALERT "initializing...\n");
//    device_init();
//    thread = test_init();

    return 0;
}

void kernelConnect_cleanup(void) {
    printk(KERN_ALERT "cleanup...\n");
//    device_cleanup();
//    test_cleanup(thread);
}


module_init(kernelConnect_init);
module_cleanup(kernelConnect_cleanup);
