#pragma once

#include <array>
#include <memory>
#include <unordered_map>
#include <vector>

#include "RenderObject.h"
#include "RenderVertex.h"

#include "IndexBuffer.h"
#include "Shader.h"
#include "Texture.h"
#include "Utils.h"
#include "VertexArray.h"
#include "ShaderEffect.h"

class Render
{
  public:
	Render(const Render &) = delete;
	~Render();

	static void render(std::vector<uint16_t> &shaderEffects) { get().renderImpl(shaderEffects); }
	static void sprite(float x, float y, double rotation, float size, uint16_t spriteID, bool isOverlay = false)
	{
		get().spriteImpl(x, y, rotation, size, size, spriteID, isOverlay);
	}
	static void sprite(float x, float y, double rotation, float width, float height, uint16_t spriteID, bool isOverlay = false)
	{
		get().spriteImpl(x, y, rotation, width, height, spriteID, isOverlay);
	}
	static void text(std::string &text, float x, float y, float scale, glm::vec4 colour, bool isCentered = false, bool isOverlay = false)
	{
		get().textImpl(text, x, y, scale, colour, isCentered, isOverlay);
	}
	static void rectangle(float x, float y, double rotation, float width, float height, glm::vec4 colour, bool isCentered = true, bool isOverlay = false, bool bottomLayer = false)
	{
		get().rectangleImpl(x, y, rotation, width, height, colour, isCentered, isOverlay, bottomLayer);
	}
	static void rectangle(float x, float y, float width, float height, glm::vec4 colour, float borderWidth, glm::vec4 borderColour, bool isCentered = true, bool isOverlay = false, bool bottomLayer = false)
	{
		get().rectangleImpl(x, y, width, height, colour, borderWidth, borderColour, isCentered, isOverlay, bottomLayer);
	}

	static float        getTextWidth(std::string &text, float scale) { return get().getTextWidthImpl(text, scale); }
	static float        getTextHeight(std::string &text, float scale) { return get().getTextHeightImpl(text, scale); }
	static CollisionBox getTextCollisionBox(std::string &text, float scale) { return get().getTextCollisionBoxImpl(text, scale); }

	static void orderBuffersByYAxis() { get().orderBuffersByYAxisImpl(); }

	static Render &get()
	{
		static Render instance;
		return instance;
	}

  private:
	static Render s_Instance;

	std::unique_ptr<IndexBuffer>  m_IndexBuffer;
	std::unique_ptr<VertexBuffer> m_VertexBuffer;

	std::unique_ptr<IndexBuffer>  m_TestIndexBuffer;
	std::unique_ptr<VertexBuffer> m_TestVertexBuffer;
	std::unique_ptr<VertexArray> m_TestVAO;
	std::unique_ptr<Shader> m_TestShader;

	std::unique_ptr<VertexArray> m_TextVAO;
	std::unique_ptr<VertexArray> m_SpriteVAO;
	std::unique_ptr<VertexArray> m_SimpleVAO;

	std::unique_ptr<Shader> m_TextShader;
	std::unique_ptr<Shader> m_SpriteShader;
	std::unique_ptr<Shader> m_SimpleShader;

	// Text Rendering

	std::unordered_map<char, Character> characters;
	std::vector<TextObject *>           m_TextObjBuffer;

	std::vector<RenderColouredObject *> m_ObjectBuffer;
	std::vector<RenderColouredObject *> m_BottomLayerObjectBuffer;

	std::vector<TexturedObject *> m_SpriteBuffer;

	bool orderBuffersByYAxisSetting;

	Render();

	void renderImpl(std::vector<uint16_t> &shaderEffects);
	void simpleRender(std::vector<RenderColouredObject *> &buffer);
	void spriteRender();
	void textRender();
	void draw(VertexArray &vao) const;

	void spriteImpl(float x, float y, double rotation, float width, float height, uint16_t spriteID, bool isOverlay);
	void textImpl(std::string &text, float x, float y, float scale, glm::vec4 colour, bool isCentered, bool isOverlay);
	void rectangleImpl(float x, float y, double rotation, float width, float height, glm::vec4 colour, bool isCentered, bool isOverlay, bool bottomLayer);
	void rectangleImpl(float x, float y, float width, float height, glm::vec4 colour, float borderWidth, glm::vec4 borderColour, bool isCentered, bool isOverlay, bool bottomLayer);

	float        getTextWidthImpl(std::string &text, float scale);
	float        getTextHeightImpl(std::string &text, float scale);
	CollisionBox getTextCollisionBoxImpl(std::string &text, float scale);

	void orderBuffersByYAxisImpl();
};