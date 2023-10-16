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
	}

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
	for(RenderColouredObject *obj : m_ObjectBuffer)
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
	simpleRender(m_BottomLayerObjectBuffer);   // Renders bottom layers first
	spriteRender();
	simpleRender(m_ObjectBuffer);
	textRender();

	// Resets settings
	orderBuffersByYAxisSetting = false;
}

void Render::simpleRender(std::vector<RenderColouredObject *> &buffer)
{
	if(buffer.size() == 0)   // Checks the buffer is not empty
		return;

	if(!m_VertexBuffer->isEmpty())   // If the buffer is not empty, it empties it
	{
		m_VertexBuffer->clearBufferData();
		Log::warning("Vertex Buffer was not empty!");
	}

	m_SimpleShader->bind();
	// Goes through all the objects in the buffer and renders them
	for(RenderColouredObject *obj : buffer)
	{
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

	buffer.clear();

	if(!m_VertexBuffer->isEmpty())   // If the buffer is not empty, it empties it
	{
		draw(*m_SimpleVAO);
		m_VertexBuffer->clearBufferData();
	}
}

void Render::spriteRender()
{
	if(m_SpriteBuffer.size() == 0)
		return;
	if(!m_VertexBuffer->isEmpty())   // If the buffer is not empty, it empties it
	{
		m_VertexBuffer->clearBufferData();
		Log::warning("Vertex Buffer was not empty!");
	}

	m_SpriteShader->bind();
	uint8_t currentTexSlot = 0;   // This stores the slot the current texture is bound to, so it can set the texID part of the vertex
	Texture::clearBufferSlots();
	for(TexturedObject *obj : m_SpriteBuffer)
	{
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

	m_SpriteBuffer.clear();
}

void Render::textRender()
{
	if(m_TextObjBuffer.size() == 0)
		return;

	if(!m_VertexBuffer->isEmpty())   // If the buffer is not empty, it empties it
	{
		m_VertexBuffer->clearBufferData();
		Log::warning("Vertex Buffer was not empty!");
	}

	m_TextShader->bind();
	uint8_t currentTexSlot = 0;   // This stores the slot the current texture is bound to
	Texture::clearBufferSlots();
	for(TextObject *text : m_TextObjBuffer)
	{
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

	m_TextObjBuffer.clear();
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
		addElementToBuffer((std::vector<RenderObject *> *) &m_SpriteBuffer, obj);
	}
}

void Render::textImpl(std::string &text, float x, float y, float scale, glm::vec4 colour, bool isCentered, bool isOverlay)   // NOTE: Scale is a percentage
{
	// Gets the collision box for the text to check if it is frame
	CollisionBox box = getTextCollisionBox(text, scale);

	if(isOverlay || Application::isInFrame(x, y, box))
	{
		// Creates an object to store the information and adds it to the buffer through the function which takes settings into account
		TextObject *obj = new TextObject(text, scale, {x, y}, box.upperBound.x, box.upperBound.y, 0.0f, colour, isCentered);
		addElementToBuffer((std::vector<RenderObject *> *) &m_TextObjBuffer, obj);
	}
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
		RenderColouredObject *obj = new RenderColouredObject({x, y}, width, height, rotation, isCentered, colour);
		if(bottomLayer)   // Checks what buffer it wants to be put on
			addElementToBuffer((std::vector<RenderObject *> *) &m_BottomLayerObjectBuffer, obj);
		else
			addElementToBuffer((std::vector<RenderObject *> *) &m_ObjectBuffer, obj);
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
	float newScale   = scale / 100;
	for(std::string::const_iterator c = text.begin(); c != text.end(); c++)
	{
		Character *ch = &characters[*c];
		float      h  = ch->size.y * newScale;
		if(h > textHeight)
			textHeight = h;
	}
	return textHeight;
}

CollisionBox Render::getTextCollisionBoxImpl(std::string &text, float scale)
{
	// This does the same as the above two function but creates a collision box and does it with one for loop
	float textWidth  = 0;
	float textHeight = 0;
	float newScale   = scale / 100;

	for(std::string::const_iterator c = text.begin(); c != text.end(); c++)
	{
		Character *ch = &characters[*c];
		textWidth += (ch->advance >> 6) * newScale;
		float h = ch->size.y * newScale;
		if(h > textHeight)
			textHeight = h;
	}

	return {{0.0f, 0.0f}, {textWidth, textHeight}};
}

void Render::addElementToBuffer(std::vector<RenderObject *> *buffer, RenderObject *obj)
{
	// FIXME
	// Checks if it wants to be ordered and there are enough elements in the buffer to order them
	if(orderBuffersByYAxisSetting && buffer->size() > 0)
	{
		// If the size is one it does a quick evaluation to see where to place it
		if(buffer->size() == 1)
		{
			if((*buffer)[0]->position.y > obj->position.y)
				buffer->push_back(obj);
			else
				buffer->insert(buffer->begin(), obj);
		}
		else
		{
			// This is a modified binomial search to find the range of where to place the new element
			// These variables keeps track of the range of elements of the set
			int startSub = 0;
			int endSub   = buffer->size();
			int index;

			while(startSub != endSub && startSub < endSub)   // This continues going until the range is 0
			{
				index = (startSub + endSub) / 2;   // Sets the index to be inbetween the new ranges
				if(index + 1 == buffer->size())
					index--;   // If it is looking at the end of the list, it minuses 1, so that the algorithm can work without any errors

				// Gets the y positions of the elements at the current index and above it
				float thisY = (*buffer)[index]->position.y;
				float nextY = (*buffer)[index + 1]->position.y;

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
					Log::critical("Vector is not sorted correctly!", LOGINFO);
					index = buffer->size();
					break;
				}
			}

			buffer->insert(buffer->begin() + index, obj);   // Inserts the object at the correct position
		}
	}
	else
		buffer->push_back(obj);
}

void Render::orderBuffersByYAxisImpl()
{
	// This sets the setting to true, and does a quick check to see if all the buffers are empty
	if(m_SpriteBuffer.size() != 0 || m_TextObjBuffer.size() != 0 || m_ObjectBuffer.size() != 0)
		Log::warning("Turning on setting 'orderBuffersByYAxis' after buffers have started to be filled!");
	orderBuffersByYAxisSetting = true;
}