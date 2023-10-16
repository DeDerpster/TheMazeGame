#include "Core.h"

#include "glDebug.h"

#include "Application.h"

#include "Renderer.h"

#include "Log.h"

#include "Event.h"

#include "Tile.h"

#include "Camera.h"

#include "Sprite.h"

namespace Application   // I've used a namespace here as I know there will only be one version of the application
{
	GLFWwindow *window;   // Stores the GLFW winodow

	glm::mat4 proj;   // Stores the projection mapping for the window

	int windowWidth, windowHeight;

	Layer *layers[2];   // This will store all the layers needed (I don't have to use a vector here as I know what is the maximum layers that will be used at one time)

	Render::Renderer *renderer;

	bool init()
	{   // This initialises everything
		windowWidth  = 940;
		windowHeight = 540;

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
		for(int i = 0; i < 2; i++)
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

	void update()   // Updates all the layers
	{
		for(int i = 0; i < 2; i++)
		{
			if(layers[i])
				layers[i]->update();
		}
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
	}
#endif

	// Get functions
	int       getWidth() { return windowWidth; }
	int       getHeight() { return windowHeight; }
	void *    getWindow() { return window; }
	glm::mat4 getProj() { return proj; }
	glm::mat4 getMVP() { return static_cast<Camera *>(layers[0])->getView(); }

	void addLayer(int index, Layer *layer)   // Adds layer to the stack
	{
		layers[index] = layer;
	}

	void removeLayer(int index)   // Removes layer by setting it to a nullptr
	{
		layers[index] = nullptr;
	}

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

	void callEvent(const Event &e)   // Sends event through the layers
	{
		for(int i = 0; i < 2; i++)
		{
			if(layers[i])
				layers[i]->eventCallback(e);
		}
	}

	void setEffect(const Effect::RenderEffect &e)   // Sends an effect through the layers
	{
		for(int i = 0; i < 2; i++)
		{
			if(layers[i])
				layers[i]->setEffect(e);
		}
	}

	void updateMVP(glm::mat4 &view, Layer *startlayer)   // updates the MVP of the layers after a point
	{
		bool found = false;
		for(int i = 0; i < 2; i++)
		{
			if(layers[i] && layers[i] == startlayer)
				found = true;
			else if(found)
				layers[i]->updateMVP(view);
		}
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

	bool isInFrame(float x, float y)
	{
		Camera *cam = static_cast<Camera *>(layers[0]);
		return cam->isInFrame(x, y);
	}

};   // namespace Application