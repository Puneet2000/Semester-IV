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

#include <iostream>
#include <cstring>

using namespace std;

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <sys/stat.h>        /* For mode constants */
#include <syslog.h>
#include <linux/limits.h>
#include <dirent.h>

#include <libmyshm.hpp>

// The standard shm_open(3) creates shm file in /dev/shm.
// We cannot create our shm file in /dev/shm and get calls into mykmod's
// fops/vmops. So we will use /tmp as our base directory.
// In the /tmp create a DSF using mknod with mykmod's major no,
// and an unused minor no (find by walking through /tmp
// using opendir(3), readdir(3), closedir(3)).
// Use mylock() and myunlock() to synchronize racing shm_open(,O_CREAT,)
// shm_unlink() calls.

static const char dirprefix[] = "/tmp";
static const char lockfile[] = "/tmp/.myshm.lock";

static int mylock(int lockfd, int mode);
static int myunlock(int lockfd);
#define DEFAULT 0

int myshm_open(const char *name, int oflag, mode_t mode)
{
#if DEFAULT
        return shm_open(name, oflag, mode);
#else
        char fname[256];
        strcat(fname,dirprefix);
        strcat(fname,name);
        int lockfd = open(lockfile,O_CREAT|O_RDWR);
        mylock(lockfd,S_IRWXU);
        int minor_no = 0;
        DIR *dp = opendir("/tmp");
        struct dirent *dir;
        while(dir = readdir(dp)){
                struct stat status;
                stat(dir->d_name,&status);
                //printf("%s %d\n",dir->d_name,major(status.st_dev));
                if(minor(status.st_dev) > minor_no && major(status.st_dev) == 245)
                        minor_no = minor(status.st_dev);
        }

        closedir(dp);
        dev_t dev = makedev(245,(minor_no+1)%255);
        int ret = mknod(fname,S_IFCHR,dev);
        myunlock(lockfd);
        if (ret <0 && errno == EEXIST) open(fname, oflag, mode);
        else if (ret <0) return ret;
        else return open(fname, oflag, mode);
#endif
}

int myshm_unlink(const char *name)
{
#if DEFAULT
        return shm_unlink(name);
#else
        return unlink(name);
#endif
}

static int mylock(int lockfd, int mode)
{
        struct flock fl;

        fl.l_type = mode;
        fl.l_whence = SEEK_SET;
        fl.l_start = 0;
        fl.l_len = 0;
        if (fcntl(lockfd, F_SETLKW, &fl) != 0) {
                syslog(LOG_ERR, "Not Locked\n");
                return 1;
        }
        return 0;
}

static int myunlock(int lockfd)
{
        struct flock fl;

        fl.l_type = F_UNLCK;
        fl.l_whence = SEEK_SET;
        fl.l_start = 0;
        fl.l_len = 0;
        if (fcntl(lockfd, F_SETLK, &fl) != 0) {
                syslog(LOG_ERR, "Not UnLocked\n");
                return 1;
        }
        return 0;
}

