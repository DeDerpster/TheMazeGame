#include "MessageManager.h"

#include <ctime>

#include "Renderer.h"

#define TEXT_SCALE 60.0f

MessageManager::MessageManager()
{
	messages.reserve(10);
}

MessageManager::~MessageManager()
{
}

void MessageManager::sendMessageImpl(std::string message, Priority priority)
{
	glm::vec4 colour;
	switch(priority)
	{
	case Priority::High:
		colour = {1.0f, 0.0f, 0.0f, 1.0f};
		break;
	case Priority::Medium:
		colour = {0.992f, 0.737f, 0.18f, 1.0f};
		break;
	default:
		colour = {1.0f, 1.0f, 1.0f, 1.0f};
		break;
	}

	messages.push_back({{message, 0.0f, 0.0f, TEXT_SCALE, colour, false, true}, std::chrono::system_clock::now()});
}

void MessageManager::updateImpl()
{
	if(messages.size() != 0)
	{
		std::chrono::duration<double> elapsedSeconds = std::chrono::system_clock::now() - messages[0].timeCreated;
		if(elapsedSeconds.count() >= 5)
			messages.erase(messages.begin());
	}
}

void MessageManager::renderImpl()
{
	if(messages.size() != 0)
	{
		Vec2f pos = {200.0f, 200.0f};

		uint8_t layer       = 8;
		float   sizeOfLayer = TEXT_SCALE * (72.0f / 100.0f);

		float maxWidth = 40.0f;

		for(int i = messages.size() - 1; i > -1; i--)
		{
			messages[i].text.render(pos.x + 25.0f, pos.y + 25.0f + (messages.size() - 1 - i) * sizeOfLayer, layer);
			float width = Render::getTextWidth(messages[i].text.m_Text, TEXT_SCALE);
			if(width > maxWidth)
				maxWidth = width;
		}

		// NOTE: This will be rendered under the text because simple render happens before text render
		Render::rectangle(pos.x, pos.y, 0.0f, maxWidth + 50.0f, messages.size() * sizeOfLayer + 25.0f, {0.1f, 0.1f, 0.1f, 0.5f}, layer, false, true);
	}
}
