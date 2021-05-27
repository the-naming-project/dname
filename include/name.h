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

#ifndef LIBNAME_H
#define LIBNAME_H

#define DNAME_SHA256_DIGEST      32
#define DNAME_SHA256_POINTER     65
#define DNAME_SHA256_ARRAY       64

typedef struct dname_digest {
    unsigned char sha256hash[DNAME_SHA256_DIGEST];              // 32 bit array
    unsigned char sha256hash_hexadecimal[DNAME_SHA256_ARRAY];   // 64 bit array
} dname_digest;

extern void about();
extern struct dname_digest getname(char *input);


#endif