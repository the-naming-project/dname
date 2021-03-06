// Copyright © 2021 Kris Nóva <kris@nivenly.com>
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.
//
//  ███╗   ██╗ ██████╗ ██╗   ██╗ █████╗
//  ████╗  ██║██╔═████╗██║   ██║██╔══██╗
//  ██╔██╗ ██║██║██╔██║██║   ██║███████║
//  ██║╚██╗██║████╔╝██║╚██╗ ██╔╝██╔══██║
//  ██║ ╚████║╚██████╔╝ ╚████╔╝ ██║  ██║
//  ╚═╝  ╚═══╝ ╚═════╝   ╚═══╝  ╚═╝  ╚═╝

#include "linux.h"
#include "dname.h"
#include "stdio.h"
#include <dirent.h>
#include <malloc.h>
#include <string.h>

/**
 * dname_linux_lookup()
 *
 * Will build the struct of parameters that can be used to identify
 * a process at runtime.
 *
 * @return
 */
struct dname_linux_lookup linux_lookup() {
  struct dname_linux_lookup lookup;
  lookup.inContainer = in_container();
  lookup.blockDeviceSerials = block_device_serials();
  lookup.containerImage = container_image();
  lookup.containerName = container_name();
  return lookup;
}

/**
 * dname_linux_string()
 *
 * Will get the string we use to identify this particular instance of dname()
 *
 * @param lookup
 * @return char*
 */
char *dname_linux_string(struct dname_linux_lookup *lookup) {
  char *linux_str = malloc(1024);
  sprintf(linux_str, "%s-%d", lookup->blockDeviceSerials, lookup->inContainer);
  return linux_str;
}

/**
 * container_name()
 *
 * If running inside a container will get the container name.
 *
 * @return char*
 */
char *container_name() {
    char *name = malloc(512);
    return name;
}

/**
 * container_image()
 *
 * If running inside a container will get the container image name.
 *
 * @return char*
 */
char *container_image() {
    char *image = malloc(512);
    return image;
}


/**
 * block_device_serials()
 *
 * Will create a unique string based on the block device serial numbers on the host.
 *
 * Note: This will cause a new name to be given if a machine has had new physical volumes
 * installed, changed, or mutated!
 *
 * @return char*
 */
char *block_device_serials() {
    // We can check /sys/block for block device details (this is where lsblock looks)
    char *serials = malloc(512);
    DIR *dir;
    struct dirent *dp;
    dir = opendir(DNAME_SYS_BLOCK_DEVICES);
    if (dir != NULL) {
        while (dp = readdir(dir)){
            if (dp->d_name[0] == '.') {
                continue;
            }
            char *subpath = malloc(256);
            sprintf(subpath, "%s/%s/device/serial", DNAME_SYS_BLOCK_DEVICES, dp->d_name);
            char *serial = static_file_contents(subpath, 256);
            if (serial == NULL) {
                continue;
            }
            if (strlen(serials) == 0){
                sprintf(serials, "%s", serial);
            }else {
                sprintf(serials, "%s-%s", serials, serial);
            }
            free(subpath);
            free(serial);
        }
        closedir(dir);
    }
    else {
        fputs("Unable to open /sys/block for hardware information.\n", stderr);
        return serials;
    }

    // Strip
    strip(serials, '\n');
    strip(serials, ' ');

    // Keep things deterministic
    return serials;
}


/**
 * in_container()
 *
 * Return a bool if we believe we are running in a "container" or not.
 *
 * @return bool
 */
int in_container() {
    // We can use /proc/1/cgroup to check and see if we are in a container
    //
    //      host:    0::/init.scope
    // container:    0::/
    char *pid1cgroup = static_file_contents(DNAME_PROC_1_CGROUP, 1024);
    for (int i = 0; i < 1024; i = i + 1) {
        // Ascii Chars
        // -----------
        // 47  |  "/"
        // 10  |  "LF" Line Feed
        // 0   |  "NULL"
        // -----------
        //printf("%d %d \n", pid1cgroup[i - 1], pid1cgroup[i]);


        // Check if the last two bits are "/" and "NULL"
        if (pid1cgroup[i - 1] == 47 && pid1cgroup[i] == 10 ){
            return 1;
        }
        // Check if the last two bits are "/" and "LF"
        if (pid1cgroup[i - 1] == 47 && pid1cgroup[i] == 0 ){
            return 1;
        }
    }
    return 0;
}

/**
 * static_file_contents()
 *
 * Get the content of a /proc file by size.
 *
 * /proc and /sys on linux is a bit tricky, we are unable to use clever
 * dynamic tricks like we can on regular files. So we simplify
 * the problem and just make the user pass in a buffer size to
 * read.
 *
 * @param char *path
 * @param int size
 * @return char*
 */
char *static_file_contents(char *path, int size) {
    char *buffer = malloc(size);
    FILE *fd=fopen(path,"r");
    fgets(buffer,size,fd);
    fclose(fd);
    return buffer;
}

/**
 * dynamic_file_contents()
 *
 * Will read a file dynamically and return the content.
 *
 * @param char *path
 * @return char*
 */
char *dynamic_file_contents(char *path) {
    long int size = 0;
    FILE *file = fopen(path, "r");
    if(!file) {
        //fputs("Unable to open file\n", stderr);
        return NULL;
    }

    // Seek to the end of the file to get the length
    // and move the stack pointer back to the beginning.
    fseek(file, 0, SEEK_END);
    size = ftell(file);
    rewind(file);

    char *content = (char *) malloc(size);
    if(!content) {
        //fputs("Memory error.\n", stderr);
        fclose(file);
        return NULL;
    }

    if(fread(content, 1, size, file) != size) {
        //fputs("Read error.\n", stderr);
        fclose(file);
        return NULL;
    }

    fclose(file);
    return content;
}

/**
 * strip()
 *
 * Remove a character from a string.
 *
 * @param dest
 * @param c
 * @return
 */
int strip(char *dest, char c) {
    int removed=0;
    char *tmp;
    while (*dest) {
        tmp=strchr(dest,c);
        if (NULL==tmp) {
            break;
        } else {
            size_t len=strlen(tmp+1);
            memmove(tmp,tmp+1,len);
            tmp[len]=0;
            ++removed;
            dest=tmp;
        }
    }
    return removed;
}