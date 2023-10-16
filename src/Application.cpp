#include "Core.h"

#include <vector>

#include "Application.h"
#include "Renderer.h"
#include "Sprite.h"
#include "Tile.h"
#include "glDebug.h"

#include "Event.h"
#include "Log.h"

namespace Application   // I've used a namespace here as I know there will only be one version of the application
{
	// SECTION: Variables
	GLFWwindow *      window;   // Stores the GLFW winodow
	Camera            camera(4500.0f, 4500.0f);
	Render::Renderer *renderer;

	glm::mat4 proj;   // Stores the projection mapping for the window
	int       windowWidth, windowHeight;

	int                  overlayStart;
	std::vector<Layer *> layers;   // This will store all the layers needed (I don't have to use a vector here as I know what is the maximum layers that will be used at one time)

	// !SECTION
	// SECTION: Initialises
	bool init()
	{   // This initialises everything
		windowWidth  = 940;
		windowHeight = 540;

		overlayStart = 0;
		layers.reserve(2);

		if(!glfwInit())   // Initialises GLFW, and checks it was okay
		{
			Log::critical("GLFW failed to initialise", LOGINFO);   // Logs a critical error
			return false;
		}

		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);   // Sets the openGL version
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 4);
		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

		window = glfwCreateWindow(windowWidth, windowHeight, "MazeGame", NULL, NULL);
		if(!window)   // Checks window is not a nullpointer
		{
			Log::critical("Window seems to be a nullptr, will now shutdown... I do not feel well", LOGINFO);
			return false;
		}

		// Sets the projection
		proj = glm::ortho(0.0f, (float) windowWidth, 0.0f, (float) windowHeight, -100.0f, 100.0f);

		glfwMakeContextCurrent(window);   // Makes context and makes it so that the program can only run at 60fps or lower (for a more constant framerate)
		glfwSwapInterval(1);

		eventInit();   // Initialises the events (in Event.h)

		if(glewInit() != GLEW_OK)   // Initialises GLEW
		{
			Log::critical("GLEW is not OK please send help", LOGINFO);
			return false;
		}

		// Logs the open GL version (from the graphics card)
		Log::variable<const GLubyte *>("GL version", glGetString(GL_VERSION));

		// Enables the default blending
		GLCall(glEnable(GL_BLEND));
		GLCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));
		renderer = new Render::Renderer(3528);
		Sprite::Sprite::init();   // Initialises all the sprites
		return true;
	}

	void terminate()   // Terminates everything
	{
		Log::info("Shutting down");

		// Deletes all the layers (as they are allocated on the heap)
		for(int i = 0; i < layers.size(); i++)
		{
			if(layers[i])
				delete layers[i];
		}

		delete renderer;

#ifdef DEBUG
		ImGui_ImplOpenGL3_Shutdown();   // Shuts down ImGui
		ImGui_ImplGlfw_Shutdown();
		ImGui::DestroyContext();   // Destoys ImGui Context
#endif

		glfwTerminate();   // Terminates glfw
	}

