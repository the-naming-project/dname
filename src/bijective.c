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
#include "names.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>


/**
 * dname_bijection()
 *
 * Take a 32 byte digest from sha-256 and create an 8 byte
 * bijective name from the name list.
 *
 * ----------------------------------------------------------+
 * Each SHA-256 Digest is 32 Bytes ling.                     |
 * 32 Bytes = (32 * 8 bits) = 256 bits = {0,255}             |
 * 32 Bytes = 8 Bytes * 4 Segments                           |
 * ----------------------------------------------------------+
 *
 *
 * [ 8 Bytes ] 1 Segment
 * +---------------------------------------------------------+
 * | [  0]  [  1]  [  2]  [  3]  [  4]  [  5]  [  6]  [  7]  |
 * | [255]  [255]  [255]  [255]  [255]  [255]  [255]  [255]  |
 * | [16^2] [16^2] [16^2] [16^2] [16^2] [16^2] [16^2] [16^2] |
 * | [0F]   [0F]   [0F]   [0F]   [0F]   [0F]   [0F]   [0F]   |
 * +---------------------------------------------------------+
 *
 *
 * [8 Bytes] 4 Pairs (1 Pair = 0->130,560)
 * +---------------------------------------------------------+
 * | [  0]  [  1]  [  2]  [  3]  [  4]  [  5]  [  6]  [  7]  |
 * | [0,130560  ]  [0,130560  ]  [0,130560  ]  [0,130560  ]  |
 * +---------------------------------------------------------+
 *
 * dname_bijection is a deterministic bijective function for
 * a 32 byte sha256 digest.
 *
 * @param digest
 */
void dname_bijection(struct dname_digest *digest) {

    // [8 Byte] truncated bijective name
    // -------------------------------------------------------------------------------------
    // Allocate 32 bits * 4 words + 3 dashes + 1 bit
    char *name = malloc(DNAME_SHA256_DIGEST_32 * 4 + 4);
    char *s0 = getnamei(dname_pair(digest->sha256hash[0], digest->sha256hash[1]));
    char *s1 = getnamei(dname_pair(digest->sha256hash[2], digest->sha256hash[3]));
    char *s2 = getnamei(dname_pair(digest->sha256hash[4], digest->sha256hash[5]));
    char *s3 = getnamei(dname_pair(digest->sha256hash[6], digest->sha256hash[7]));
    sprintf(name,"%s-%s-%s-%s", s0, s1, s2, s3);
    digest->name = malloc(strlen( name));
    strcpy(digest->name, name);
    free(name);
    // -------------------------------------------------------------------------------------
}

// dname_pair implements the Cantor pairing function for
// deterministically pairing integers together.

/**
 * dname_pair()
 *
 * Use math (The Cantor Pairing Function) to deterministically pair integers together.
 *
 * We know we are paring bytes, so we know this value will never exceed 130,560.
 *
 * @param i1
 * @param i2
 * @return int
 */
int dname_pair(int i1, int i2) {
    return (i1+i2) * (i1+i2+1) / 2 + i2;
}

