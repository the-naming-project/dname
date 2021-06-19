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

#include "dname.h"
#include <unistd.h>
#include <stdio.h>
#include <string.h>
// #include <stdbool.h> // TODO use true/false insteadd of 0/1


void usage() {
    about();
    printf("\n");
    printf("Usage:\n");
    printf("dname [options...] <input>\n");
    printf("\n");
    printf("[Options]\n");
    printf("\n");
    printf("  -h               print the usage and help screen.\n");
    printf("  -x               print the hexidecimal value of the 32 byte bash.\n");
    printf("  -j               print the entire digest as JSON to stdout.\n");
    printf("\n");
    printf("[Flags]\n");
    printf("\n");
    printf("  -s <string>      add an optional salt string to any input.\n");
    printf("\n");
}

/**
 * main()
 *
 * The main entrypoint of the dname command line tool.
 *
 * @param argc
 * @param argv
 * @return
 */
int main (int argc, char **argv) {

    // --------------------------
    // [Options]
    //
    int help = 0;
    int json = 0;
    int hex = 0;
    int raw = 0;
    int invalid = 0;
    int hasValue = 0;
    char *input = NULL;
    char *salt;
    int option;
    //
    // --------------------------

    while (optind < argc) {
        if ((option = getopt(argc, argv, "hjrs:x")) != -1) {
            switch (option) {
                case 'h':
                    usage();
                    return 1;
                case 'r':
                    raw = 1;
                case 'j':
                    json = 1;
                    break;
                case 'x':
                    hex = 1;
                case 's':
                    salt = optarg;
                    break;
                // This is the system to handle missing or incomplete
                // command line flags.
                // We only return usage() and let the library echo to stderr.
                case '?':
                default:
                    usage();
                    return 1;
            }
        } else {
            // This is the system that will handle
            // command line values other than flags
            // optind = 0 : dname
            // optind = 1 : <input>
            if (optind == 1) {
                hasValue = 1;
                input = argv[optind];
                optind++;
            }
        }
        optind++;
    }

    // Find the digest based on input
    struct dname_digest digest;
    if (hasValue == 0) {
        // hasValue = FALSE
        // $ dname [options]
        if (raw == 1) {
            char *raw = dname_lookup_raw();
            printf("%s\n", raw);
            return 0;
        }
        digest = dname_lookup();
    } else {
        // hasValue = TRUE
        // $ dname [options] <input>
        digest = dname(input);
    }

    // JSON
    if (json == 1) {
        dname_json_print(&digest);
        return 0;
    }

    // Hex
    if (hex == 1){
        for (size_t i = 0; i < DNAME_SHA256_DIGEST_32; i++) {
            printf("%x", digest.sha256hash[i]);
        }
        printf("\n");
        return 0;
    }

    // Dname
    printf("%s\n", digest.name);
}