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
#include <string.h>

void dname_sha256(char *input, struct dname_digest *digest) {

    // Calculate the SHA256sum of arbitrary input.
    unsigned char hash[DNAME_SHA256_ARRAY];
    SHA256_CTX h;
    SHA224_Init(&h);
    SHA256_Update(&h, input, sizeof input);
    SHA256_Final(hash, &h);

    // Preserve the original 32 bit hash.
    strcpy(digest->sha256hash, hash);

    // System to translate the 32 bit raw hash
    // Into the 65 bit hexadecimal sum.
    unsigned char hex[DNAME_SHA256_POINTER];
    for (int i = 0; i < DNAME_SHA256_DIGEST; i++) {
        sprintf(hex + (i * 2), "%02x", hash[i]);
    }
    // Here we pointer index[0] of the 65 bit array
    // which will give us our DNAME_SHA256_ARRAY (64)
    // bit sum.
    strcpy(digest->sha256hash_hexadecimal, &hex[0]);
}


struct dname_digest getname(char *input) {
    struct dname_digest digest;
    dname_sha256(input, &digest);
    return digest;
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


