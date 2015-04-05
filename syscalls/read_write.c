#include <cloudover/open.h>

long cloudover_read(int fd, void *buf, size_t count) {
    printk(KERN_ALERT"read\n");
    char *tmp = kmalloc(count, GFP_KERNEL);
    
    unsigned long int params[6] = {fd, (unsigned long int) tmp, count};
    unsigned long int params_size[6] = {0x00, count, 0x00};
    int params_dir[6] = {DIRECTION_parameter, DIRECTION_to_process, DIRECTION_parameter};

    int retval = cloudover_rpc_call(__NR_read, 3, params, params_size, params_dir);

    long i, sum = 0;
    for (i = 0; i < count; i++) {
        sum += tmp[i];
    }
    printk(KERN_ALERT"control sum: %d\n", sum);

    copy_to_user(buf, tmp, retval);
    kfree(tmp);
    return retval;
}

long cloudover_write(int fd, void *buf, size_t count) {
    printk(KERN_ALERT"write\n");
    char *tmp = kmalloc(count, GFP_KERNEL);
    copy_from_user(tmp, buf, count);
    unsigned long int params[6] = {fd, (unsigned long int) tmp, count};
    unsigned long int params_size[6] = {0x00, count, 0x00};
    int params_dir[6] = {DIRECTION_parameter, DIRECTION_to_kernel, DIRECTION_parameter};

    int retval = cloudover_rpc_call(__NR_write, 3, params, params_size, params_dir);
    kfree(tmp);
    return retval;
}

