#include "Core.h"

#include <algorithm>
#include <functional>
#include <vector>

#include "Application.h"
#include "Renderer.h"
#include "Sprite.h"
#include "Tile.h"
#include "glDebug.h"

#include "Maze.h"

#include "VertexBufferLayout.h"

#include "GUILayer.h"

#include "Event.h"
#include "Log.h"
#include "MessageManager.h"
#include "ShaderEffectsManager.h"

// SECTION: Initialises
Application::Application()
	: camera(4500.0f, 4500.0f),
	  windowWidth(940),
	  windowHeight(540),
	  proj(glm::ortho(0.0f, (float) windowWidth, 0.0f, (float) windowHeight, -100.0f, 100.0f)),
	  overlayStart(0),
	  gameIsPaused(false)
{   // This initialises everything
	layers.reserve(2);

	if(!glfwInit())   // Initialises GLFW, and checks it was okay
	{
		Log::critical("GLFW failed to initialise", LOGINFO);   // Logs a critical error
	}

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);   // Sets the openGL version
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 4);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	window = glfwCreateWindow(windowWidth, windowHeight, "MazeGame", NULL, NULL);
	if(!window)   // Checks window is not a nullpointer
	{
		Log::critical("Window seems to be a nullptr, will now shutdown... I do not feel well", LOGINFO);
	}

	glfwMakeContextCurrent(window);   // Makes context and makes it so that the program can only run at 60fps or lower (for a more constant framerate)
	glfwSwapInterval(0);

	if(glewInit() != GLEW_OK)   // Initialises GLEW
	{
		Log::critical("GLEW is not OK please send help", LOGINFO);
	}

	// Logs the open GL version (from the graphics card)
	Log::variable<const GLubyte *>("GL version", glGetString(GL_VERSION));

	// Enables the default blending
	GLCall(glEnable(GL_BLEND));
	GLCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));

	Sprite::init();   // Initialises all the sprites
}

Application::~Application()   // Terminates everything
{
	Log::info("Shutting down");

	// Deletes all the layers (as they are allocated on the heap)
	for(int i = 0; i < layers.size(); i++)
	{
		if(layers[i])
			delete layers[i];
	}

#ifdef DEBUG
	ImGui_ImplOpenGL3_Shutdown();   // Shuts down ImGui
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();   // Destoys ImGui Context
#endif

	glfwTerminate();   // Terminates glfw
}

#ifdef DEBUG
bool Application::setupImGuiImpl()   // Sets up ImGui
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

ImGuiIO *Application::getImGuiContextImpl()
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

void Application::updateImpl()   // Updates all the layers
{
	Event::update();
	if(projEffectID == 0)
		updateWindowSizeImpl(windowWidth, windowHeight);
	for(int i = layers.size() - 1; i > -1; i--)
	{
		layers[i]->update();
		if(gameIsPaused && i == overlayStart)
			break;
	}
	camera.update();

	MessageManager::update();

	for(const Event::Event *e : eventBuffer)
	{
		callEvent(*e, true);
		delete e;
	}
	eventBuffer.clear();
}

void Application::renderImpl()   // Renders all the layers
{
	camera.render();
	for(int i = 0; i < layers.size(); i++)
	{
		layers[i]->render();
		Render::render(layers[i]->getShaderEffects());
	}

	// TODO:: Make a layer for this or do something clever
	MessageManager::render();
	std::vector<uint16_t> temp;
	Render::render(temp);
}

#ifdef DEBUG
void Application::imGuiRenderImpl()   // Renders ImGui in all the layers
{
	for(int i = 0; i < layers.size(); i++)
		layers[i]->imGuiRender();
	camera.imGuiRender();
}
#endif
// !SECTION

// SECTION: Layers

void Application::setupLayersImpl()
{
	gameIsPaused = true;
	// TODO: Put this in a separate function
	for(Layer *layer : layers)
		delete layer;
	layers.clear();
	overlayStart = 0;
	camera.clearAnchor();

	addOverlayImpl(new GUILayer(GUILayer::Type::MainMenu, nullptr));
	Effect::ShaderEffectsManager::updateShaderEffects();
}

void Application::startGameImpl()
{
	gameIsPaused = false;
	for(Layer *layer : layers)
		delete layer;
	layers.clear();
	overlayStart = 0;

	Maze *maze = new Maze();
	maze->generate();   // Generates the maze
	addLayer(maze);     // Adds it to the layers

	Effect::ShaderEffectsManager::updateShaderEffects();
}

