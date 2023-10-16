#include "Renderer.h"

#include "Application.h"
#include "Sprite.h"
#include "ShaderEffect.h"
#include "VertexBufferLayout.h"

#include <ft2build.h>
#include FT_FREETYPE_H

Render::Render()
	: orderBuffersByYAxisSetting(false)
{
	// Text initialisation
	FT_Library ft;
	if(FT_Init_FreeType(&ft))
		Log::critical("FREETYPE: Could not init FreeType", LOGINFO);

	FT_Face face;
	if(FT_New_Face(ft, "res/fonts/FiraCode.ttf", 0, &face))
		Log::critical("FREETYPE: Failed to load font", LOGINFO);

	FT_Set_Pixel_Sizes(face, 0, 48);
	if(FT_Load_Char(face, 'X', FT_LOAD_RENDER))
		Log::critical("FREETYPE: Failed to load Glyph", LOGINFO);
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);   // disable byte-alignment restriction

	int maxHeight = 0;
	for(unsigned char c = 0; c < 128; c++)
	{
		// Loads each glyph
		if(FT_Load_Char(face, c, FT_LOAD_RENDER))
		{
			std::cout << "ERROR::FREETYPE: Failed to load Glyph" << std::endl;
			continue;
		}
		// Adds the character characters map, so it can be quickly retrieved later
		Character character = {
			nullptr,
			glm::ivec2(face->glyph->bitmap.width, face->glyph->bitmap.rows),
			glm::ivec2(face->glyph->bitmap_left, face->glyph->bitmap_top),
			(uint16_t) face->glyph->advance.x,
		};
		characters.insert(std::pair<char, Character>(c, character));
		characters[c].texture = new Texture(face->glyph->bitmap.width, face->glyph->bitmap.rows, face->glyph->bitmap.buffer);

		if(characters[c].size.y > maxHeight)
			maxHeight = characters[c].size.y;
	}

	Log::variable("Max Height", maxHeight);

	FT_Done_Face(face);
	FT_Done_FreeType(ft);

	Log::info("Text initialised");

	// Shaders setup
	int samplers[32];   // This is used for when rendering multiple textures with one draw function
	for(int i = 0; i < 32; i++)
		samplers[i] = i;

	m_SpriteShader = std::make_unique<Shader>("res/shaders/SpriteShader.glsl");
	m_SpriteShader->setUniform1iv("u_Textures", 32, samplers);

	m_TextShader = std::make_unique<Shader>("res/shaders/TextShader.glsl");
	m_TextShader->setUniform1iv("u_Textures", 32, samplers);

	m_SimpleShader = std::make_unique<Shader>("res/shaders/SimpleShader.glsl");

	// Buffers setup
	uint32_t maxVertices = 3528;

	m_VertexBuffer = std::make_unique<VertexBuffer>(nullptr, (uint16_t) sizeof(float) * 5 * maxVertices);
	m_IndexBuffer = std::make_unique<IndexBuffer>((maxVertices / 4) * 6);


	// Sprite VAO set up
	// Creates the VAO and links it to the vertex buffer
	m_SpriteVAO = std::make_unique<VertexArray>();
	m_SpriteVAO->bind();

	// Creates the layout of variables being pushed to the shader
	VertexBufferLayout spriteLayout = {
		{ShaderDataType::Float2, "position"},
		{ShaderDataType::Float2, "texCoord"},
		{ShaderDataType::Float, "texIndex"},
	};
	m_SpriteVAO->addBuffer(*m_VertexBuffer, spriteLayout);   // Adds it to the VAO
	m_IndexBuffer->bind();

	// Unbinds everything
	m_SpriteVAO->unbind();
	m_IndexBuffer->unbind();

	// Text VAO set up
	// Creates the VAO and links it to the vertex buffer
	m_TextVAO = std::make_unique<VertexArray>();
	m_TextVAO->bind();
	// Creates the layout of variables being pushed to the shader
	VertexBufferLayout textLayout = {
		{ShaderDataType::Float2, "position"},
		{ShaderDataType::Float2, "texCoord"},
		{ShaderDataType::Float, "texIndex"},
		{ShaderDataType::Float4, "texColour"},
	};
	m_TextVAO->addBuffer(*m_VertexBuffer, textLayout);   // Adds it to the VAO
	m_IndexBuffer->bind();
	// Unbinds everything
	m_TextVAO->unbind();
	m_IndexBuffer->unbind();

	// Simple VAO set up
	// Creates the VAO and links it to the vertex buffer
	m_SimpleVAO = std::make_unique<VertexArray>();
	m_SimpleVAO->bind();

	// Creates the layout of variables being pushed to the shader
	VertexBufferLayout simpleLayout = {
		{ShaderDataType::Float2, "position"},
		{ShaderDataType::Float4, "colour"},
	};
	m_SimpleVAO->addBuffer(*m_VertexBuffer, simpleLayout);   // Adds it to the VAO
	m_IndexBuffer->bind();
	// Unbinds everything
	m_SimpleVAO->unbind();
	m_IndexBuffer->unbind();

	Log::info("Buffer and VAOs set up");

	Log::info("Renderer initialised");
}

