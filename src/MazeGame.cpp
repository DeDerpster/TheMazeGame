#include "Core.h"

#include "glDebug.h"

#include "Renderer.h"

#include "Application.h"
#include "Log.h"
#include "RandomGen.h"

#include "Layer.h"

#include "Maze.h"

#include "Tile.h"

#include "Camera.h"

#include <thread>

// This is the game loop that keeps the game running
void gameLoop()
{
#ifdef DEBUG
	ImGuiIO &io = *Application::getImGuiContext();   // Creates an ImGui Interface, if I am debugging
#endif

	{

		Maze *maze = new Maze();
		maze->generate();              // Generates the maze
		Application::addLayer(maze);   // Adds it to the layers
	}

	// Application::update();
	while(Application::isWindowOpen())
	{
#ifdef DEBUG
		GLCall(glClearColor(1.0f, 1.0f, 1.0f, 1.0f));   // Sets the background to white if I am Debugging as it is easier to see if textures are not rendering
#endif
		glClear(GL_COLOR_BUFFER_BIT);   // Resets the screen

		// Updates and renders the application
		// std::thread updateThread(&Application::update);
		Application::update();
		Application::render();
		// updateThread.join();

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
	// Initialises the logging system and the application
	Log::init();
	bool isOk = Application::init();
	if(!isOk)   // Checks if the Application initialised correctly
	{
		Log::critical("Failed to initialise! Will now shutdown!", LOGINFO);
		return -1;
	}
	Random::init();   // Initialises randomised system

	Log::info("Initialised program");

	gameLoop();   // Starts the game loop

	Application::terminate();   // Terminates the application

	return 0;
}