#include "MessageManager.h"

#include <ctime>

#include "Renderer.h"

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

	messages.push_back({{message, 0.0f, 0.0f, 60.0f, colour, false, true}, std::chrono::system_clock::now()});
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
		float sizeOfLayer = 60.0f;   // TODO: Actally find one that works;
		Render::rectangle(200.0f, 200.0f, 0.0f, 500.0f, messages.size() * sizeOfLayer + 25.0f, {0.1f, 0.1f, 0.1f, 0.5f}, false, true);

		for(int i = messages.size() - 1; i > -1; i--)
		{
			messages[i].text.render(225.0f, 225.0f + i * sizeOfLayer);
		}
	}
}
