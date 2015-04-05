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

long cloudover_truncate(void *path, long length) {
    printk(KERN_DEBUG "cloudover_truncate\n");
    struct co_syscall_context *ctx = co_syscall_initialize();

    char *_path = kmalloc(4096, GFP_KERNEL);
    strncpy_from_user(_path, path, 4096);
    ctx->syscall->param[0] = _path;
    ctx->syscall->param_mode[0] = CO_PARAM_WRITE;
    ctx->syscall->param_size[0] = strlen(_path);

    ctx->syscall->param[1] = length;
    ctx->syscall->param_mode[1] = CO_PARAM_VALUE;

    //TODO: syscall num

    co_syscall_serialize(ctx);
    co_syscall_deserialize(ctx);
    printk(KERN_DEBUG "cloudover_truncate: return %d\n", ctx->syscall->ret_code);
    return ctx->syscall->ret_code;
}

long cloudover_access(void *path, int mode, int flags) {
    printk(KERN_DEBUG "cloudover_access\n");
    struct co_syscall_context *ctx = co_syscall_initialize();

    char *_path = kmalloc(4096, GFP_KERNEL);
    strncpy_from_user(_path, path, 4096);

    ctx->syscall->param[0] = _path;
    ctx->syscall->param_mode[0] = CO_PARAM_WRITE;
    ctx->syscall->param_size[0] = strlen(_path);

    ctx->syscall->param[1] = mode;
    ctx->syscall->param_mode[1] = CO_PARAM_VALUE;

    ctx->syscall->param[2] = flags;
    ctx->syscall->param_mode[2] = CO_PARAM_VALUE;

    //TODO: syscall num

    co_syscall_serialize(ctx);
    co_syscall_deserialize(ctx);
    printk(KERN_DEBUG "cloudover_access: return %d\n", ctx->syscall->ret_code);
    return ctx->syscall->ret_code;
}


long cloudover_open(void *path, int mode, int flags) {
    printk(KERN_DEBUG "cloudover_open\n");
    struct co_syscall_context *ctx = co_syscall_initialize();

    char *_path = kmalloc(4096, GFP_KERNEL);
    strncpy_from_user(_path, path, 4096);

    ctx->syscall->param[0] = _path;
    ctx->syscall->param_mode[0] = CO_PARAM_WRITE;
    ctx->syscall->param_size[0] = strlen(_path);

    ctx->syscall->param[1] = mode;
    ctx->syscall->param_mode[1] = CO_PARAM_VALUE;

    ctx->syscall->param[2] = flags;
    ctx->syscall->param_mode[2] = CO_PARAM_VALUE;

    //TODO: syscall num

    co_syscall_serialize(ctx);
    co_syscall_deserialize(ctx);
    printk(KERN_DEBUG "cloudover_open: return %d\n", ctx->syscall->ret_code);
    return ctx->syscall->ret_code;
}


long cloudover_close(int fd) {
    printk(KERN_DEBUG "cloudover_close\n");
    struct co_syscall_context *ctx = co_syscall_initialize();

    ctx->syscall->param[0] = fd;
    ctx->syscall->param_mode[0] = CO_PARAM_VALUE;

    ctx->syscall->syscall_num = __NR_open;
    //TODO: syscall num

    co_syscall_serialize(ctx);
    co_syscall_deserialize(ctx);
    printk(KERN_DEBUG "cloudover_close: return %d\n", ctx->syscall->ret_code);
    return ctx->syscall->ret_code;
}
