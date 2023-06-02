#include "opengl_debug.h"
#include <stdio.h>

#define UNUSED(X) (void)(X)

void message_callback(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar* message, const void* userParam) {
	UNUSED(source);
	UNUSED(id);
	UNUSED(length);
	UNUSED(userParam);

	if (type == GL_DEBUG_TYPE_ERROR) {
		fprintf( stderr, "GL CALLBACK: %s type = 0x%x, severity = 0x%x, message = %s\n",
				 ( type == GL_DEBUG_TYPE_ERROR ? "** GL ERROR **" : "" ),
				 type, severity, message );
	}
}