Render::~Render()
{
	// Deletes all the objects stored as pointers
	for(TextObject *obj : m_TextObjBuffer)
		delete obj;
	for(ColouredObject *obj : m_ObjectBuffer)
		delete obj;
	for(TexturedObject *obj : m_SpriteBuffer)
		delete obj;
	Log::info("Renderer destroyed");
}

void Render::renderImpl(std::vector<uint16_t> &shaderEffects)
{
	// resets the default effects
	m_SpriteShader->setUniform4f("u_Zoom", 1.0f, 1.0f, 1.0f, 1.0f);
	m_TextShader->setUniform4f("u_Zoom", 1.0f, 1.0f, 1.0f, 1.0f);
	m_SimpleShader->setUniform4f("u_Zoom", 1.0f, 1.0f, 1.0f, 1.0f);
	m_SpriteShader->setUniformMat4f("u_MVP", Application::getProj());
	m_TextShader->setUniformMat4f("u_MVP", Application::getProj());
	m_SimpleShader->setUniformMat4f("u_MVP", Application::getProj());

	// Sets effects given
	for(uint16_t id : shaderEffects)
	{
		if(id == 0)   // Checks if the effect exists
		{
			Log::warning("Trying to use effect that doesn't exist!");
			continue;
		}
		Effect::RenderShaderEffect *e = Effect::ShaderEffects::getShaderEffect(id);
		// TODO: Add effects that only effect certain shaders
		e->setEffect(*m_SpriteShader);
		e->setEffect(*m_TextShader);
		e->setEffect(*m_SimpleShader);
	}
	// Renders
	// simpleRender(m_ObjectBuffer);   // Renders bottom layers first
	// TODO: Put everything into one buffer?
	for(uint8_t i = 0; i < 10; i++)
	{
		spriteRender(i);
		simpleRender(i);
		textRender(i);
	}

	// Resets settings
	orderBuffersByYAxisSetting = false;

	m_ObjectBuffer.clear();
	m_SpriteBuffer.clear();
	m_TextObjBuffer.clear();
}

void Render::simpleRender(uint8_t layer)
{
	if(m_ObjectBuffer.getLayerPos(layer + 1) - m_ObjectBuffer.getLayerPos(layer) == 0)   // Checks the buffer is not empty
		return;

	if(!m_VertexBuffer->isEmpty())   // If the buffer is not empty, it empties it
	{
		m_VertexBuffer->clearBufferData();
		Log::warning("Vertex Buffer was not empty!");
	}

	m_SimpleShader->bind();
	// Goes through all the objects in the buffer and renders them
	for(uint32_t i = m_ObjectBuffer.getLayerPos(layer); i < m_ObjectBuffer.getLayerPos(layer + 1); i++)
	{
		ColouredObject *obj = m_ObjectBuffer[i];

		auto vertices = obj->convertToColouredVertices();
		// Checks if the buffer is full or the buffer is too big and draws what there is
		if(!m_VertexBuffer->canStore(obj->getSizeOfVertices()))
		{
			draw(*m_SimpleVAO);
			m_VertexBuffer->clearBufferData();   // Resets the buffer so it can draw again
		}
		// Adds the current object to the buffer by creating its quad (this is for memory efficiency)
		m_VertexBuffer->addToBuffer((void *) &vertices, obj->getSizeOfVertices());
		delete obj;
	}

	if(!m_VertexBuffer->isEmpty())   // If the buffer is not empty, it empties it
	{
		draw(*m_SimpleVAO);
		m_VertexBuffer->clearBufferData();
	}
}

