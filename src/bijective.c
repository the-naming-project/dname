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

#include "dname.h"
#include "bijective.h"
#include "names.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>


// We know that the hash digest is 32 bytes long.
//
// 32 bytes * 8 bits = 256 bits
//
// 8 Bytes
// [  0]  [  1]  [  2]  [  3]  [  4]  [  5]  [  6]  [  7]
// [ 42]  [ 97]  [211]  [ 12]  [217]  [114]  [ 33]  [  8]
// [255]  [255]  [255]  [255]  [255]  [255]  [255]  [255]

// More notes for consideration: There are 44 sounds in english.

// dname_bijection is a deterministic bijective function
// for a 32 byte sha256 digest.
// We need to be VERY careful with memory allocation here
// so we do not end up with any memory leaks.
void dname_bijection(struct dname_digest *digest) {
    int j;
    char *name = malloc(DNAME_SHA256_DIGEST_32 * DNAME_SHA256_DIGEST_32 + 1);
    for (size_t i = 0; i < DNAME_SHA256_DIGEST_32; i = i + 2) {
        int ii = i + 1;
        int p;
        p = dname_pair(digest->sha256hash[i], digest->sha256hash[ii]);
        // 0 >= p >= 130,560
        char *n = getnamei(p);
        //printf("%d\n", p);
        if (strlen(name) == 0) {
            sprintf(name,"%s", n);
        }else {
            sprintf(name,"%s-%s", name, n);
        }

    }

    // Copy
    digest->name = malloc(DNAME_SHA256_DIGEST_32 * DNAME_SHA256_DIGEST_32 + 1);
    strcpy(digest->name, name);
    free(name);
}

// dname_pair implements the Cantor pairing function for
// deterministically pairing integers together.
//
// Note: this doesn't seem to be sane for small integers (0, 1, 2, 3)
// we should add some cases for them and build some tests.
// (3+3)/2 = 6/2 = 3
//  3/2 + 3/2 = 1 + 1 = 2
//
// p <= 130,560
// p = dname_pair(k1, k2)
// 130560 = dname_pair(255, 255)
int dname_pair(int k1, int k2) {
    return (k1+k2)*(k1+k2+1)/2 + k2;
}

