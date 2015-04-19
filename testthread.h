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

#ifndef TESTTHREAD_H
#define TESTTHREAD_H

#include <processbuffer.h>
#include <linux/kthread.h>
#include <linux/delay.h>


struct task_struct *test_init(void);

void test_send(void *data);

void test_cleanup(struct task_struct *thread);

#endif // TESTTHREAD_H
