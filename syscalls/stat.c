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

#include <cloudover/stat.h>

long cloudover_stat(void *filename, struct __old_kernel_stat *statbuf) {
    printk(KERN_ALERT"stat\n");
    char *tmp_filename = kmalloc(4096, GFP_KERNEL);
    char *tmp_statbuf = kmalloc(sizeof(struct __old_kernel_stat), GFP_KERNEL);

    copy_from_user(tmp_filename, filename, 4096);

    unsigned long int params[6] = {(unsigned long int) tmp_filename, (unsigned long int) tmp_statbuf};
    unsigned long int params_size[6] = {4096, sizeof(struct __old_kernel_stat)};
    int params_dir[6] = {DIRECTION_to_kernel, DIRECTION_to_process};

    int retval = cloudover_rpc_call(__NR_fstat, 2, params, params_size, params_dir);
    copy_to_user(statbuf, tmp_statbuf, sizeof(struct __old_kernel_stat));
    kfree(tmp_statbuf);
    kfree(tmp_filename);
    return retval;
}

long cloudover_fstat(int fd, struct __old_kernel_stat *statbuf) {
    printk(KERN_ALERT"fstat\n");
    char *tmp_statbuf = kmalloc(sizeof(struct __old_kernel_stat), GFP_KERNEL);

    unsigned long int params[6] = {fd, (unsigned long int) tmp_statbuf};
    unsigned long int params_size[6] = {0x00, sizeof(struct __old_kernel_stat)};
    int params_dir[6] = {DIRECTION_parameter, DIRECTION_to_process};

    int retval = cloudover_rpc_call(__NR_fstat, 2, params, params_size, params_dir);
    copy_to_user(statbuf, tmp_statbuf, sizeof(struct __old_kernel_stat));
    kfree(tmp_statbuf);
    return retval;
}

long cloudover_newstat(void *filename, struct stat *statbuf) {
    printk(KERN_ALERT"stat\n");
    char *tmp_filename = kmalloc(4096, GFP_KERNEL);
    char *tmp_statbuf = kmalloc(sizeof(struct stat), GFP_KERNEL);

    copy_from_user(tmp_filename, filename, 4096);

    unsigned long int params[6] = {(unsigned long int) tmp_filename, (unsigned long int) tmp_statbuf};
    unsigned long int params_size[6] = {4096, sizeof(struct stat)};
    int params_dir[6] = {DIRECTION_to_kernel, DIRECTION_to_process};

    int retval = cloudover_rpc_call(__NR_fstat, 2, params, params_size, params_dir);
    copy_to_user(statbuf, tmp_statbuf, sizeof(struct stat));
    kfree(tmp_statbuf);
    kfree(tmp_filename);
    return retval;
}

long cloudover_newfstat(int fd, struct stat *statbuf) {
    printk(KERN_ALERT"newstat\n");
    char *tmp_statbuf = kmalloc(sizeof(struct stat), GFP_KERNEL);

    unsigned long int params[6] = {fd, (unsigned long int) tmp_statbuf};
    unsigned long int params_size[6] = {0x00, sizeof(struct stat)};
    int params_dir[6] = {DIRECTION_parameter, DIRECTION_to_process};

    int retval = cloudover_rpc_call(__NR_fstat, 2, params, params_size, params_dir);
    copy_to_user(statbuf, tmp_statbuf, sizeof(struct stat));
    kfree(tmp_statbuf);
    return retval;
}
