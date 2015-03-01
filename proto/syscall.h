#ifndef SYSCALL_H
#define SYSCALL_H

#include <processbuffer.h>
#include <linux/module.h>
#include <linux/sched.h>
#include <asm-generic/uaccess.h>

#define CO_PARAM_COUNT 6

/*
 * Parameter direction for system calls.
 * CO_PARAM_WRITE - process is passing memory address with data which kernel
 *                  should read
 * CO_PARAM_READ - process is passing memory address where kernel should write
 *                 some data for process
 * CO_PARAM_VALUE - the parameter is only value (not pointer)
 */
enum co_syscall_param_mode {
    CO_PARAM_READ,
    CO_PARAM_WRITE,
    CO_PARAM_VALUE,
    CO_PARAM_BOTH,
};

struct co_syscall_data {
    unsigned long syscall_num;
    long param[CO_PARAM_COUNT];
    unsigned long param_size[CO_PARAM_COUNT];
    enum co_syscall_param_mode param_mode[CO_PARAM_COUNT];
    void *param_data[CO_PARAM_COUNT];

    int ret_code;
};

struct co_syscall_context {
    long syscall_id;
    struct co_syscall_data *syscall;
};

/**
 * @brief co_syscall_initialize Initialize remote system calls subsystem
 * @param sock Zmq socket for system calls
 * @return Returns new context structure
 */
extern struct co_syscall_context* co_syscall_initialize(char *path);

/**
 * @brief co_syscall_cleanup
 * @param ctx
 */
void co_syscall_cleanup(struct co_syscall_context *ctx);

/**
 * @brief co_syscall_execute Execute fetched system call
 * @param ctx The co_syscall_context object. Handles pointer to socket and systemcall mutex
 */
extern void co_syscall_execute(struct co_syscall_context *ctx);

/**
 * @brief co_syscall_serialize - Serialize executed system call. This function puts whole struct co_syscall_data and required parameters (for READ direction)
 * @param ctx - co_syscall_context object. Handles pointer to socket and systemcall mutex
 */
extern void co_syscall_serialize(struct co_syscall_context *ctx);

/**
 * @brief co_syscall_deserialize - Unpack system call dara and all params form socket (with direction WRITE or BOTH)
 * @param ctx - co_syscall_context object. Handles pointer to socket and systemcall mutex
 */
extern int co_syscall_deserialize(struct co_syscall_context *ctx);

#endif // SYSCALL_H
