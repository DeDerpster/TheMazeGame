#include "Core.h"

#include <chrono>
#include <thread>

#include "glDebug.h"

#include "Camera.h"
#include "Renderer.h"

#include "Application.h"
#include "Log.h"
#include "RandomGen.h"

#include "Layer.h"
#include "Maze.h"

#include "Button.h"

#include "Tile.h"

// This is the game loop that keeps the game running
void gameLoop()
{
#ifdef DEBUG
	ImGuiIO &io = *Application::getImGuiContext();   // Creates an ImGui Interface, if I am debugging
#endif

	int          fps = 0;
	int          ups = 0;
	const double ns  = 1000000000.0f / 60.0f;

	auto   lastTime = std::chrono::high_resolution_clock::now();
	double delta    = 1.0f;

	while(Application::isWindowOpen())
	{
#ifdef DEBUG
		GLCall(glClearColor(1.0f, 1.0f, 1.0f, 1.0f));   // Sets the background to white if I am Debugging as it is easier to see if textures are not rendering
#endif
		glClear(GL_COLOR_BUFFER_BIT);   // Resets the screen

		auto now = std::chrono::high_resolution_clock::now();
		delta += (double) std::chrono::duration_cast<std::chrono::nanoseconds>(now - lastTime).count() / ns;
		lastTime = now;
		// Updates and renders the application
		while(delta >= 1)
		{
			Application::update();
			ups++;
			delta--;
		}
		fps++;
		Application::render();

#ifdef DEBUG   // Renders all the ImGui interface to make it easier while debugging
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();

		ImGui::Begin("MazeGame");
		Application::imGuiRender();
		// Shows the framerate of the program
		ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
		ImGui::End();

		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
#endif

		// Swaps the buffers of the application
		Application::swapBuffers();
		glfwPollEvents();   // Processes all pending events
#ifdef DEBUG
		if(io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
		{
			GLFWwindow *backup_current_context = glfwGetCurrentContext();
			ImGui::UpdatePlatformWindows();
			ImGui::RenderPlatformWindowsDefault();
			glfwMakeContextCurrent(backup_current_context);
		}
#endif
	}
}

int main(void)
{
	// TODO: Add check to see if there was a problem booting up

	Event::init();   // Initialises the events (in Event.h)

	Log::info("Initialised program");

	Application::setupLayers();

	gameLoop();   // Starts the game loop

	return 0;
}