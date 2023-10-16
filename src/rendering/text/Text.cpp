#include "Text.h"

#include <Shader.h>
#include <unordered_map>
#include <utility>
#include <vector>

#include <ft2build.h>
#include FT_FREETYPE_H

#include "Application.h"
#include "Log.h"

namespace Render::Text
{
	struct TextVertex
	{
		glm::vec2 position;
		Vec2f     texCoords;
		glm::vec4 colour;
		float     texID;

		TextVertex() {}
		TextVertex(glm::vec2 position, Vec2f texCoords, glm::vec4 colour, float texID)
			: position(position), texCoords(texCoords), colour(colour), texID(texID) {}
	};

	struct Character
	{
		Texture *texture;
		// unsigned int textureID;   // ID handle of the glyph texture
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
		float       x, y;
		float       scale;
		glm::vec4   colour;

		TextObj(std::string text, float x, float y, float scale, glm::vec4 colour)
			: text(text), x(x), y(y), scale(scale), colour(colour) {}
	};

	static std::vector<TextObj *> buffer;

	static std::unordered_map<char, Character> characters;

	bool init()
	{
		FT_Library ft;
		if(FT_Init_FreeType(&ft))
		{
			Log::error("FREETYPE: Could not init FreeType", LOGINFO);
			return false;
		}

		FT_Face face;
		if(FT_New_Face(ft, "res/fonts/FiraCode.ttf", 0, &face))
		{
			Log::error("FREETYPE: Failed to load font", LOGINFO);
			return false;
		}

		FT_Set_Pixel_Sizes(face, 0, 48);
		if(FT_Load_Char(face, 'X', FT_LOAD_RENDER))
		{
			Log::error("FREETYPE: Failed to load Glyph", LOGINFO);
			return false;
		}
		glPixelStorei(GL_UNPACK_ALIGNMENT, 1);   // disable byte-alignment restriction

		for(unsigned char c = 0; c < 128; c++)
		{
			// load character glyph
			if(FT_Load_Char(face, c, FT_LOAD_RENDER))
			{
				std::cout << "ERROR::FREETYPE: Failed to load Glyph" << std::endl;
				continue;
			}
			/*// generate texture
			unsigned int texture;
			glGenTextures(1, &texture);
			glBindTexture(GL_TEXTURE_2D, texture);
			glTexImage2D(
				GL_TEXTURE_2D,
				0,
				GL_RED,
				face->glyph->bitmap.width,
				face->glyph->bitmap.rows,
				0,
				GL_RED,
				GL_UNSIGNED_BYTE,
				face->glyph->bitmap.buffer);
			// set texture options
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);*/
			// now store character for later use
			Character character = {
				nullptr,
				glm::ivec2(face->glyph->bitmap.width, face->glyph->bitmap.rows),
				glm::ivec2(face->glyph->bitmap_left, face->glyph->bitmap_top),
				(uint16_t) face->glyph->advance.x   //
			};
			characters.insert(std::pair<char, Character>(c, character));
			characters[c].texture = new Texture(face->glyph->bitmap.width, face->glyph->bitmap.rows, face->glyph->bitmap.buffer);
		}

		FT_Done_Face(face);
		FT_Done_FreeType(ft);

		return true;
	}

	void addToBuffer(std::string text, float x, float y, float scale, glm::vec4 colour)
	{
		buffer.push_back(new TextObj(text, x, y, scale, colour));
	}

	void renderText(SmartBuffer &smartBuffer)
	{
		for(TextObj *text : buffer)
		{
			// glUniform3f(glGetUniformLocation(s.Program, "textColor"), color.x, color.y, color.z);
			// glActiveTexture(GL_TEXTURE0);
			// glBindVertexArray(VAO);

			// iterate through all characters
			for(std::string::const_iterator c = text->text.begin(); c != text->text.end(); c++)
			{
				Character *ch = &characters[*c];

				float xPos = text->x + ch->bearing.x * text->scale;
				float yPos = text->y - (ch->size.y - ch->bearing.y) * text->scale;

				float w = ch->size.x * text->scale;
				float h = ch->size.y * text->scale;
				// update VBO for each character
				TextVertex v0({xPos, yPos + h}, {0.0f, 0.0f}, text->colour, 0);
				TextVertex v1({xPos + w, yPos + h}, {1.0f, 0.0f}, text->colour, 0);
				TextVertex v2({xPos + w, yPos}, {1.0f, 1.0f}, text->colour, 0);
				TextVertex v3({xPos, yPos}, {0.0f, 1.0f}, text->colour, 0);

				std::array<TextVertex, 4> vertices = {v0, v1, v2, v3};
				/*float                     vertices[6][4] = {
                    {xPos, yPos + h, 0.0f, 0.0f},
                    {xPos, yPos, 0.0f, 1.0f},
                    {xPos + w, yPos, 1.0f, 1.0f},

                    {xPos, yPos + h, 0.0f, 0.0f},
                    {xPos + w, yPos, 1.0f, 1.0f},
                    {xPos + w, yPos + h, 1.0f, 0.0f}};*/
				ch->texture->bind(0);
				smartBuffer.addToBuffer((const void *) &vertices, sizeof(vertices));
				// render glyph texture over quad
				/*glBindTexture(GL_TEXTURE_2D, ch.textureID);
				// update content of VBO memory
				glBindBuffer(GL_ARRAY_BUFFER, VBO);
				glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);
				glBindBuffer(GL_ARRAY_BUFFER, 0);
				// render quad
				glDrawArrays(GL_TRIANGLES, 0, 6);*/
				smartBuffer.draw();
				smartBuffer.resetBuffer();
				// now advance cursors for next glyph (note that advance is number of 1/64 pixels)
				text->x += (ch->advance >> 6) * text->scale;   // bitshift by 6 to get value in pixels (2^6 = 64)
			}
			glBindVertexArray(0);
			glBindTexture(GL_TEXTURE_2D, 0);
		}
		buffer.clear();
	}
}   // namespace Render::Text