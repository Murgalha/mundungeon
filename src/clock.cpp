#include "clock.h"
#include <SDL2/SDL.h>

Clock::Clock() {
	last_frame = 0.0f;
}

double Clock::delta_time() {
	double current_frame = SDL_GetTicks64();
	double delta_time = current_frame - last_frame;
	last_frame = current_frame;

	return delta_time;
}
