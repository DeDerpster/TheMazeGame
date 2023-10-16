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
		// load character glyph
		if(FT_Load_Char(face, c, FT_LOAD_RENDER))
		{
			std::cout << "ERROR::FREETYPE: Failed to load Glyph" << std::endl;
			continue;
		}
		// now store character for later use
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

	int samplers[32];
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
	for(TextObject *obj : m_TextObjBuffer)
		delete obj;
	for(RenderColouredObject *obj : m_ObjectBuffer)
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
		if(id == 0)
		{
			Log::warning("Trying to use effect that doesn't exist!");
			continue;
		}
		Effect::RenderShaderEffect *e = Effect::ShaderEffects::getShaderEffect(id);
		e->setEffect(*m_SpriteShader);
		e->setEffect(*m_TextShader);
		e->setEffect(*m_SimpleShader);
	}
	// Renders
	simpleRender(m_BottomLayerObjectBuffer);
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
		// draw();
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
	for(TexturedObject *obj : m_SpriteBuffer)
	{
		uint8_t texSlot = Texture::getBoundSlot(Sprite::getSprite(obj->spriteID)->getTexture());
		if(texSlot == 32)
		{
			texSlot = currentTexSlot;
			currentTexSlot++;
			if(currentTexSlot == 32)
			{
				draw(*m_SpriteVAO);
				m_VertexBuffer->clearBufferData();   // Resets the buffer so it can draw again
				currentTexSlot = 0;                  // resets this as all the textures have been rendered
				Texture::clearBufferSlots();
			}
			Sprite::getSprite(obj->spriteID)->bind(texSlot);
		}
		auto vertices = obj->convertToTexturedVertices(texSlot);
		// Checks if the buffer is full or the buffer is too big and draws what there is
		if(!m_VertexBuffer->canStore(obj->getSizeOfVertices()))
		{
			draw(*m_SpriteVAO);
			m_VertexBuffer->clearBufferData();   // Resets the buffer so it can draw again
			currentTexSlot = 0;                  // resets this as all the textures have been rendered
			Texture::clearBufferSlots();
		}
		// Adds the current object to the buffer by creating its quad (this is for memory efficiency)
		m_VertexBuffer->addToBuffer((void *) &vertices, obj->getSizeOfVertices());
		delete obj;
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
	for(TextObject *text : m_TextObjBuffer)
	{
		float xOffset = 0.0f;
		if(text->text.empty())
			continue;
		for(std::string::const_iterator c = text->text.begin(); c != text->text.end(); c++)
		{
			Character *ch = &characters[*c];

			uint8_t texSlot = Texture::getBoundSlot(ch->texture);
			if(texSlot == 32)
			{
				texSlot = currentTexSlot;
				currentTexSlot++;
				if(currentTexSlot == 32)
				{
					draw(*m_TextVAO);
					m_VertexBuffer->clearBufferData();   // Resets the buffer so it can draw again
					currentTexSlot = 0;                  // resets this as all the textures have been rendered
					Texture::clearBufferSlots();
				}
				ch->texture->bind(texSlot);
			}


			std::array<TextVertex, 4> vertices = text->convertCharacterToVertices(ch, xOffset, texSlot);
			if(!m_VertexBuffer->canStore(text->getSizeOfVertices()))
			{
				draw(*m_SpriteVAO);
				m_VertexBuffer->clearBufferData();   // Resets the buffer so it can draw again
				currentTexSlot = 0;                  // resets this as all the textures have been rendered
				Texture::clearBufferSlots();
			}
			m_VertexBuffer->addToBuffer((const void *) &vertices, text->getSizeOfVertices());

			// now advance cursors for next glyph (note that advance is number of 1/64 pixels)
			float newScale = text->scale / 100;
			xOffset += (ch->advance >> 6) * newScale;   // bitshift by 6 to get value in pixels (2^6 = 64)
		}
	}
	if(!m_VertexBuffer->isEmpty())   // If the buffer is not empty, it empties it
	{
		draw(*m_TextVAO);
		m_VertexBuffer->clearBufferData();
	}
	for(TextObject *obj : m_TextObjBuffer)
		delete obj;
	m_TextObjBuffer.clear();
}

