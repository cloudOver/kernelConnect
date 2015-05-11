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

#include "processbuffer.h"

LIST_HEAD(outgoing_buffer);
LIST_HEAD(incoming_buffer);

static DEFINE_SPINLOCK(out_buf_lock);
static DEFINE_SPINLOCK(inc_buf_lock);


struct message *message_new(void *data, unsigned long size) {
    struct message *msg = kmalloc(sizeof(struct message), GFP_KERNEL);

    if (msg == NULL)
        return NULL;

    memset((void*)msg, 0x00, sizeof(struct message));

    msg->data = data;
    msg->size = size;
    msg->pid = task_pid_nr(current);

    INIT_LIST_HEAD(&msg->list);
    printk(KERN_DEBUG "message_new: created new message for pid %ud at %p (%lu bytes, at %p)\n", msg->pid, msg, msg->size, msg->data);
    return msg;
}

void message_destroy(struct message *msg) {
    kfree(msg->data);
    kfree(msg);
}

void message_send(struct message *msg) {
    unsigned long flags;
    spin_lock_irqsave(&out_buf_lock, flags);

    printk(KERN_DEBUG "message_send: adding message to buffer\n");

    list_add_tail(&msg->list, &outgoing_buffer);

    spin_unlock_irqrestore(&out_buf_lock, flags);
}

struct message *message_get_sent() {
    unsigned long flags;
    struct list_head* tmp;
    spin_lock_irqsave(&out_buf_lock, flags);

    // TODO: rewrite it to working list_entry
    list_for_each(tmp, &outgoing_buffer) {
        struct message *msg = list_entry(tmp, struct message, list);
        printk(KERN_DEBUG "message_get_sent: found message to forward at %p with data at %p\n", msg, msg->data);
        list_del(&msg->list);

        spin_unlock_irqrestore(&out_buf_lock, flags);
        return msg;
    }

    printk(KERN_DEBUG "message_get_sent: no new messages\n");
    spin_unlock_irqrestore(&out_buf_lock, flags);
    return NULL;
}

struct message *message_get() {
    struct list_head* tmp;
    unsigned long flags;
    pid_t pid;

    pid = task_pid_nr(current);
    while (1) {
        spin_lock_irqsave(&inc_buf_lock, flags);
        list_for_each(tmp, &incoming_buffer) {
            struct message *msg = list_entry(tmp, struct message, list);
            if (msg->pid == pid) {
                printk(KERN_DEBUG "message_get: found message for pid %d at %p with data at %p\n", pid, msg, msg->data);
                list_del(&msg->list);
                spin_unlock_irqrestore(&inc_buf_lock, flags);
                return msg;
            }
        }
        spin_unlock_irqrestore(&inc_buf_lock, flags);
        schedule();
    }
    spin_unlock_irqrestore(&inc_buf_lock, flags);
    return NULL;
}

void message_put_incoming(struct message *msg) {
    unsigned long flags;
    spin_lock_irqsave(&inc_buf_lock, flags);
    printk(KERN_DEBUG "message_put_incoming: received new message for PID %d at %p\n", msg->pid, msg->data);
    list_add_tail(&msg->list, &incoming_buffer);
    spin_unlock_irqrestore(&inc_buf_lock, flags);
}
