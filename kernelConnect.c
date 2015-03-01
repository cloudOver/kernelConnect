#include <linux/module.h>
#include <linux/init.h>

#include <processbuffer.h>
#include <clouddev.h>

static int kernelConnect_init() {
    device_init();
}

static void kernelConnect_cleanup() {
    device_cleanup();
}


module_init(kernelConnect_init);
module_cleanup(kernelConnect_cleanup);

MODULE_LICENSE("GPL");