void Render::spriteRender(uint8_t layer)
{
	if(m_SpriteBuffer.getLayerPos(layer + 1) - m_SpriteBuffer.getLayerPos(layer) == 0)
		return;
	if(!m_VertexBuffer->isEmpty())   // If the buffer is not empty, it empties it
	{
		m_VertexBuffer->clearBufferData();
		Log::warning("Vertex Buffer was not empty!");
	}

	m_SpriteShader->bind();
	uint8_t currentTexSlot = 0;   // This stores the slot the current texture is bound to, so it can set the texID part of the vertex
	Texture::clearBufferSlots();

	for(uint32_t i = m_SpriteBuffer.getLayerPos(layer); i < m_SpriteBuffer.getLayerPos(layer + 1); i++)
	{
		TexturedObject *obj = m_SpriteBuffer[i];
		// Checks if the buffer is full or the buffer is too big and draws what there is
		if(!m_VertexBuffer->canStore(obj->getSizeOfVertices()))
		{
			draw(*m_SpriteVAO);
			m_VertexBuffer->clearBufferData();   // Resets the buffer so it can draw again
			Texture::clearBufferSlots();
			currentTexSlot = 0;   // resets this as all the textures have been rendered
		}

		// Gets the texture slot
		uint8_t texSlot = Texture::getBoundSlot(Sprite::getSprite(obj->spriteID)->getTexture());
		if(texSlot == 32)   // This means the texture is not bound, so it gets bound
		{
			if(currentTexSlot == 32)
			{
				draw(*m_SpriteVAO);
				m_VertexBuffer->clearBufferData();   // Resets the buffer so it can draw again
				Texture::clearBufferSlots();
				currentTexSlot = 0;   // resets this as all the textures have been rendered
			}
			texSlot = currentTexSlot;
			currentTexSlot++;
			Sprite::getSprite(obj->spriteID)->bind(texSlot);
		}

		if(texSlot >= currentTexSlot)
		{
			Log::warning("Unbounded texSlot given!");
		}

		auto vertices = obj->convertToTexturedVertices(texSlot);   // Creates the vertices

		// Adds the current object to the buffer by creating its quad (this is for memory efficiency)
		m_VertexBuffer->addToBuffer((void *) &vertices, obj->getSizeOfVertices());

		delete obj;   // Deletes the object
	}
	if(!m_VertexBuffer->isEmpty())   // If the buffer is not empty, it empties it
	{
		draw(*m_SpriteVAO);
		m_VertexBuffer->clearBufferData();
	}
}

