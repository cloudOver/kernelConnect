#ifndef PROCESSBUFFER_H
#define PROCESSBUFFER_H

#include <linux/list.h>
#include <linux/thread_info.h>
#include <linux/slab.h>
#include <linux/sched.h>
#include <linux/spinlock.h>
#include <asm/spinlock.h>

/**
 * @brief The message struct keeps single message. Use message_new and
 * message_destroy functions to create or remove message.
 */
struct message {
    int pid;
    long size;
    void *data;
    struct list_head list;
};

extern struct list_head outgoing_buffer;
extern struct list_head incoming_buffer;


/**
 * @brief message_new allocate new message struct and its data
 * @param size size of the data in the message
 * @return new message
 */
struct message *message_new(long size);


/**
 * @brief message_destroy destroys message allocated by message_new
 * @param msg pointer to message object
 */
void message_destroy(struct message *msg);

/**
 * @brief message_send Put new message into outgoing buffer
 * @param msg
 */
void message_send(struct message *msg);

/**
 * @brief message_get Get message from buffer and remove it from buffer
 * @return  pointer to message or null, if there is no new message
 */
struct message *message_get(pid_t pid);

#endif // PROCESSBUFFER_H
