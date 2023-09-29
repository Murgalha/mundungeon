#ifndef _RANDOM_H_
#define _RANDOM_H_

#include "types.h"

void random_init(unsigned int);
unsigned long random_long();
unsigned long random_rangei(unsigned long, unsigned long);

#endif
