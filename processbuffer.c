#include "processbuffer.h"

LIST_HEAD(outgoing_buffer);
LIST_HEAD(incoming_buffer);

static DEFINE_SPINLOCK(out_buf_lock);
static DEFINE_SPINLOCK(inc_buf_lock);


struct message *message_new(void *data, long size) {
    struct message *msg = kmalloc(sizeof(struct message), GFP_KERNEL);
    if (msg == NULL)
        return NULL;

    msg->data = data;
    msg->size = size;
    msg->pid = task_pid_nr(current);

    INIT_LIST_HEAD(&msg->list);
    return msg;
}

void message_destroy(struct message *msg) {
    kfree(msg);
}

void message_send(struct message *msg) {
    unsigned long flags;
    spin_lock_irqsave(&out_buf_lock, flags);

    list_add_tail(&msg->list, &outgoing_buffer);

    spin_unlock_irqrestore(&out_buf_lock, flags);
}

struct message *message_get_sent() {
    unsigned long flags;
    spin_lock_irqsave(&out_buf_lock, flags);

    struct message *msg = list_entry(&outgoing_buffer, struct message, list);
    list_del(&msg->list);

    spin_unlock_irqrestore(&out_buf_lock, flags);

    return msg;
}

struct message *message_get() {
    struct list_head* tmp;
    unsigned long flags;
    pid_t pid;

    pid = task_pid_nr(current);
    spin_lock_irqsave(&inc_buf_lock, flags);
    list_for_each(tmp, &incoming_buffer) {
        struct message *msg = list_entry(tmp, struct message, list);
        if (msg->pid == pid) {
            list_del(&msg->list);
            return msg;
        }
    }
    spin_unlock_irqrestore(&inc_buf_lock, flags);
    return NULL;
}

void message_put_incoming(struct message *msg) {
    unsigned long flags;
    spin_lock_irqsave(&inc_buf_lock, flags);
    list_add_tail(&msg->list, &incoming_buffer);
    spin_unlock_irqrestore(&inc_buf_lock, flags);
}
