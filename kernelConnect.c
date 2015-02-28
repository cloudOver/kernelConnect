#include <linux/module.h>
#include <linux/init.h>

#include <processbuffer.h>
#include <clouddev.h>

MODULE_LICENSE("GPL");

static int kernelConnect_init() {
    device_init();
}
