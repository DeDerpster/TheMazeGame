#include "Event.h"

#include "Application.h"

#include "Core.h"
#include "Log.h"
#include "ShaderEffect.h"

namespace Event
{
	static uint8_t mouseClickedDelay;

	static void key_callback(GLFWwindow *window, int key, int scancode, int action, int mods)
	{
		KeyboardEvent e(key, scancode, action, mods);
		Application::callEvent(e, true);
	}

	static void window_size_callback(GLFWwindow *window, int width, int height)
	{
		//windowSizeChange(windowWidth - width, windowHeight - height);
		Application::updateWindowSize(width, height);
		glViewport(0, 0, width, height);

		WindowResizeEvent e(Application::getWidth(), Application::getHeight(), width, height);
		Application::callEvent(e, true);
	}

	static void scroll_callback(GLFWwindow *window, double xoffset, double yoffset)
	{
		ScrollEvent e(xoffset, yoffset);
		Application::callEvent(e, true);
	}

	static void error_callback(int error, const char *description)
	{
		Log::error(description, LOGINFO);
	}

	static void mouse_button_callback(GLFWwindow *window, int button, int action, int mods)
	{
		if(mouseClickedDelay == 0)
		{   // TODO: Make this so it knows the action and mods and get rid of the mouse delay
			MouseButton       mButton = static_cast<MouseButton>(button);
			MouseClickedEvent e(mButton, getMousePos());
			Application::callEvent(e, true);
			mouseClickedDelay = 10;
		}
	}

	void update()
	{
		if(mouseClickedDelay > 0)
			mouseClickedDelay--;
	}

	void init()
	{
		GLFWwindow *window = static_cast<GLFWwindow *>(Application::getWindow());
		glfwSetKeyCallback(window, key_callback);   // TODO: Change this to one function
		glfwSetWindowSizeCallback(window, window_size_callback);
		glfwSetScrollCallback(window, scroll_callback);
		glfwSetErrorCallback(error_callback);
		glfwSetMouseButtonCallback(window, mouse_button_callback);

		mouseClickedDelay = 0;
	}

	bool isKeyPressed(int key)
	{
		int keystate = glfwGetKey(static_cast<GLFWwindow *>(Application::getWindow()), key);
		return keystate == GLFW_PRESS || keystate == GLFW_REPEAT;
	}

	Vec2f getMousePos()
	{
		double xPos, yPos;
		glfwGetCursorPos((GLFWwindow *) Application::getWindow(), &xPos, &yPos);

		return {(float) xPos, Application::getHeight() - (float) yPos};
	}

}   // namespace Application
