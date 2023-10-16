#pragma once

#include "GLM.h"

#include "Layer.h"
#include "RenderEffect.h"

#include "Event.h"

namespace Application
{
	bool      init();
	void      terminate();
	int       getWidth();
	int       getHeight();
	void *    getWindow();
	void      updateWindowSize(int width, int height);
	void      updateMVP(glm::mat4 &view, Layer *startlayer);
	glm::mat4 getMVP();

	void addLayer(int index, Layer *layer);
	void removeLayer(int index);
	void callEvent(const Event &e);
	void setEffect(const Effect::RenderEffect &e);
	void update();
	void render();
#ifdef DEBUG
	void imGuiRender();
#endif

	glm::mat4 getProj();
	bool      isWindowOpen();
	void      swapBuffers();

	bool isInFrame(float x, float y);

#ifdef DEBUG
	ImGuiIO *getImGuiContext();
	bool     setupImGui();
#endif
}   // namespace Application
