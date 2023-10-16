#pragma once

#include <GL/glew.h>

#include "Log.h"
#include <sstream>

#ifdef IS_ON_WINDOWS
	#define ASSERT(x) \
		if(!(x)) __debugbreak();
// if(!(x)) __debugbreak();
#else
	#define ASSERT(x) \
		if(!(x)) __builtin_trap();
#endif
#define GLCall(x)   \
	GLClearError(); \
	x;              \
	ASSERT(GLLogCall(#x, __FILE__, __LINE__))

static void GLClearError()
{
	while(glGetError() != GL_NO_ERROR)
		;
}

static bool GLLogCall(const char *function, const char *file, int line)
{
	while(GLenum error = glGetError())
	{
		std::stringstream ss;
		ss << "OpenGL error (" << error << "): " << function;
		Log::critical(ss.str().c_str(), file, line);
		return false;
	}
	return true;
}