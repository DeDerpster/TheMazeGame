#include "layer/MessageManager.h"

#include <ctime>

#include "rendering/Renderer.h"

#include "Application.h"

#define TEXT_SCALE 60.0f

MessageManager::MessageManager()
{
	// Reserves 10 slots for the messages (as this will be the max displayed)
	messages.reserve(10);
}

MessageManager::~MessageManager()
{
}

void MessageManager::sendMessageImpl(std::string message, Priority priority)
{
	// Adds a message to the list and chooses its colour based on the priority
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

	// Adds a tuple of a text and time of when it was set so that this can be used to see how old a message is and delete it if necessary
	messages.push_back({{message, 0.0f, 0.0f, TEXT_SCALE, colour, false, true}, std::chrono::system_clock::now()});
}

void MessageManager::updateImpl()
{
	// Checks the first message for when it was created (as this will be the oldest message) and deletes it if it is older than 5 seconds
	if(messages.size() != 0)
	{
		std::chrono::duration<double> elapsedSeconds = std::chrono::system_clock::now() - messages[0].timeCreated;
		if(elapsedSeconds.count() >= 5)
			messages.erase(messages.begin());
	}
}

void MessageManager::renderImpl()
{
	// Renders all the messages
	if(messages.size() != 0)
	{
		Vec2f pos = {200.0f, 200.0f};

		uint8_t layer       = 8;
		// Calculates the size of each layer (72 comes from experimentation of different sizes as well as getting the maximum size of text at scale 100)
		float sizeOfLayer = TEXT_SCALE * Application::getGUIScale() * (72.0f / 100.0f);

		float maxWidth = 40.0f * Application::getGUIScale();   // Sets the minimum width of the message board to be 50

		// Stores the text offset
		Vec2f textOffset = {25.0f * Application::getGUIScale(), 25.0f * Application::getGUIScale()};

		for(int i = messages.size() - 1; i > -1; i--)
		{
			// Renders the text at the correct spot
			messages[i].text.render(pos.x + textOffset.x, pos.y + textOffset.y + (messages.size() - 1 - i) * sizeOfLayer, layer);

			// Checks if the width is larger than the current max width
			float width = Render::getTextWidth(messages[i].text.m_Text, TEXT_SCALE * Application::getGUIScale());
			if(width > maxWidth)
				maxWidth = width;
		}

		// NOTE: This will be rendered under the text because simple render happens before text render
		Render::rectangle(pos.x, pos.y, 0.0f, maxWidth + 2 * textOffset.x, messages.size() * sizeOfLayer + textOffset.y, {0.1f, 0.1f, 0.1f, 0.5f}, layer, false, true);
	}
}
