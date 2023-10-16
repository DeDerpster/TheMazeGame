#pragma once

#include <array>
#include <memory>
#include <unordered_map>
#include <vector>

#include "RenderObject.h"
#include "RenderVertex.h"

#include "Buffer.h"
#include "IndexBuffer.h"
#include "Shader.h"
#include "ShaderEffectsManager.h"
#include "Texture.h"
#include "Utils.h"
#include "VertexArray.h"

// Render is a singleton and handles all the rendering
// all the static functions are there so you can call Render::rectangle(...) instead of Render::get().rectangle(...)

class Render
{
  public:
	Render(const Render &) = delete;
	~Render();

	static void render(std::vector<uint16_t> &shaderEffects) { get().renderImpl(shaderEffects); }
	static void sprite(float x, float y, double rotation, float size, Sprite::ID spriteID, uint8_t layer, bool isOverlay = false)
	{
		get().spriteImpl(x, y, rotation, size, size, spriteID, layer, isOverlay);
	}
	static void sprite(float x, float y, double rotation, float width, float height, Sprite::ID spriteID, uint8_t layer, bool isOverlay = false)
	{
		get().spriteImpl(x, y, rotation, width, height, spriteID, layer, isOverlay);
	}
	static void text(std::string &text, float x, float y, float scale, glm::vec4 colour, uint8_t layer, bool isCentered = false, bool isOverlay = false)
	{
		get().textImpl(text, x, y, scale, colour, layer, isCentered, isOverlay);
	}
	static void hoverText(std::string &text, float x, float y, float scale, glm::vec4 textColour, glm::vec4 backgroundColour, uint8_t layer, bool isOverlay = false)
	{
		get().hoverTextImpl(text, x, y, scale, textColour, backgroundColour, layer, isOverlay);
	}
	static void rectangle(float x, float y, double rotation, float width, float height, glm::vec4 colour, uint8_t layer, bool isCentered = true, bool isOverlay = false)
	{
		get().rectangleImpl(x, y, rotation, width, height, colour, layer, isCentered, isOverlay);
	}
	static void rectangle(float x, float y, float width, float height, glm::vec4 colour, float borderWidth, glm::vec4 borderColour, uint8_t layer, bool isCentered = true, bool isOverlay = false)
	{
		get().rectangleImpl(x, y, width, height, colour, borderWidth, borderColour, layer, isCentered, isOverlay);
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

	// The index and vertex buffers are shared accross the vertex arrays, so there is only need for one of each
	std::unique_ptr<IndexBuffer>  m_IndexBuffer;
	std::unique_ptr<VertexBuffer> m_VertexBuffer;

	// Each vertex array are for each shader used, and are used so I don't have to keep reapplying the vertex buffer layout
	std::unique_ptr<VertexArray> m_TextVAO;
	std::unique_ptr<VertexArray> m_SpriteVAO;
	std::unique_ptr<VertexArray> m_SimpleVAO;

	// The shaders used when rendering
	std::unique_ptr<Shader> m_TextShader;
	std::unique_ptr<Shader> m_SpriteShader;
	std::unique_ptr<Shader> m_SimpleShader;

	std::unordered_map<char, Character> characters;        // This stores all the information for each character of text needed when rendering

	Buffer<TextObject> m_TextObjBuffer;   // This acts as a buffer for the text, so that it can render it in one go

	// These are the buffers that store the simple coloured rectanges information, so that they can be rendered properly later on
	Buffer<ColouredObject> m_ObjectBuffer;

	// This is the buffer for all the sprites
	Buffer<TexturedObject> m_SpriteBuffer;

	// Settings
	bool orderBuffersByYAxisSetting;   // This will order the sprite buffer so that they are rendered in the correct order

	Render();

	// Rendering functions
	void renderImpl(std::vector<uint16_t> &shaderEffects);
	void simpleRender(uint8_t startLayer, uint8_t endLayer);
	void spriteRender(uint8_t startLayer, uint8_t endLayer);
	void textRender(uint8_t startLayer, uint8_t endLayer);
	void draw(VertexArray &vao) const;

	// Functions for adding objects to buffers
	void spriteImpl(float x, float y, double rotation, float width, float height, Sprite::ID spriteID, uint8_t layer, bool isOverlay);
	void textImpl(std::string &text, float x, float y, float scale, glm::vec4 colour, uint8_t layer, bool isCentered, bool isOverlay);   // TODO: Make this order better
	void hoverTextImpl(std::string &inpText, float x, float y, float scale, glm::vec4 textColour, glm::vec4 backgroundColour, uint8_t layer, bool isOverlay);
	void rectangleImpl(float x, float y, double rotation, float width, float height, glm::vec4 colour, uint8_t layer, bool isCentered, bool isOverlay);
	void rectangleImpl(float x, float y, float width, float height, glm::vec4 colour, float borderWidth, glm::vec4 borderColour, uint8_t layer, bool isCentered, bool isOverlay);

	// Getters
	float        getTextWidthImpl(std::string &text, float scale);
	float        getTextHeightImpl(std::string &text, float scale);
	CollisionBox getTextCollisionBoxImpl(std::string &text, float scale);

	// Settings
	void orderBuffersByYAxisImpl();
};