void Application::addLayerImpl(Layer *layer)   // Inserts a layer before the background
{
	layers.insert(layers.begin() + overlayStart, layer);
	overlayStart++;
}

void Application::addLayerImpl(Layer *layer, int index)   // Adds layer at a given index
{
	layers.insert(layers.begin() + index, layer);
	overlayStart++;
}

void Application::addOverlayImpl(Layer *layer)   // Adds an overlay to the layer stack, meaning it is appended to the end of the vector
{
	layers.push_back(layer);
}

void Application::removeLayerImpl(int index)   // Removes layer
{
	layers.erase(layers.begin() + index);
}

void Application::removeLayerImpl(Layer *layer, bool deleteLayer)
{
	std::vector<Layer *>::iterator index = std::find(layers.begin(), layers.end(), layer);
	if(index != layers.end())
	{
		if(deleteLayer)
			delete layers[index - layers.begin()];
		layers.erase(index);
	}
	else
		Log::warning("Cannot find layer to remove!");
}
// !SECTION

// SECTION: Events & Effects
void Application::callEventImpl(const Event::Event &e, bool includeOverlay)   // Sends event through the layers
{
	// TODO: Make this multithreading
	if(camera.eventCallback(e))
		return;

	int startVal;
	if(includeOverlay)
		startVal = layers.size();
	else
	{
		if(gameIsPaused && !e.ignoreIfPaused())
			return;
		startVal = overlayStart;
	}

	for(int i = startVal - 1; i > -1; i--)
	{
		if(layers[i])
		{
			if(layers[i]->eventCallback(e))
				break;
		}

		if(gameIsPaused && i == overlayStart && !e.ignoreIfPaused())
			break;
	}
}

void Application::setEffectImpl(Effect::Effect *e, bool includeOverlay)   // Sends an effect through the layers
{
	if(e->getType() == Effect::Effect::Type::removeShaderEffect)
	{
		Effect::RemoveShaderEffect *ne = static_cast<Effect::RemoveShaderEffect *>(e);
		if(ne->getID() == projEffectID)
		{
			Log::warning("Deleting projection effect!");
			projEffectID = 0;
		}
		else if(ne->getID() > projEffectID)
			projEffectID--;
	}
	int endVal;
	if(includeOverlay)
		endVal = layers.size();
	else
		endVal = overlayStart;

	for(int i = 0; i < endVal; i++)
		layers[i]->setEffect(e);
}

void Application::setOverlayEffectImpl(Effect::Effect *e)
{
	for(int i = overlayStart; i < layers.size(); i++)
		layers[i]->setEffect(e);
}

// !SECTION

// SECTION: Window stuff
void Application::updateWindowSizeImpl(int width, int height)   // updates the window size and projection matrix
{
	windowWidth  = width;
	windowHeight = height;
	proj         = glm::ortho(0.0f, (float) width, 0.0f, (float) height, -100.0f, 100.0f);
	if(projEffectID == 0)
	{
		std::string name = "u_MVP";
		projEffectID     = Effect::ShaderEffectsManager::sendOverlayEffect(name, proj);
	}
	else
	{
		Effect::UniformMat4 *e = static_cast<Effect::UniformMat4 *>(Effect::ShaderEffectsManager::getShaderEffect(projEffectID));   // TODO: Change this to a dynamic cast or make a function for it
		e->setMat(proj);
	}
}

bool Application::isWindowOpenImpl()   // Returns if the window is still open
{
	return !glfwWindowShouldClose(window);
}

void Application::swapBuffersImpl()   // Swaps the buffers
{
	glfwSwapBuffers(window);
}

bool Application::isInFrameImpl(float x, float y, CollisionBox box)
{
	return camera.isInFrame(x, y, box);
}

void Application::closeApplicationImpl()
{
	glfwDestroyWindow(window);
}

// !SECTION

// SECTION: Getters
int       Application::getWidthImpl() { return windowWidth; }
int       Application::getHeightImpl() { return windowHeight; }
void *    Application::getWindowImpl() { return window; }
Camera *  Application::getCameraImpl() { return &camera; }
glm::mat4 Application::getProjImpl() { return proj; }
// !SECTION