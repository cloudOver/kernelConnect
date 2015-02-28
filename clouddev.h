#ifndef CLOUDDEV_H
#define CLOUDDEV_H

#include <linux/fs.h>
#include <processbuffer.h>
#include <asm/errno.h>

int device_init();
void device_cleanup();

#endif // CLOUDDEV_H
