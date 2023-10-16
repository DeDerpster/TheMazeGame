#pragma once

#include <GL/glew.h>

#include "Log.h"
#include <sstream>

// This creates an ASSERT definition so that when debugging I can set a breakpoint if a statement does not conform
#ifdef IS_ON_WINDOWS
	#define ASSERT(x) \
		if(!(x)) __debugbreak();
#else
	#define ASSERT(x) \
		if(!(x)) __builtin_trap();
#endif

// This uses an old method of debugging openGL by making checking for errors after every line
#define GLCall(x)   \
	GLClearError(); \
	x;              \
	ASSERT(GLLogCall(#x, __FILE__, __LINE__))

static void GLClearError()
{
	// This goes through every current error and removes it
	while(glGetError() != GL_NO_ERROR)
		;
}

static bool GLLogCall(const char *function, const char *file, int line)
{
	// This goes through all the errors Logs them
	while(GLenum error = glGetError())
	{
		std::stringstream ss;
		ss << "OpenGL error (" << error << "): " << function;
		Log::critical(ss.str().c_str(), file, line);
		return false;
	}
	return true;
}