void Render::spriteImpl(float x, float y, double rotation, float width, float height, uint16_t spriteID, bool isOverlay)
{
	CollisionBox box = {{-width / 2, -height / 2}, {width / 2, height / 2}};
	if(isOverlay || Application::isInFrame(x, y, box))
	{
		TexturedObject *obj              = new TexturedObject({x, y}, width, height, rotation, true, spriteID);

		if(orderBuffersByYAxisSetting && m_SpriteBuffer.size() > 0)
		{
			if(m_SpriteBuffer.size() == 1)
			{
				if(m_SpriteBuffer[0]->position.y > y)
					m_SpriteBuffer.push_back(obj);
				else
					m_SpriteBuffer.insert(m_SpriteBuffer.begin(), obj);
			}
			else
			{
				int startSub = 0;
				int endSub   = m_SpriteBuffer.size();
				int index    = m_SpriteBuffer.size() / 2;

				int indexOfInsertion = 0;

				while(startSub != endSub && startSub < endSub)
				{
					if(index + 1 == m_SpriteBuffer.size())
						index--;

					float thisY    = m_SpriteBuffer[index]->position.y;
					float nextY    = m_SpriteBuffer[index + 1]->position.y;

					if(thisY == y || (thisY > y && nextY < y))
					{
						index++;
						break;
					}
					else if(nextY == y)
					{
						index += 2;
						break;
					}
					else if(thisY > y && nextY > y)
						startSub = index + 2;
					else if(thisY < y && nextY < y)
						endSub = index;
					else
					{
						Log::critical("Vector is not sorted correctly!", LOGINFO);
						indexOfInsertion = m_SpriteBuffer.size();
						break;
					}

					index = (startSub + endSub) / 2;
				}

				m_SpriteBuffer.insert(m_SpriteBuffer.begin() + index, obj);
			}
		}
		else
			m_SpriteBuffer.push_back(obj);
	}
}

void Render::textImpl(std::string &text, float x, float y, float scale, glm::vec4 colour, bool isCentered, bool isOverlay)   // NOTE: Scale is a percentage
{
	CollisionBox box = getTextCollisionBox(text, scale);

	if(isOverlay || Application::isInFrame(x, y, box))
		m_TextObjBuffer.push_back(new TextObject(text, scale, {x, y}, box.upperBound.x, box.upperBound.y, 0.0f, colour, isCentered));
}

void Render::rectangleImpl(float x, float y, double rotation, float width, float height, glm::vec4 colour, bool isCentered, bool isOverlay, bool bottomLayer)
{
	CollisionBox box;
	if(isCentered)
		box = {{-width / 2, -height / 2}, {width / 2, height / 2}};
	else
		box = {{0, 0}, {width, height}};

	if(isOverlay || Application::isInFrame(x, y, box))
	{
		if(bottomLayer)
			m_BottomLayerObjectBuffer.push_back(new RenderColouredObject({x, y}, width, height, rotation, isCentered, colour));
		else
			m_ObjectBuffer.push_back(new RenderColouredObject({x, y}, width, height, rotation, isCentered, colour));
	}
}

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
	rectangle(x, y, 0.0f, width, height, colour, isCentered, isOverlay, bottomLayer);

	rectangle(tempX, tempY, 0.0f, width, borderWidth, borderColour, false, isOverlay, bottomLayer);
	rectangle(tempX, tempY, 0.0f, borderWidth, height, borderColour, false, isOverlay, bottomLayer);
	rectangle(tempX, tempY + height - borderWidth, 0.0f, width, borderWidth, borderColour, false, isOverlay, bottomLayer);
	rectangle(tempX + width - borderWidth, tempY, 0.0f, borderWidth, height, borderColour, false, isOverlay, bottomLayer);
}

float Render::getTextWidthImpl(std::string &text, float scale)
{
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

void Render::draw(VertexArray &vao) const   // Assumes VAO and shader have already been bound
{
	// Binds what this renderer is using for vertices
	vao.bind();
	m_VertexBuffer->bind();
	m_IndexBuffer->bind();
	GLCall(glDrawElements(GL_TRIANGLES, m_IndexBuffer->getCount(), GL_UNSIGNED_INT, nullptr));
	vao.unbind();
}


void Render::orderBuffersByYAxisImpl()
{
	if(m_SpriteBuffer.size() != 0 || m_TextObjBuffer.size() != 0 || m_ObjectBuffer.size() != 0)
		Log::warning("Turning on setting 'orderBuffersByYAxis' after buffers have started to be filled!");
	orderBuffersByYAxisSetting = true;
}