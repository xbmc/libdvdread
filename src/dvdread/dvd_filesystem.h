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

#ifndef DVDREAD_FILESYSTEM_H_
#define DVDREAD_FILESYSTEM_H_

/*
 * directory access
 */

// Our dirent struct only contains the parts we care about.
typedef struct
{
    char    d_name[256];
} DVD_DIRENT;

typedef struct dvd_dir_s DVD_DIR_H;
struct dvd_dir_s
{
    void* internal;
    void (*close)(DVD_DIR_H *dir);
    int (*read)(DVD_DIR_H *dir, DVD_DIRENT *entry);
};

typedef DVD_DIR_H* (*dvd_reader_dir_cb) (const char* dirname);


#endif /* DVDREAD_FILESYSTEM_H_ */
