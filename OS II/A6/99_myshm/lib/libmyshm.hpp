/*
 *
 * Copyright (C) 1998-2019 Maruthi Seshidhar Inukonda - All Rights Reserved.
 * maruthi.inukonda@gmail.com
 *
 * This file is released under the LGPL v2.
 *
 * This file may be redistributed under the terms of the Lesser GNU Public
 * License.
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/mman.h>

#ifndef __LIBMYSHM__
int myshm_open(const char *name, int oflag, mode_t mode);
int myshm_unlink(const char *name);
#endif

