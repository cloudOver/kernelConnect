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

#include <cloudover/open.h>

long cloudover_read(int fd, void *buf, size_t count) {
    printk(KERN_DEBUG "cloudover_read\n");
    struct co_syscall_context *ctx = co_syscall_initialize();

    char *_path = kmalloc(4096, GFP_KERNEL);
    strncpy_from_user(_path, path, 4096);
    ctx->syscall->param[0] = _path;
    ctx->syscall->param_mode[0] = CO_PARAM_WRITE;
    ctx->syscall->param_size[0] = strlen(_path);

    ctx->syscall->param[1] = length;
    ctx->syscall->param_mode[1] = CO_PARAM_VALUE;

    ctx->syscall->syscall_num = __NR_read;

    co_syscall_serialize(ctx);
    co_syscall_deserialize(ctx);
    printk(KERN_DEBUG "cloudover_truncate: return %d\n", ctx->syscall->ret_code);
    return ctx->syscall->ret_code;


    printk(KERN_ALERT"read\n");
    char *tmp = kmalloc(count, GFP_KERNEL);
    
    unsigned long int params[6] = {fd, (unsigned long int) tmp, count};
    unsigned long int params_size[6] = {0x00, count, 0x00};
    int params_dir[6] = {DIRECTION_parameter, DIRECTION_to_process, DIRECTION_parameter};

    int retval = cloudover_rpc_call(__NR_read, 3, params, params_size, params_dir);

    long i, sum = 0;
    for (i = 0; i < count; i++) {
        sum += tmp[i];
    }
    printk(KERN_ALERT"control sum: %d\n", sum);

    copy_to_user(buf, tmp, retval);
    kfree(tmp);
    return retval;
}

long cloudover_write(int fd, void *buf, size_t count) {
    printk(KERN_ALERT"write\n");
    char *tmp = kmalloc(count, GFP_KERNEL);
    copy_from_user(tmp, buf, count);
    unsigned long int params[6] = {fd, (unsigned long int) tmp, count};
    unsigned long int params_size[6] = {0x00, count, 0x00};
    int params_dir[6] = {DIRECTION_parameter, DIRECTION_to_kernel, DIRECTION_parameter};

    int retval = cloudover_rpc_call(__NR_write, 3, params, params_size, params_dir);
    kfree(tmp);
    return retval;
}

