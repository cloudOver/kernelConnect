/**
Copyright (c) 2015 Maciej Nabozny

This file is part of KernelConnect project.

KernelConnect is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

KernelConnect is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with KernelConnect.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "clouddev.h"

static ssize_t dev_read(struct file *filp, char __user *data, size_t size, loff_t *offset) {
    struct message *msg;
    printk(KERN_DEBUG "dev_read: read\n");

    msg = message_get_sent();
    if (msg == NULL) {
        printk(KERN_CRIT "dev_read: cannot get next message\n");
        return 0;
    }

    if (msg->size > size) {
        printk(KERN_INFO "dev_read: not enough space to copy message (%lu). Message is %zu\n", size, msg->size);
        return -EAGAIN;
    }

    copy_to_user(data, msg->data, msg->size);

    message_destroy(msg);
    return size;
}


static ssize_t dev_write(struct file *filp, const char __user *data, size_t size, loff_t *offset) {
    struct message *msg;
    void *msg_data;
    printk(KERN_DEBUG "dev_write: write\n");
    msg_data = kmalloc(size, GFP_KERNEL);

    msg = message_new(msg_data, size);

    printk(KERN_DEBUG "Message pid size: %lu\n", sizeof(msg->pid));

    copy_from_user(&msg->pid, data, sizeof(msg->pid));
    copy_from_user(&msg->data, data + sizeof(msg->pid), size - sizeof(msg->pid));

    message_put_incoming(msg);
    return size;
}

int dev_ioctl(struct file *filp, unsigned int func, unsigned long data) {
    // TODO: Lock
    struct pid *p;
    struct task_struct *task;

    printk(KERN_INFO "dev_ioctl: ");

    p = find_get_pid(data);
    task = get_pid_task(p, PIDTYPE_PID);
    task->cloudover_flags = 0x01;
    // TODO: Unlock
    return 0;
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
    .unlocked_ioctl = dev_ioctl,
};

int device_init(void) {
    int dev;
    dev = register_chrdev(109, "kernelconect", &fops);
    if (dev >= 0) {
        printk(KERN_INFO "device_init: registered kernelConnect device with major %d\n", dev);
        return 0;
    } else {
        printk(KERN_CRIT "device_init: failed to register kernelConnect device: %d\n", dev);
        return -1;
    }
}

void device_cleanup(void) {
    unregister_chrdev(109, "kernelconnect");
}
