#include "event/Event.h"

#include "Application.h"

#include "Core.h"
#include "ShaderEffectsManager.h"
#include "utils/Log.h"

#include "input/Keyboard.h"
#include "input/Mouse.h"
#include "menu/WindowResize.h"

namespace Event
{
	static void keyCallback(GLFWwindow *window, int key, int scancode, int action, int mods)
	{
		KeyboardEvent e(static_cast<KeyboardKey>(key), scancode, static_cast<Action>(action), static_cast<KeyboardMod>(mods));
		Application::callEvent(e, CallType::All);
	}

	static void windowSizeCallback(GLFWwindow *window, int width, int height)
	{
		Application::updateWindowSize(width, height);
		glViewport(0, 0, width, height);

		WindowResizeEvent e(Application::getWidth(), Application::getHeight(), width, height);
		Application::callEvent(e, CallType::Overlay);
	}

	static void scrollCallback(GLFWwindow *window, double xoffset, double yoffset)
	{
		ScrollEvent e(xoffset, yoffset);
		Application::callEvent(e, CallType::All);
	}

	static void errorCallback(int error, const char *description)
	{
		Log::error(description, LOGINFO);
	}

	static void mouseButtonCallback(GLFWwindow *window, int button, int action, int mods)
	{
		MouseClickedEvent e(static_cast<MouseButton>(button), getMousePos(), static_cast<Action>(action), static_cast<KeyboardMod>(mods));
		Application::callEvent(e, CallType::All);
	}

	void init()
	{
		GLFWwindow *window = static_cast<GLFWwindow *>(Application::getWindow());
		glfwSetKeyCallback(window, keyCallback);
		glfwSetWindowSizeCallback(window, windowSizeCallback);
		glfwSetScrollCallback(window, scrollCallback);
		glfwSetErrorCallback(errorCallback);
		glfwSetMouseButtonCallback(window, mouseButtonCallback);
	}

	bool isKeyPressed(KeyboardKey key)
	{
		Action keystate = static_cast<Action>(glfwGetKey(static_cast<GLFWwindow *>(Application::getWindow()), static_cast<int>(key)));

		return keystate == Action::Press || keystate == Action::Repeat;
	}

	Vec2f getMousePos()
	{
		double xPos, yPos;
		glfwGetCursorPos((GLFWwindow *) Application::getWindow(), &xPos, &yPos);

		return {(float) xPos, Application::getHeight() - (float) yPos};
	}

}   // namespace Event
