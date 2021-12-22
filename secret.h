#ifndef __SECRET_H
#define __SECRET_H

#include <minix/drivers.h>
#include <minix/driver.h>
#include <minix/const.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <minix/ds.h>
#include <sys/socket.h>
#include <sys/ioctl.h>

/*Left over from the hello file*/
#define SECRET_MESSAGE "This is a secret.\n"

#ifndef SECRET_SIZE
#define SECRET_SIZE 8192
#endif /*SECRET_SIZE*/


#endif /* __SECRET_H */
