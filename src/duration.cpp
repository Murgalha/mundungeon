#include "duration.h"

Duration::Duration() {
	duration = 0;
	current_timer = 0;
}

Duration::Duration(time_t millis) {
	duration = millis;
	current_timer = 0;
}

void Duration::add(time_t delta) {
	current_timer += delta;
}

void Duration::reset() {
	current_timer = 0;
}

bool Duration::is_finished() {
	return current_timer >= duration;
}
