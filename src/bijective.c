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


// -----------------------------------------------------------
// Each SHA-256 Digest is 32 Bytes ling.
// 32 Bytes = (32 * 8 bits) = 256 bits = {0,255}
// 32 Bytes = 8 Bytes * 4 Segments
// -----------------------------------------------------------
//
// [ 8 Bytes ] 1 Segment
// +---------------------------------------------------------+
// | [  0]  [  1]  [  2]  [  3]  [  4]  [  5]  [  6]  [  7]  |
// | [255]  [255]  [255]  [255]  [255]  [255]  [255]  [255]  |
// | [16^2] [16^2] [16^2] [16^2] [16^2] [16^2] [16^2] [16^2] |
// | [0F]   [0F]   [0F]   [0F]   [0F]   [0F]   [0F]   [0F]   |
// +---------------------------------------------------------+

// [8 Bytes] 4 Pairs (1 Pair = 0->130,560)
// +---------------------------------------------------------+
// | [  0]  [  1]  [  2]  [  3]  [  4]  [  5]  [  6]  [  7]  |
// | [0,130560  ]  [0,130560  ]  [0,130560  ]  [0,130560  ]  |
//

// dname_bijection is a deterministic bijective function for a 32 byte sha256 digest.
//
// The bijection works by iterating through 16 pairs of bytes, and then calculating
// a pairing sum of each. This ensures that each pair has a unique and deterministic
// sum.
//
void dname_bijection(struct dname_digest *digest) {
    int j;
    char *name = malloc(DNAME_SHA256_DIGEST_32 * DNAME_SHA256_DIGEST_32 + 1); // 32*32+1 1,024+1
    for (size_t i = 0; i < DNAME_SHA256_DIGEST_32; i = i + 2) {
        int ii = i + 1;
        int p;
        p = dname_pair(digest->sha256hash[i], digest->sha256hash[ii]);

        // Given P, find a name
        // There will be 16 names
        char *n = getnamei(p);

        // String cat the name
        // TODO use math instead of strings please
        printf("%d\n", p);
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
// p = dname_pair(i1, i2)
// 130560 = dname_pair(255, 255)
int dname_pair(int i1, int i2) {
    return (i1+i2) * (i1+i2+1) / 2 + i2;
}

