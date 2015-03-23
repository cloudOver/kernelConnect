#include "clouddev.h"

static ssize_t dev_read(struct file *filp, char __user *data, size_t size, loff_t *offset) {
    printk(KERN_DEBUG "dev_read: read\n");

    struct message *msg = message_get_sent();
    if (msg == NULL) {
        printk(KERN_CRIT "dev_read: cannot get next message\n");
        return -EAGAIN;
    }

    if (msg->size > size) {
        printk(KERN_INFO "dev_read: not enough space to copy message (%d). Message is %ud\n", size, msg->size);
        return -EAGAIN;
    }

    copy_to_user(data, msg->data, size);

    // TODO: free message's data
    return size;
}

static ssize_t dev_write(struct file *filp, const char __user *data, size_t size, loff_t *offset) {
    printk(KERN_DEBUG "dev_write: write\n");
    void *msg_data = kmalloc(size, GFP_KERNEL);
    copy_from_user(msg_data, data, size);
    struct message *msg = message_new(msg_data, size);

    message_put_incoming(msg);
    return size;
}

int dev_open(struct inode *inp, struct file *filp) {
    printk(KERN_DEBUG "dev_open: device opened\n");
    return 0;
}

int dev_release(struct inode *inp, struct file *filp) {
    printk(KERN_DEBUG "dev_release: device closed\n");
    return 0;
}


static struct file_operations fops = {
    .open = dev_open,
    .read = dev_read,
    .write = dev_write,
    .release = dev_release,
};

int device_init() {
    int dev = register_chrdev(109, "kernelconect", &fops);
    if (dev >= 0) {
        printk(KERN_INFO "registered kernelConnect device with major %d\n", dev);
        return 0;
    } else {
        printk(KERN_CRIT "failed to register kernelConnect device: %d\n", dev);
        return -1;
    }
}

void device_cleanup() {
    unregister_chrdev(99, "kernelconnect");
}
