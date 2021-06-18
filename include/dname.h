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

#ifndef LIBDNAME_H
#define LIBDNAME_H

#define DNAME_SHA256_DIGEST_32      32
#define DNAME_SHA256_ARRAY_64       64
#define DNAME_SHA256_POINTER_65     65 // 64 + 1 bit for *pointer


typedef struct dname_digest {
    char *input;
    char *name;
    unsigned char sha256hash[DNAME_SHA256_DIGEST_32];
} dname_digest;

typedef struct dname_linux_lookup {
    int inContainer;
} dname_linux_lookup;

extern void about();
extern struct dname_digest dname(char *input);
extern struct dname_digest dname_lookup();
extern void dname_pretty_print(struct dname_digest *digest);
extern void dname_json_print(struct dname_digest *digest);

#endif