#pragma once

#include "GLM.h"

#include "Renderer.h"

namespace Render::Text
{
	bool init();

	void renderText(SmartBuffer &smartBuffer);

	void addToBuffer(std::string text, float x, float y, float scale, glm::vec4 colour);
}   // namespace Render::Text