/*
 * syscalls-cloudover.h - CloudOver modification for system calls
 *
 * Copyright (c) 2015 Maciej Nabozny
 * Copyright (c) 2004 Open Source Development Labs
 *
 * This file is released under the GPLv2.
 */

#ifndef _LINUX_SYSCALLS_CLOUDOVER_H
#define _LINUX_SYSCALLS_CLOUDOVER_H

#ifdef SYSCALL_DEFINEx
#undef SYSCALL_DEFINEx
#endif

#ifdef SYSCALL_DEFINEx
#error "SYSCALL_DEFINEx is still defined!"
#else
#error "Syscall not defined"
#endif

#define __SYSCALL_DEFINEx(x, name, ...)                                 \
    asmlinkage long sys##name(__MAP(x,__SC_DECL,__VA_ARGS__))           \
        __attribute__((alias(__stringify(SyS##name))));                 \
/* CloudOver syscall registration */                                    \
asmlinkage long (*cloudover_ptr_##name)(__MAP(x,__SC_LONG,__VA_ARGS__)) = NULL; \
asmlinkage void cloudover_set_##name(void *ptr);                        \
asmlinkage void cloudover_set_##name(void *ptr) {                       \
    cloudover_ptr_##name = ptr;                                         \
}                                                                       \
EXPORT_SYMBOL(cloudover_set_##name);                                    \
/* Syscall definition */                                                \
    static inline long SYSC##name(__MAP(x,__SC_DECL,__VA_ARGS__));      \
    asmlinkage long SyS##name(__MAP(x,__SC_LONG,__VA_ARGS__));          \
    asmlinkage long SyS##name(__MAP(x,__SC_LONG,__VA_ARGS__))           \
    {                                                                   \
        long ret = 0;                                                   \
        if (current != NULL &&                                          \
            current->co_is_traced == 0x01)                              \
            printk(KERN_ALERT "" #name "\n");                           \
                                                                        \
        if (cloudover_ptr_##name != NULL &&                             \
            current != NULL &&                                          \
            current->co_is_traced == 0x01)                              \
            ret = cloudover_ptr_##name(__MAP(x,__SC_CAST,__VA_ARGS__)); \
        else                                                            \
            ret = SYSC##name(__MAP(x,__SC_CAST,__VA_ARGS__));	        \
        __MAP(x,__SC_TEST,__VA_ARGS__);				                    \
        __PROTECT(x, ret,__MAP(x,__SC_ARGS,__VA_ARGS__));	            \
        return ret;						                                \
    }                                                                   \
    static inline long SYSC##name(__MAP(x,__SC_DECL,__VA_ARGS__))

#endif
