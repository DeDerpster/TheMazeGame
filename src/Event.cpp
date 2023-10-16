#include "Event.h"

#include "Application.h"

#include "Core.h"
#include "Log.h"

namespace Application
{
	static void key_callback(GLFWwindow *window, int key, int scancode, int action, int mods)
	{
		KeyboardEvent e(key, scancode, action, mods);
		callEvent(e);
	}

	static void window_size_callback(GLFWwindow *window, int width, int height)
	{
		//windowSizeChange(windowWidth - width, windowHeight - height);
		WindowResizeEvent e(getWidth(), getHeight(), width, height);
		callEvent(e);
		updateWindowSize(width, height);
		glViewport(0, 0, width, height);
	}

	static void scroll_callback(GLFWwindow *window, double xoffset, double yoffset)
	{
		ScrollEvent e(xoffset, yoffset);
		callEvent(e);
	}

	static void error_callback(int error, const char *description)
	{
		Log::error(description, LOGINFO);
	}

	void eventInit()
	{
		GLFWwindow *window = static_cast<GLFWwindow *>(getWindow());
		glfwSetKeyCallback(window, key_callback);   // TODO: Change this to one function
		glfwSetWindowSizeCallback(window, window_size_callback);
		glfwSetScrollCallback(window, scroll_callback);
		glfwSetErrorCallback(error_callback);
	}

	bool isKeyPressed(int key)
	{
		int keystate = glfwGetKey(static_cast<GLFWwindow *>(getWindow()), key);
		return keystate == GLFW_PRESS || keystate == GLFW_REPEAT;
	}
}   // namespace Application
