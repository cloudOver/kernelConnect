#ifndef SYSCALL_H
#define SYSCALL_H

/**
 * @brief syscall_init Reconnect system calls with new, cloudover calls
 */
void syscall_init();

/**
 * @brief syscall_cleanup Restore original system calls
 */
void syscall_cleanup();

#endif // SYSCALL_H
