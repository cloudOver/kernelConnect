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

#ifndef OPEN_H
#define OPEN_H

#include <asm-generic/uaccess.h>
#include <linux/module.h>
#include <linux/string.h>

extern void cloudover_set__truncate(void *);
long cloudover_truncate(void *path, long length);

// ftruncate
// truncate64
// ftruncate64
// fallocate
// faccessat
extern void cloudover_set__access(void *);
long cloudover_access(void *filename, int mode, int flags);
// chdir
// fchdir
// chroot
// fchmod
// fchmodat
// chmod
// fchownat
// chown
// lchown
// fchown

extern void cloudover_set__open(void *);
long cloudover_open(void *path, int mode, int flags);

// openat
// creat
extern void cloudover_set__close(void *);
long cloudover_close(int fd);
// vhangup
#endif // OPEN_H
