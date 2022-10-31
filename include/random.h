#ifndef _RANDOM_H_
#define _RANDOM_H_

#include "types.h"

void random_init(u32);
u64 random_long();
u64 random_rangei(u64, u64);

#endif
