#include <proto/syscall.h>

struct co_syscall_context* co_syscall_initialize() {
    struct co_syscall_context *ctx = (struct co_syscall_context*) kmalloc(sizeof(struct co_syscall_context), GFP_KERNEL);
    if (ctx == NULL) {
        printk(KERN_CRIT "co_syscall_initialize: cannot allocate memory");
        return NULL;
    }

    ctx->syscall_id = 0;
    ctx->syscall = (struct co_syscall_data*) kmalloc(sizeof(struct co_syscall_data), GFP_KERNEL);

    return ctx;
}

void co_syscall_cleanup(struct co_syscall_context *ctx) {
    kfree(ctx->syscall);
    kfree(ctx);
}

void co_syscall_serialize(struct co_syscall_context *ctx) {
    printk(KERN_DEBUG "co_syscall_serialize: serializing syscall %ld", ctx->syscall_id);

    struct message *msg = message_new(ctx->syscall, sizeof(struct co_syscall_data));
    message_send(msg);
    int i;
    for (i = 0; i < CO_PARAM_COUNT; i++) {
        // Serialize required params (READ and BOTH directions)
        if (ctx->syscall->param_mode[i] == CO_PARAM_WRITE || ctx->syscall->param_mode[i] == CO_PARAM_BOTH) {
            printk(KERN_DEBUG "co_syscall_serialize: \tsending parameter %d (%ld bytes)\n", i, ctx->syscall->param_size[i]);

            msg = message_new((void*)ctx->syscall->param[i], ctx->syscall->param_size[i]);
            message_send(msg);
        }
    }
}

int co_syscall_deserialize(struct co_syscall_context *ctx) {
    struct message *msg;
    msg = message_get();

    ctx->syscall_id += 1;
    printk(KERN_DEBUG "co_syscall_deserialize: received syscall: %ld\n", ctx->syscall->syscall_num);

    int i;
    for (i = 0; i < CO_PARAM_COUNT; i++) {
        if (ctx->syscall->param_mode[i] != CO_PARAM_VALUE) {
            printk(KERN_ALERT "co_syscall_serialize: \tallocating memory for param %d (%ld bytes)\n", i, ctx->syscall->param_size[i]);
            ctx->syscall->param[i] = (unsigned long) kmalloc(ctx->syscall->param_size[i], GFP_KERNEL);
        }

        if (ctx->syscall->param_mode[i] == CO_PARAM_READ || ctx->syscall->param_mode[i] == CO_PARAM_BOTH) {
            printk(KERN_DEBUG "co_syscall_serialize: \treceiving parameter %d", i);
            //zmq_recv(ctx->zmq_sock, (void *)ctx->syscall->param[i], ctx->syscall->param_size[i], 0);
            //message_new((void *)ctx->syscall->param[i], ctx->syscall->param_size[i]);
            struct message *msg = message_get();
            ctx->syscall->param[i] = msg->data;
            ctx->syscall->param_size[i] = msg->size;
            kfree(msg);
        }
    }

    return ctx->syscall_id;
}
