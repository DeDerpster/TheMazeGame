#pragma once

#include "GLM.h"

#include "Camera.h"
#include "Layer.h"
#include "RenderEffect.h"

#include "Event.h"

namespace Application
{
	bool init();
	void terminate();
#ifdef DEBUG
	ImGuiIO *getImGuiContext();
	bool     setupImGui();
#endif

	void update();
	void render();
#ifdef DEBUG
	void imGuiRender();
#endif

	void addLayer(Layer *layer);
	void addLayer(Layer *layer, int index);
	void addOverlay(Layer *layer);
	void removeLayer(int index);
	void removeLayer(Layer *layer);

	void callEvent(const Event &e, bool includeOverlay = false);
	void setEffect(const Effect::RenderEffect &e, bool includeOverlay = false);

	void updateWindowSize(int width, int height);
	bool isWindowOpen();
	void swapBuffers();
	bool isInFrame(float x, float y);

	Camera *  getCamera();
	glm::mat4 getProj();
	int       getWidth();
	int       getHeight();
	void *    getWindow();
}   // namespace Application
