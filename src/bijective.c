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
#include "bijective.h"
#include <stdio.h>

void dname_bijection(struct dname_digest *digest) {
    for (size_t i = 0; i < DNAME_SHA256_DIGEST_32; i++) {
       printf("%d\n", digest->sha256hash[i]);
    }


    // We have 32 integers ( 0 < N < 255 )
    // We have 8 sets of 256
    // We get 4 names


}

