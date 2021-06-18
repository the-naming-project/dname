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
#include "bijective.h"
#include "version.h"
#include <stdio.h>
#include <openssl/sha.h>
#include <string.h>
#include <stdlib.h>


// dname_sha256 will only use memory allocation in the openssl
// libraries and otherwise will be a static memory system.
void dname_sha256(char *input, struct dname_digest *digest) {
    // Calculate the SHA256sum of arbitrary input.
    char hash[DNAME_SHA256_POINTER_65];
    SHA256_CTX h;
    SHA224_Init(&h);
    SHA256_Update(&h, input, strlen(input)); // Use strlen() instead of sizeof so we can handle "multiple input words"
    SHA256_Final(hash, &h);

    // Preserve the original 32 bit hash, without allocating anymore memory.
    strcpy(digest->sha256hash, hash);
}

// dname_json_print will json print the digest
void dname_json_print(struct dname_digest *digest) {
    printf("{\n");
    printf("\t\"input\": \"%s\",\n", digest->input);
    printf("\t\"name\": \"%s\",\n", digest->name);
    char *hash = malloc(DNAME_SHA256_DIGEST_32);
    for (size_t i = 0; i < DNAME_SHA256_DIGEST_32; i++) {
        sprintf(hash,"%s%x", hash, digest->sha256hash[i]);
    }
    printf("\t\"hex\": \"%s\"\n", hash); // No trailing comma!
    printf("}\n");
}


// dname_pretty_print will just show the contents of the digest
void dname_pretty_print(struct dname_digest *digest) {
    printf("\n");
    for (size_t i = 0; i < strlen(digest->input) + 3; i++) {
        printf("-");
    }
    printf("\n");
    printf("[%s]\n", digest->input);
    for (size_t i = 0; i < strlen(digest->input) + 3; i++) {
        printf("-");
    }
    printf("\n\n");
    printf("Memory Address: *%p\n", digest);
    printf("\n");

    // Raw
    printf("Raw Integer Values:\n");
    for (size_t i = 0; i < DNAME_SHA256_DIGEST_32; i++) {
        printf("%d ", digest->sha256hash[i]);
    }
    printf("\n");

    // Hex
    printf("Hexadecimal Values:\n");
    for (size_t i = 0; i < DNAME_SHA256_DIGEST_32; i++) {
        printf("%x ", digest->sha256hash[i]);
    }
    printf("\n");

    // Hex String
    printf("Hexadecimal String:\n");
    for (size_t i = 0; i < DNAME_SHA256_DIGEST_32; i++) {
        printf("%x", digest->sha256hash[i]);
    }
    printf("\n");

    // Name
    printf("Name:\n");
    printf("%s\n", digest->name);

}

struct dname_digest dname_discover() {
    return dname("");
}



// dname()
//
// A determinstic function that will return a unique name digest
// based on the given input.
struct dname_digest dname(char *input) {
    struct dname_digest digest;

    // System to name our digest
    // Malloc here for input
    digest.input = malloc(strlen(input)+1);
    strcpy(digest.input, input);

    // Deterministic sum with sha256
    dname_sha256(input, &digest);

    // Bijection function
    dname_bijection(&digest);
    return digest;
}

// about will share information about the library.
void about() {
    // 80 char width
    printf("********************************************************************************\n");
    printf("                           __        libdname.so.%s                             \n", VERSION);
    printf("                      ____/ /___  ____ _____ ___  ___                           \n");
    printf("                     / __  / __ \\/ __ '/ __ '__ \\/ _ \\                       \n");
    printf("                    / /_/ / / / / /_/ / / / / / /  __/                          \n");
    printf("                    \\__,_/_/ /_/\\__,_/_/ /_/ /_/\\___/                        \n");
    printf("                                                                                \n");
    printf("                       Author: Kris Nóva <kris@nivenly.com>                     \n");
    printf("                       Deterministic Naming Library                             \n");
    printf("********************************************************************************\n");
    printf("\n");
    printf("DNAME is based on the SHA-256 hashing algorithm. DNAME will calculate a\n");
    printf("truncated (8 bytes) bijective name for a given input.\n");
    printf("If no input is defined DNAME will build the input at runtime based on the state\n");
    printf("of the linux filesystem (/proc) at runtime.\n");
    printf("DNAME will return a deterministic name based on several variables.\n");
    printf("See github.com/the-naming-project/dname for more details on what DNAME uses to\n");
    printf("discover an input.\n");
}


