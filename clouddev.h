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

#ifndef CLOUDDEV_H
#define CLOUDDEV_H

#include <linux/pid.h>
#include <linux/fs.h>
#include <asm-generic/uaccess.h>
#include <asm/errno.h>
#include <processbuffer.h>

int device_init(void);
void device_cleanup(void);

#endif // CLOUDDEV_H