void Render::textRender(uint8_t layer)
{
	if(m_TextObjBuffer.getLayerPos(layer + 1) - m_TextObjBuffer.getLayerPos(layer) == 0)
		return;

	if(!m_VertexBuffer->isEmpty())   // If the buffer is not empty, it empties it
	{
		m_VertexBuffer->clearBufferData();
		Log::warning("Vertex Buffer was not empty!");
	}

	m_TextShader->bind();
	uint8_t currentTexSlot = 0;   // This stores the slot the current texture is bound to
	Texture::clearBufferSlots();
	for(uint32_t i = m_TextObjBuffer.getLayerPos(layer); i < m_TextObjBuffer.getLayerPos(layer + 1); i++)
	{
		TextObject *text = m_TextObjBuffer[i];

		float xOffset = 0.0f;
		if(text->text.empty())
			continue;

		// This goes through each character in the text and adds it to the buffer
		for(std::string::const_iterator c = text->text.begin(); c != text->text.end(); c++)
		{
			Character *ch = &characters[*c];

			// Checks if the buffer can store the new vertices
			if(!m_VertexBuffer->canStore(text->getSizeOfVertices()))
			{
				draw(*m_SpriteVAO);
				m_VertexBuffer->clearBufferData();   // Resets the buffer so it can draw again
				Texture::clearBufferSlots();
				currentTexSlot = 0;   // resets this as all the textures have been rendered
			}

			// Gets the textures slot
			uint8_t texSlot = Texture::getBoundSlot(ch->texture);
			if(texSlot == 32)   // This means it is not bound so it gets bound
			{
				if(currentTexSlot == 32)   // This means the it cannot render anymore textures
				{
					draw(*m_TextVAO);
					m_VertexBuffer->clearBufferData();   // Resets the buffer so it can draw again
					Texture::clearBufferSlots();
					currentTexSlot = 0;   // resets this as all the textures have been rendered
				}
				texSlot = currentTexSlot;
				currentTexSlot++;
				ch->texture->bind(texSlot);
			}

			// Gets the vertices
			auto vertices = text->convertCharacterToVertices(ch, xOffset, texSlot);

			m_VertexBuffer->addToBuffer((const void *) &vertices, text->getSizeOfVertices());

			// now advance cursors for next glyph (note that advance is number of 1/64 pixels)
			float newScale = text->scale / 100;
			xOffset += (ch->advance >> 6) * newScale;   // bitshift by 6 to get value in pixels (2^6 = 64)
		}

		// Deletes the text obj
		delete text;
	}
	if(!m_VertexBuffer->isEmpty())   // If the buffer is not empty, it empties it
	{
		draw(*m_TextVAO);
		m_VertexBuffer->clearBufferData();
	}
}

void Render::draw(VertexArray &vao) const   // Assumes VAO and shader have already been bound
{
	// Binds what this renderer is using for vertices
	vao.bind();
	m_VertexBuffer->bind();
	m_IndexBuffer->bind();
	GLCall(glDrawElements(GL_TRIANGLES, m_IndexBuffer->getCount(), GL_UNSIGNED_INT, nullptr));
	vao.unbind();
}

void Render::spriteImpl(float x, float y, double rotation, float width, float height, uint16_t spriteID, bool isOverlay)
{
	// TODO: Add a bottom layer sprite buffer
	// This creates a collision box for the sprite, so it can check if it is in frame
	CollisionBox box = {{-width / 2, -height / 2}, {width / 2, height / 2}};
	if(isOverlay || Application::isInFrame(x, y, box))
	{
		// Creates an object to store the information and adds it to the buffer through the function which takes settings into account
		TexturedObject *obj              = new TexturedObject({x, y}, width, height, rotation, true, spriteID);
		m_SpriteBuffer.addElement(obj, 1, orderBuffersByYAxisSetting);
	}
}

void Render::textImpl(std::string &text, float x, float y, float scale, glm::vec4 colour, bool isCentered, bool isOverlay)   // NOTE: Scale is a percentage
{
	// Gets the collision box for the text to check if it is frame
	CollisionBox box = getTextCollisionBox(text, scale);

	if(isOverlay || Application::isInFrame(x, y, box))
	{
		// Creates an object to store the information and adds it to the buffer through the function which takes settings into account
		TextObject *obj = new TextObject(text, scale, {x, y}, box.upperBound.x - box.lowerBound.x, box.upperBound.y - box.lowerBound.y, 0.0f, colour, isCentered);
		m_TextObjBuffer.addElement(obj, 3, orderBuffersByYAxisSetting);
	}
}

void Render::hoverTextImpl(std::string &inpText, float x, float y, float scale, glm::vec4 textColour, glm::vec4 backgroundColour, bool isOverlay)
{
	CollisionBox box = Render::getTextCollisionBox(inpText, scale);

	float borderWidth = 2.0f;

	float width  = box.upperBound.x - box.lowerBound.x;
	float height = box.upperBound.y - box.lowerBound.y;

	float yOffset = 4.0f;

	float textX = x + box.lowerBound.x;
	float textY = y + yOffset + borderWidth - box.lowerBound.y + height / 2;

	rectangle(x, y + yOffset + borderWidth + height / 2, 0.0f, width + 2 * borderWidth, height + 2 * borderWidth, backgroundColour, true, isOverlay);
	text(inpText, x, textY, scale, textColour, true, isOverlay);
}

