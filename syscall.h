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

#ifndef SYSCALL_H
#define SYSCALL_H

#include <syscalls/open.h>

/**
 * @brief syscall_init Reconnect system calls with new, cloudover calls
 */
void syscall_init(void);

/**
 * @brief syscall_cleanup Restore original system calls
 */
void syscall_cleanup(void);

#endif // SYSCALL_H
