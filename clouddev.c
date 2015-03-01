#include "clouddev.h"

static ssize_t dev_read(struct file *filp, char __user *data, size_t size, loff_t *offset) {
    struct message *msg = message_get_sent();
    if (msg == NULL) {
        printk(KERN_CRIT "cannot allocate new message\n");
        return -EAGAIN;
    }

    if (msg->size > size) {
        return -EAGAIN;
    }

    copy_to_user(data, msg->data, size);
    return size;
}

static ssize_t dev_write(struct file *filp, const char __user *data, size_t size, loff_t *offset) {
    struct message *msg = message_new(size);
    copy_from_user(msg->data, data, size);
    message_put_incoming(msg);
    return size;
}

int dev_open(struct inode *inp, struct file *filp) {
    return 0;
}

int dev_release(struct inode *inp, struct file *filp) {
    return 0;
}


static struct file_operations fops = {
    .open = dev_open,
    .read = dev_read,
    .write = dev_write,
    .release = dev_release,
};

int device_init() {
    int dev = register_chrdev(99, "kernelconect", &fops);
    if (dev >= 0) {
        printk(KERN_INFO "registered kernelConnect device with major %d\n", dev);
        return 0;
    } else {
        printk(KERN_CRIT "failed to register kernelConnect device\n");
        return -1;
    }
}

void device_cleanup() {
    unregister_chrdev(99, "kernelconnect");
}
