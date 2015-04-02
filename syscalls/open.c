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
    printk(KERN_INFO "cloudover_truncate\n");
    struct co_syscall_context *ctx = co_syscall_initialize();

    char *_path = kmalloc(4096, GFP_KERNEL);
    copy_from_user(_path, path, 4096);
    ctx->syscall->param[0] = _path;
    ctx->syscall->param_mode[0] = CO_PARAM_WRITE;
    ctx->syscall->param_size[0] = strlen(_path);

    ctx->syscall->param[1] = length;
    ctx->syscall->param_mode[1] = CO_PARAM_VALUE;

    co_syscall_serialize(ctx);
    co_syscall_deserialize(ctx);
    printk(KERN_DEBUG "cloudover_truncate: return %d\n", ctx->syscall->ret_code);
    return ctx->syscall->ret_code;
}

long cloudover_access(void *filename, int mode, int flags) {
    printk(KERN_ALERT"access\n");
    char _filename[4096];
    printk(KERN_ALERT"copy\n");
    copy_from_user(_filename, filename, 4096);
    _filename[4095] = 0x00;

    unsigned long int params[6] = {(unsigned long int) _filename, mode, flags};
    unsigned long int params_size[6] = {strlen(_filename), 0x00, 0x00};

    printk(KERN_ALERT"directions\n");
    int params_dir[6] = {DIRECTION_to_kernel, DIRECTION_parameter, DIRECTION_parameter};

    printk(KERN_ALERT"call\n");
    int ret = cloudover_rpc_call(__NR_access, 2, params, params_size, params_dir);

    printk(KERN_ALERT"return\n");
    return ret;
}


long cloudover_open(void *path, int mode, int flags) {
    printk(KERN_ALERT"open\n");
    char _path[4096];
    copy_from_user(_path, path, 4096);
    unsigned long int params[6] = {(unsigned long int)_path, mode, flags};
    unsigned long int params_size[6] = {strlen(_path)};
    int params_dir[6] = {DIRECTION_to_kernel, DIRECTION_parameter, DIRECTION_parameter};
    return cloudover_rpc_call(__NR_open, 3, params, params_size, params_dir);
}


long cloudover_close(int fd) {
    printk(KERN_ALERT"close\n");
    unsigned long int params[6] = {fd};
    unsigned long int params_size[6] = {0x00};
    int params_dir[6] = {DIRECTION_parameter};
    return cloudover_rpc_call(__NR_close, 1, params, params_size, params_dir);
}
