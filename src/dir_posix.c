/*
 * This file is part of libdvdread
 * Copyright (C) 2022 VideoLAN
 *
 * This file is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This file is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library. If not, see
 * <http://www.gnu.org/licenses/>.
 */

#include "config.h"

#include <stdlib.h>
#include <string.h>
#if HAVE_DIRENT_H
#include <dirent.h>
#endif

#if defined(__GLIBC__) && defined(__GLIBC_MINOR__)
#  if __GLIBC__ > 2 || (__GLIBC__ == 2 && __GLIBC_MINOR__ >= 24)
#    define USE_READDIR
#    include <errno.h>
#  endif
#endif

#include <dvdread/dvd_filesystem.h>

static void _dir_close_posix(DVD_DIR_H *dir)
{
    if (dir) {
        closedir((DIR *)dir->internal);


        free(dir);
        dir = NULL;
    }
}

static void _error(const char *msg, int errnum, void *dir)
{
    char buf[128];
    if (strerror_r(errnum, buf, sizeof(buf))) {
        strcpy(buf, "?");
    }
}

static int _dir_read_posix(DVD_DIR_H *dir, DVD_DIRENT *entry)
{
    struct dirent *p_e;

#ifdef USE_READDIR
    errno = 0;
    p_e = readdir((DIR*)dir->internal);
    if (!p_e && errno) {
        _error("Error reading directory", errno, dir);
        return -1;
    }
#else /* USE_READDIR */
    int result;
    struct dirent e;

    result = readdir_r((DIR*)dir->internal, &e, &p_e);
    if (result) {
        _error("Error reading directory", result, dir);
        return -result;
    }
#endif /* USE_READDIR */

    if (p_e == NULL) {
        return 1;
    }
    strncpy(entry->d_name, p_e->d_name, sizeof(entry->d_name));
    entry->d_name[sizeof(entry->d_name) - 1] = 0;

    return 0;
}

static DVD_DIR_H *_dir_open_posix(const char* dirname)
{
    DVD_DIR_H *dir = calloc(1, sizeof(DVD_DIR_H));

    if (!dir) {
        return NULL;
    }

    dir->close = _dir_close_posix;
    dir->read = _dir_read_posix;

    if ((dir->internal = opendir(dirname))) {
        return dir;
    }

    free(dir);
    dir = NULL;

    return NULL;
}

DVD_DIR_H* (*dir_open)(const char* dirname) = _dir_open_posix;

dvd_reader_dir_cb dir_open_default(void)
{
    return _dir_open_posix;
}
