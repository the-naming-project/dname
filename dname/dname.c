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

#include "name.h"
#include <stdio.h>

void usage() {
    about();
    printf("\n");
    printf("Usage:\n");
    printf("dname [options...] <input>\n");
    printf("\n");
}

int main (int argc, char **argv) {
    if (argc < 2) {
        usage();
        return 1;
    }
    char* input = argv[1];
    printf("[Input ]: %s\n", input);
    struct dname_digest digest;
    digest = getname(input);
    printf("[Output]: %s\n", digest.sha256hash_hexadecimal);
    return 0;
}