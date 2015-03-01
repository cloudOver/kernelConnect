#ifndef CLOUDDEV_H
#define CLOUDDEV_H

#include <linux/fs.h>
#include <asm-generic/uaccess.h>
#include <asm/errno.h>
#include <processbuffer.h>

int device_init();
void device_cleanup();

#endif // CLOUDDEV_H
