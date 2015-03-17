MODULE_AUTHOR("Maciej Nabozny <mn@mnabozny.pl>");
MODULE_DESCRIPTION("CloudOver kernel driver");
MODULE_LICENSE("GPL");
MODULE_VERSION("0.1");


#include <linux/module.h>
#include <linux/init.h>

#include <processbuffer.h>
#include <clouddev.h>
#include <testthread.h>

static task_struct *thread = NULL;

static int kernelConnect_init() {
    device_init();
    thread = test_init();
}

static void kernelConnect_cleanup() {
    device_cleanup();
    test_cleanup(thread);
}


module_init(kernelConnect_init);
module_cleanup(kernelConnect_cleanup);
