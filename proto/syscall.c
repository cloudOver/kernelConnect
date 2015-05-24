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

#include <proto/syscall.h>

struct co_syscall_context* co_syscall_initialize(void) {
    struct co_syscall_context *ctx = (struct co_syscall_context*) kmalloc(sizeof(struct co_syscall_context), GFP_KERNEL);
    if (ctx == NULL) {
        printk(KERN_CRIT "co_syscall_initialize: cannot allocate memory");
        return NULL;
    }

    ctx->syscall_id = 0;
    ctx->syscall = (struct co_syscall_data*) kmalloc(sizeof(struct co_syscall_data), GFP_KERNEL);

    memset(ctx->syscall, 0, sizeof(struct co_syscall_data));

    return ctx;
}

void co_syscall_cleanup(struct co_syscall_context *ctx) {
    kfree(ctx->syscall);
    kfree(ctx);
}

void co_syscall_serialize(struct co_syscall_context *ctx) {
    struct message *msg;
    int i;

    printk(KERN_DEBUG "co_syscall_serialize: serializing syscall %l\n", ctx->syscall_id);

    msg = message_new(ctx->syscall, sizeof(struct co_syscall_data));
    message_send(msg);
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
    struct message *param;
    int i;
    msg = message_get();

    ctx->syscall_id += 1;
    printk(KERN_DEBUG "co_syscall_deserialize: received syscall: %ld\n", ctx->syscall->syscall_num);

    for (i = 0; i < CO_PARAM_COUNT; i++) {
        if (ctx->syscall->param_mode[i] != CO_PARAM_VALUE) {
            printk(KERN_ALERT "co_syscall_serialize: allocating memory for param %d (%lud bytes)\n", i, ctx->syscall->param_size[i]);
            ctx->syscall->param[i] = (unsigned long) kmalloc(ctx->syscall->param_size[i], GFP_KERNEL);
            printk(KERN_DEBUG "co_syscall_deserialize: \tparam allocated\n");
        }

        if (ctx->syscall->param_mode[i] == CO_PARAM_READ || ctx->syscall->param_mode[i] == CO_PARAM_BOTH) {
            printk(KERN_DEBUG "co_syscall_serialize: \treceiving parameter %d", i);
            //zmq_recv(ctx->zmq_sock, (void *)ctx->syscall->param[i], ctx->syscall->param_size[i], 0);
            //message_new((void *)ctx->syscall->param[i], ctx->syscall->param_size[i]);
            param = message_get();
            ctx->syscall->param[i] = param->data;
            ctx->syscall->param_size[i] = param->size;
            kfree(param);
        }
    }

    return ctx->syscall_id;
}

void co_syscall_prepare(struct co_syscall_context *ctx) {
    int i;
    memset(ctx->syscall, 0x00, sizeof(struct co_syscall_data));
    for (i = 0; i < 6; i++) {
        ctx->syscall->param_mode[i] = CO_PARAM_VALUE;
    }
}
