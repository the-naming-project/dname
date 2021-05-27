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
#include <openssl/sha.h>

// sha256_input is used to calculate a sha256
// sum based on a given pointer input.
char* sha256_input(char *input) {
    unsigned char hash[SHA256_DIGEST_LENGTH];
    SHA256_CTX sha256;
    SHA224_Init(&sha256);
    SHA256_Update(&sha256, input, sizeof input);
    SHA256_Final(hash, &sha256);
    unsigned char outputBuffer[65];
    size_t i;
    for (i = 0; i < SHA256_DIGEST_LENGTH; i++) {
        sprintf(outputBuffer + (i * 2), "%02x", hash[i]);
    }
    // Take the pointer of the buffer and return
    char *output = &outputBuffer[0];
    return output;
}


// dname is used to pass in a pointer to a string
// and receive the deterministic name from the given
// input.
char* dname(char *input) {
    return sha256_input(input);
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


