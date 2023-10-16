#pragma once

#include <array>
#include <memory>
#include <unordered_map>
#include <vector>

#include "IndexBuffer.h"
#include "Shader.h"
#include "Texture.h"
#include "Utils.h"
#include "VertexArray.h"
#include "ShaderEffect.h"

struct Vertex
{
	glm::vec2 position;
	Vec2f     texCoords;
	float     texID;

	Vertex() {}
	Vertex(glm::vec2 position, Vec2f texCoords, float texID)
		: position(position), texCoords(texCoords), texID(texID) {}
};

struct RenderObject
{
	glm::vec2 position;
	double    rotation;
	float     width, height;

	RenderObject() {}
	RenderObject(glm::vec2 position, double rotation, float width, float height)
		: position(position), rotation(rotation), width(width), height(height) {}
};

struct RenderColouredObject : RenderObject
{
	glm::vec4 colour;

	RenderColouredObject() {}
	RenderColouredObject(glm::vec2 position, double rotation, float width, float height, glm::vec4 colour)
		: RenderObject(position, rotation, width, height), colour(colour) {}
};

struct ColouredVertex
{
	glm::vec2 position;
	glm::vec4 colour;

	ColouredVertex() {}
	ColouredVertex(glm::vec2 position, glm::vec4 colour)
		: position(position), colour(colour) {}
};

struct TextVertex
{
	glm::vec2 position;
	Vec2f     texCoords;
	glm::vec4 colour;
	float     texID;

	TextVertex() {}
	TextVertex(glm::vec2 position, Vec2f texCoords, glm::vec4 colour, float texID)
		: position(position), texCoords(texCoords), texID(texID), colour(colour) {}
};

struct Character
{
	Texture *  texture;
	glm::ivec2 size;      // Size of glyph
	glm::ivec2 bearing;   // Offset from baseline to left/top of glyph
	uint16_t   advance;   // Offset to advance to next glyph

	~Character()
	{
		if(texture)
			delete texture;
	}
};

struct TextObj
{
	std::string text;
	glm::vec2   position;
	float       scale;
	glm::vec4   colour;

	TextObj(std::string text, glm::vec2 position, float scale, glm::vec4 colour)
		: text(text), position(position), scale(scale), colour(colour) {}
};

class Render
{
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
	std::vector<TextObj *>              m_TextObjBuffer;
	std::unordered_map<char, Character> characters;

	std::vector<RenderColouredObject *> m_ObjectBuffer;

	Render();

	void renderImpl(std::vector<uint16_t> &shaderEffects);
	void simpleRender();
	void spriteRender();
	void textRender();
	void draw(VertexArray &vao) const;

	void spriteImpl(float x, float y, double rotation, float width, float height, uint16_t spriteID);
	void textImpl(std::string &text, float x, float y, float scale, glm::vec4 colour, bool centerX);
	void rectangleImpl(float x, float y, double rotation, float width, float height, glm::vec4 colour);

	float        getTextWidthImpl(std::string &text, float scale);
	float        getTextHeightImpl(std::string &text, float scale);
	CollisionBox getTextCollisionBoxImpl(std::string &text, float scale);

	std::array<Vertex, 4>         createQuadImpl(float x, float y, double rotation, float width, float height, uint32_t texID, bool centering);
	std::array<glm::vec2, 4>      createVecQuadImpl(float x, float y, double rotation, float width, float height, bool centering);
	std::array<ColouredVertex, 4> createColouredQuadImpl(float x, float y, double rotation, float width, float height, glm::vec4 colour, bool centering);
	std::array<TextVertex, 4>     createTextQuadImpl(float x, float y, double rotation, float width, float height, glm::vec4 colour, uint32_t texID, bool centering);

  public:
	Render(const Render &) = delete;
	~Render();

	static void render(std::vector<uint16_t> &shaderEffects) { get().renderImpl(shaderEffects); }
	static void sprite(float x, float y, double rotation, float size, uint16_t spriteID) { get().spriteImpl(x, y, rotation, size, size, spriteID); }
	static void sprite(float x, float y, double rotation, float width, float height, uint16_t spriteID) { get().spriteImpl(x, y, rotation, width, height, spriteID); }
	static void text(std::string &text, float x, float y, float scale, glm::vec4 colour, bool centerX = false) { get().textImpl(text, x, y, scale, colour, centerX); }
	static void rectangle(float x, float y, double rotation, float width, float height, glm::vec4 colour) { get().rectangleImpl(x, y, rotation, width, height, colour); }

	static float        getTextWidth(std::string &text, float scale) { return get().getTextWidthImpl(text, scale); }
	static float        getTextHeight(std::string &text, float scale) { return get().getTextHeightImpl(text, scale); }
	static CollisionBox getTextCollisionBox(std::string &text, float scale) { return get().getTextCollisionBoxImpl(text, scale); }

	static std::array<Vertex, 4> createQuad(float x, float y, double rotation, float size, uint32_t texID, bool centering = true)
	{
		return get().createQuadImpl(x, y, rotation, size, size, texID, centering);
	}
	static std::array<Vertex, 4> createQuad(float x, float y, double rotation, float width, float height, uint32_t texID, bool centering = true) { return get().createQuadImpl(x, y, rotation, width, height, texID, centering); }

	static std::array<glm::vec2, 4> createVecQuad(float x, float y, double rotation, float size, bool centering = true) { return get().createVecQuadImpl(x, y, rotation, size, size, centering); }
	static std::array<glm::vec2, 4> createVecQuad(float x, float y, double rotation, float width, float height, bool centering = true) { return get().createVecQuadImpl(x, y, rotation, width, height, centering); }

	static std::array<ColouredVertex, 4> createColouredQuad(float x, float y, double rotation, float size, glm::vec4 colour, bool centering = true) { return get().createColouredQuadImpl(x, y, rotation, size, size, colour, centering); }
	static std::array<ColouredVertex, 4> createColouredQuad(float x, float y, double rotation, float width, float height, glm::vec4 colour, bool centering = true) { return get().createColouredQuadImpl(x, y, rotation, width, height, colour, centering); }

	static std::array<TextVertex, 4> createTextQuad(float x, float y, double rotation, float size, glm::vec4 colour, uint32_t texID, bool centering = true) { return get().createTextQuadImpl(x, y, rotation, size, size, colour, texID, centering); }
	static std::array<TextVertex, 4> createTextQuad(float x, float y, double rotation, float width, float height, glm::vec4 colour, uint32_t texID, bool centering = true) { return get().createTextQuadImpl(x, y, rotation, width, height, colour, texID, centering); }

	static Render &get()
	{
		static Render instance;
		return instance;
	}
};