void Render::rectangleImpl(float x, float y, double rotation, float width, float height, glm::vec4 colour, bool isCentered, bool isOverlay, bool bottomLayer)
{
	// Creates a collision box, taking into account if it wants to be centered or not
	CollisionBox box;
	if(isCentered)
		box = {{-width / 2, -height / 2}, {width / 2, height / 2}};
	else
		box = {{0, 0}, {width, height}};

	if(isOverlay || Application::isInFrame(x, y, box))
	{
		// Creates an object to store the information
		ColouredObject *obj = new ColouredObject({x, y}, width, height, rotation, isCentered, colour);
		if(bottomLayer)   // Checks what buffer it wants to be put on
			m_ObjectBuffer.addElement(obj, 0, orderBuffersByYAxisSetting);
		else
			m_ObjectBuffer.addElement(obj, 2, orderBuffersByYAxisSetting);
	}
}

// This is for rendering a box with a border
void Render::rectangleImpl(float x, float y, float width, float height, glm::vec4 colour, float borderWidth, glm::vec4 borderColour, bool isCentered, bool isOverlay, bool bottomLayer)
{
	float tempX, tempY;   // Stores the x and y position of the botton corner of the rectangle
	if(isCentered)
	{
		tempX = x - width / 2;
		tempY = y - height / 2;
	}
	else
	{
		tempX = x;
		tempY = y;
	}
	// Adds the normal rectangle to the buffer
	rectangle(x, y, 0.0f, width, height, colour, isCentered, isOverlay, bottomLayer);

	// For each border it renders more rectangles
	rectangle(tempX, tempY, 0.0f, width, borderWidth, borderColour, false, isOverlay, bottomLayer);
	rectangle(tempX, tempY, 0.0f, borderWidth, height, borderColour, false, isOverlay, bottomLayer);
	rectangle(tempX, tempY + height - borderWidth, 0.0f, width, borderWidth, borderColour, false, isOverlay, bottomLayer);
	rectangle(tempX + width - borderWidth, tempY, 0.0f, borderWidth, height, borderColour, false, isOverlay, bottomLayer);
}

float Render::getTextWidthImpl(std::string &text, float scale)
{
	// This goes through each letter of text and adds all the advance together to get the width
	float textWidth = 0;
	float newScale  = scale / 100;
	for(std::string::const_iterator c = text.begin(); c != text.end(); c++)
	{
		Character *ch = &characters[*c];
		textWidth += (ch->advance >> 6) * newScale;
	}
	return textWidth;
}

float Render::getTextHeightImpl(std::string &text, float scale)
{
	// This goes through each letter of the text and finds the character with the biggest height and returns that
	float textHeight = 0;
	float minYPos    = 0;
	float newScale   = scale / 100;

	for(std::string::const_iterator c = text.begin(); c != text.end(); c++)
	{
		Character *ch = &characters[*c];
		float      h  = ch->bearing.y * newScale;
		if(h > textHeight)
			textHeight = h;

		float y = (ch->bearing.y - ch->size.y) * newScale;
		if(y < minYPos)
			minYPos = y;
	}

	return textHeight - minYPos;
}

CollisionBox Render::getTextCollisionBoxImpl(std::string &text, float scale)
{
	// This does the same as the above two function but creates a collision box and does it with one for loop
	float textWidth  = 0;
	float textHeight = 0;
	float minYPos    = 0;
	float newScale   = scale / 100;

	for(std::string::const_iterator c = text.begin(); c != text.end(); c++)
	{
		Character *ch = &characters[*c];
		textWidth += (ch->advance >> 6) * newScale;
		float h = ch->bearing.y * newScale;
		if(h > textHeight)
			textHeight = h;
		float y = (ch->bearing.y - ch->size.y) * newScale;
		if(y < minYPos)
			minYPos = y;
	}

	return {{0.0f, minYPos}, {textWidth, textHeight}};
}

void Render::orderBuffersByYAxisImpl()
{
	// This sets the setting to true, and does a quick check to see if all the buffers are empty
	if(m_SpriteBuffer.size() != 0 || m_TextObjBuffer.size() != 0 || m_ObjectBuffer.size() != 0)
		Log::critical("Turning on setting 'orderBuffersByYAxis' after buffers have started to be filled!", LOGINFO);
	orderBuffersByYAxisSetting = true;
}