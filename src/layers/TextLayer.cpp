#include "TextLayer.h"

#include <functional>

#include "Application.h"
#include "Text.h"
#include "VertexBufferLayout.h"

TextLayer::TextLayer()
{
	auto bufferInit = [](VertexBufferLayout &layout) {
		layout.push<float>(2);
		layout.push<float>(2);
		layout.push<float>(4);
		layout.push<float>(1);
	};
	m_Buffer = std::make_unique<Render::SmartBuffer>(3528, bufferInit);

	m_Shader = std::make_unique<Shader>("res/shaders/TextShader.glsl");
	m_Shader->setUniformMat4f("u_MVP", Application::getProj());
}

void TextLayer::render()
{
	m_Shader->bind();
	Render::Text::renderText(*m_Buffer);
}
