#ifndef _CLOCK_H_
#define _CLOCK_H_

struct Clock {
	float last_frame;

	Clock();
	double delta_time();
};

#endif
