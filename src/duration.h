#ifndef _DURATION_H_
#define _DURATION_H_

#include <time.h>

struct Duration {
	Duration();
	Duration(time_t millis);
	void add(time_t delta);
	bool is_finished();
	void reset();

private:
	time_t current_timer;
	time_t duration;
};

#endif