#ifdef DEBUG
	// ImGuiIO &io;
	bool setupImGui()   // Sets up ImGui
	{
		ImGui::CreateContext();   // Creates ImGui context
		ImGuiIO &io = ImGui::GetIO();
		// io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
		io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
		io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;

		ImGui::StyleColorsDark();
		ImGuiStyle &style = ImGui::GetStyle();
		if(io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
		{
			style.WindowRounding              = 0.0f;
			style.Colors[ImGuiCol_WindowBg].w = 1.0f;
		}
		bool output1, output2;
		output1 = ImGui_ImplGlfw_InitForOpenGL(window, true);
		output2 = ImGui_ImplOpenGL3_Init("#version 150");
		if(output1 && output2)
			return true;
		else
		{
			Log::critical("ImGUI failed", LOGINFO);
			return false;
		}
	}
	ImGuiIO *getImGuiContext()
	{
		ImGui::CreateContext();   // Creates ImGui context
		ImGuiIO &io = ImGui::GetIO();
		// io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
		io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
		io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;

		ImGui::StyleColorsDark();
		ImGuiStyle &style = ImGui::GetStyle();
		if(io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
		{
			style.WindowRounding              = 0.0f;
			style.Colors[ImGuiCol_WindowBg].w = 1.0f;
		}
		bool output1, output2;
		output1 = ImGui_ImplGlfw_InitForOpenGL(window, true);
		output2 = ImGui_ImplOpenGL3_Init("#version 150");
		if(output1 && output2)
			return &io;
		else
		{
			Log::critical("ImGUI failed while creating the context", LOGINFO);
			return nullptr;
		}
	}
#endif

	void update()   // Updates all the layers
	{
		for(int i = 0; i < 2; i++)
		{
			if(layers[i])
				layers[i]->update();
		}
		camera.update();
	}

	void render()   // Renders all the layers
	{
		for(int i = 0; i < 2; i++)
		{
			if(layers[i])
			{
				layers[i]->render();
				renderer->render();
			}
		}
	}

#ifdef DEBUG
	void imGuiRender()   // Renders ImGui in all the layers
	{
		for(int i = 0; i < 2; i++)
		{
			if(layers[i])
				layers[i]->imGuiRender();
		}
		camera.imGuiRender();
	}
#endif

	// !SECTION
	// SECTION: Layers
	void addLayer(Layer *layer)   // Inserts a layer before the background
	{
		layers.insert(layers.begin() + overlayStart, layer);
		overlayStart++;
	}

	void addLayer(Layer *layer, int index)   // Adds layer at a given index
	{
		layers.insert(layers.begin() + index, layer);
		if(index < overlayStart)
			overlayStart++;
	}

	void addOverlay(Layer *layer)   // Adds an overlay to the layer stack, meaning it is appended to the end of the vector
	{
		layers.push_back(layer);
	}

	void removeLayer(int index)   // Removes layer
	{
		layers.erase(layers.begin() + index);
	}

	void removeLayer(Layer *layer)
	{
		for(int i = 0; i < layers.size(); i++)
		{
			if(layer == layers[i])
			{
				delete layers[i];
				layers.erase(layers.begin() + i);
				if(i < overlayStart)
					overlayStart--;
			}
		}
	}

	// !SECTION
	// SECTION: Events & Effects
	void callEvent(const Event &e, bool includeOverlay)   // Sends event through the layers
	{
		int endVal;
		if(includeOverlay)
			endVal = layers.size();
		else
			endVal = overlayStart;

		camera.eventCallback(e);

		for(int i = 0; i < endVal; i++)
		{
			if(layers[i])
				layers[i]->eventCallback(e);
		}
	}

	void setEffect(const Effect::RenderEffect &e, bool includeOverlay)   // Sends an effect through the layers
	{
		int endVal;
		if(includeOverlay)
			endVal = layers.size();
		else
			endVal = overlayStart;

		for(int i = 0; i < endVal; i++)
		{
			if(layers[i])
				layers[i]->setEffect(e);
		}
	}

	// !SECTION
	// SECTION: Window stuff
	void updateWindowSize(int width, int height)   // updates the window size and projection matrix
	{
		windowWidth  = width;
		windowHeight = height;
		proj         = glm::ortho(0.0f, (float) width, 0.0f, (float) height, -100.0f, 100.0f);
	}

	bool isWindowOpen()   // Returns if the window is still open
	{
		return !glfwWindowShouldClose(window);
	}

	void swapBuffers()   // Swaps the buffers
	{
		glfwSwapBuffers(window);
	}

	bool isInFrame(float x, float y)
	{
		return camera.isInFrame(x, y);
	}

	// !SECTION
	// SECTION: Getters
	int       getWidth() { return windowWidth; }
	int       getHeight() { return windowHeight; }
	void *    getWindow() { return window; }
	Camera *  getCamera() { return &camera; }
	glm::mat4 getProj() { return proj; }
};   // namespace Application