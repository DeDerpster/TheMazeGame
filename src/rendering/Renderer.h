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

// Render is a singleton and handles all the rendering
// all the static functions are there so you can call Render::rectangle(...) instead of Render::get().rectangle(...)

// TODO: Put this into a separate file?
template <typename T>
class Buffer : public std::vector<T *>
{
  private:
	std::array<uint32_t, 11> layersLoc;   // last layer is there for safety reasons and just stores the size of the vector

  public:
	Buffer()
	{
		memset(&layersLoc, 0, sizeof(layersLoc));
	}
	~Buffer()
	{
		for(T *obj : *this)
			delete obj;
	}

	void addElement(T *obj, uint8_t layer, bool orderByYAxis)
	{
		if(layer >= layersLoc.size() - 1)   // NOTE: this skips the last layer because it for safety
		{
			Log::warning("Buffer layer given is to high!");
			layer = layersLoc.size() - 2;
		}

		int layerSize = layersLoc[layer + 1] - layersLoc[layer];

		if(orderByYAxis && layerSize > 0)
		{
			// If the size is one it does a quick evaluation to see where to place it
			if(layerSize == 1)
			{
				if((*begin() + layersLoc[layer])->position.y > obj->position.y)
					push_back(obj);
				else
					insert(begin(), obj);
			}
			else
			{
				// This is a modified binomial search to find the range of where to place the new element
				// These variables keeps track of the range of elements of the set
				int startSub = layersLoc[layer];
				int endSub   = layersLoc[layer + 1];
				int index;

				while(startSub != endSub && startSub < endSub)   // This continues going until the range is 0
				{
					index = (startSub + endSub) / 2;   // Sets the index to be inbetween the new ranges
					if(index + 1 == layersLoc[layer + 1])
						index--;   // If it is looking at the end of the list, it minuses 1, so that the algorithm can work without any errors

					// Gets the y positions of the elements at the current index and above it
					float thisY = (*(begin() + index))->position.y;
					float nextY = (*(begin() + index + 1))->position.y;

					// Checks to see if it has found a location to place the new element
					if(thisY == obj->position.y || (thisY > obj->position.y && nextY < obj->position.y))
					{
						index++;
						break;
					}
					else if(nextY == obj->position.y)
					{
						index += 2;
						break;
					}
					else if(thisY > obj->position.y && nextY > obj->position.y)   // Otherwise it modifies the range for the binomial search to work
						startSub = index + 2;
					else if(thisY < obj->position.y && nextY < obj->position.y)
						endSub = index;
					else
					{   // If the vector is not sorted beforehand, it will raise an error here
						Log::critical("Buffer is not sorted correctly!", LOGINFO);
						index = size();
						break;
					}
				}

				if(index > layersLoc[layer + 1])
				{
					Log::warning("The index is over its range!");
					push_back(obj);
				}
				else if(index == size())
					push_back(obj);
				else
					insert(begin() + index, obj);   // Inserts the object at the correct position
			}
		}
		else
		{
			// Inserts it at the end of the layer wanted
			if(layersLoc[layer + 1] == size())
				push_back(obj);
			else
				insert(begin() + layersLoc[layer + 1], obj);
		}

		// updates locations of the layers
		for(int i = layer + 1; i < layersLoc.size(); i++)
			layersLoc[i]++;
	}

	uint32_t getLayerPos(uint8_t layer)
	{
		if(layer >= layersLoc.size())   // NOTE: this skips the last layer because it for safety
		{
			Log::warning("Buffer layer given is to high!");
			layer = layersLoc.size() - 1;
		}
		return layersLoc[layer];
	}

	void clear()
	{
		// Clears the layers information
		memset(&layersLoc, 0, sizeof(layersLoc));
		std::vector<T *>::clear();
	}
};

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
	static void hoverText(std::string &text, float x, float y, float scale, glm::vec4 textColour, glm::vec4 backgroundColour, bool isOverlay = false)
	{
		get().hoverTextImpl(text, x, y, scale, textColour, backgroundColour, isOverlay);
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
	// std::vector<TextObject *>           m_TextObjBuffer;   // This acts as a buffer for the text, so that it can render it in one go
	// std::array<uint32_t, 10>            m_TextObjLayers;
	Buffer<TextObject> m_TextObjBuffer;

	// These are the buffers that store the simple coloured rectanges information, so that they can be rendered properly later on
	Buffer<ColouredObject> m_ObjectBuffer;
	// std::vector<ColouredObject *> m_ObjectBuffer;
	// std::array<uint32_t, 10>            m_ObjectLayers;
	// std::vector<ColouredObject *> m_BottomLayerObjectBuffer;   // This is for the option to choose to render objects before anything else

	// This is the buffer for all the sprites
	Buffer<TexturedObject> m_SpriteBuffer;
	// std::vector<TexturedObject *> m_SpriteBuffer;
	// std::array<uint32_t, 10>      m_SpriteLayers;

	// Settings
	bool orderBuffersByYAxisSetting;   // This will order the sprite buffer so that they are rendered in the correct order

	Render();

	// Rendering functions
	void renderImpl(std::vector<uint16_t> &shaderEffects);
	void simpleRender(uint8_t layer);
	void spriteRender(uint8_t layer);
	void textRender(uint8_t layer);
	void draw(VertexArray &vao) const;

	// Functions for adding objects to buffers
	void spriteImpl(float x, float y, double rotation, float width, float height, uint16_t spriteID, bool isOverlay);
	void textImpl(std::string &text, float x, float y, float scale, glm::vec4 colour, bool isCentered, bool isOverlay);   // TODO: Make this order better
	void hoverTextImpl(std::string &inpText, float x, float y, float scale, glm::vec4 textColour, glm::vec4 backgroundColour, bool isOverlay);
	void rectangleImpl(float x, float y, double rotation, float width, float height, glm::vec4 colour, bool isCentered, bool isOverlay, bool bottomLayer);
	void rectangleImpl(float x, float y, float width, float height, glm::vec4 colour, float borderWidth, glm::vec4 borderColour, bool isCentered, bool isOverlay, bool bottomLayer);

	// Getters
	float        getTextWidthImpl(std::string &text, float scale);
	float        getTextHeightImpl(std::string &text, float scale);
	CollisionBox getTextCollisionBoxImpl(std::string &text, float scale);

	// Settings
	void orderBuffersByYAxisImpl();
};