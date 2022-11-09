#include <stdlib.h>
#include "random.h"

static unsigned long x, y, z;

void random_init(unsigned int seed) {
	srand(seed);
	x = rand();
	y = rand();
	z = rand();
}

unsigned long random_long() {
	unsigned long t;
	x ^= x << 16;
	x ^= x >> 5;
	x ^= x << 1;

	t = x;
	x = y;
	y = z;
	z = t ^ x ^ y;

	return z;
}

unsigned long random_rangei(unsigned long begin, unsigned long end) {
	return (random_long() % (end - begin)) + begin;
}
