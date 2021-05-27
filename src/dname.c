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
#include "version.h"
#include <stdio.h>
#include <string.h>
#include <openssl/sha.h>


char* dname(char *input) {
    unsigned char hash[SHA256_DIGEST_LENGTH];
    SHA256_CTX sha256;
    SHA224_Init(&sha256);
    SHA256_Update(&sha256, input, sizeof input);
    SHA256_Final(hash, &sha256);
    char outputBuffer[65];
    size_t i;
    for (i = 0; i < SHA256_DIGEST_LENGTH; i++) {
        sprintf(outputBuffer + (i * 2), "%02x", hash[i]);
    }
    char *output = &outputBuffer[0];
    return output;
}

void about() {
    printf("---------------------------------------\n");
    printf("       __        v%s                   \n", VERSION);
    printf("  ____/ /___  ____ _____ ___  ___      \n");
    printf(" / __  / __ \\/ __ '/ __ '__ \\/ _ \\  \n");
    printf("/ /_/ / / / / /_/ / / / / / /  __/     \n");
    printf("\\__,_/_/ /_/\\__,_/_/ /_/ /_/\\___/   \n");
    printf("                                       \n");
    printf("   Author: Kris Nóva <kris@nivenly.com>\n");
    printf("   Deterministic Naming Library        \n");
    printf("---------------------------------------\n");
}


