#pragma once

#include "GLM.h"

#include "Camera.h"
#include "Layer.h"
#include "RenderEffect.h"
#include "Log.h"

#include "Event.h"

class Application
{
  public:
	Application(const Application &) = delete;

	~Application();

	static Application &get()
	{
		static Application instance;
		return instance;
	}

#ifdef DEBUG
	static ImGuiIO *getImGuiContext()
	{
		return get().getImGuiContextImpl();
	}
	static bool setupImGui() { return get().setupImGuiImpl(); }
	static void imGuiRender() { get().imGuiRenderImpl(); }
#endif

	static void update()
	{
		get().updateImpl();
	}
	static void render() { get().renderImpl(); }

	static void addLayer(Layer *layer) { get().addLayerImpl(layer); }
	static void addLayer(Layer *layer, int index) { get().addLayerImpl(layer, index); }
	static void addOverlay(Layer *layer) { get().addOverlayImpl(layer); }
	static void removeLayer(int index) { get().removeLayerImpl(index); }
	static void removeLayer(Layer *layer, bool deleteLayer = false) { get().removeLayerImpl(layer, deleteLayer); }

	static void callEvent(const Event::Event &e, bool includeOverlay = false) { get().callEventImpl(e, includeOverlay); }
	static void setEffect(Effect::RenderEffect *e, bool includeOverlay = false) { get().setEffectImpl(e, includeOverlay); }
	static void setOverlayEffect(Effect::RenderEffect *e) { get().setOverlayEffectImpl(e); }

	static void updateWindowSize(int width, int height) { get().updateWindowSizeImpl(width, height); }
	static bool isWindowOpen() { return get().isWindowOpenImpl(); }
	static void swapBuffers() { get().swapBuffersImpl(); }
	static bool isInFrame(float x, float y, CollisionBox box) { return get().isInFrameImpl(x, y, box); }

	static Camera *  getCamera() { return get().getCameraImpl(); }
	static glm::mat4 getProj() { return get().getProjImpl(); }
	static int       getWidth() { return get().getWidthImpl(); }
	static int       getHeight() { return get().getHeightImpl(); }
	static void *    getWindow() { return get().getWindowImpl(); }

	static bool getIsPaused() { return get().gameIsPaused; }
	static void setIsPaused(bool paused) { get().gameIsPaused = paused; }

  private:
	static Application s_Instance;

	GLFWwindow *window;   // Stores the GLFW winodow
	Camera      camera;

	int       windowWidth, windowHeight;
	glm::mat4 proj;   // Stores the projection mapping for the window

	int                  overlayStart;
	std::vector<Layer *> layers;   // This will store all the layers needed (I don't have to use a vector here as I know what is the maximum layers that will be used at one time

	uint16_t projEffectID;

	bool gameIsPaused;

	Application();

#ifdef DEBUG
	ImGuiIO *getImGuiContextImpl();
	bool     setupImGuiImpl();
	void     imGuiRenderImpl();
#endif

	void updateImpl();
	void renderImpl();

	void addLayerImpl(Layer *layer);
	void addLayerImpl(Layer *layer, int index);
	void addOverlayImpl(Layer *layer);
	void removeLayerImpl(int index);
	void removeLayerImpl(Layer *layer, bool deleteLayer);

	void callEventImpl(const Event::Event &e, bool includeOverlay = false);
	void setEffectImpl(Effect::RenderEffect *e, bool includeOverlay = false);
	void setOverlayEffectImpl(Effect::RenderEffect *e);

	void updateWindowSizeImpl(int width, int height);
	bool isWindowOpenImpl();
	void swapBuffersImpl();
	bool isInFrameImpl(float x, float y, CollisionBox box);

	Camera *  getCameraImpl();
	glm::mat4 getProjImpl();
	int       getWidthImpl();
	int       getHeightImpl();
	void *    getWindowImpl();
};