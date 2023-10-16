#pragma once

#include "gui/object/Text.h"

#include <chrono>
#include <vector>

class MessageManager
{
  public:
	// Different priorities for the messages
	enum class Priority
	{
		High,
		Medium,
		Low
	};
	MessageManager(const MessageManager &) = delete;

	~MessageManager();

	static MessageManager &get()
	{
		static MessageManager instance;
		return instance;
	}

	// Adds a given message to the stack
	static void sendMessage(std::string message, Priority priority) { get().sendMessageImpl(message, priority); };

	static void update() { get().updateImpl(); };
	static void render() { get().renderImpl(); };

  private:
	// Message stores the text (which stores all the information for rendering it) as well as the time created as this allows to see how old an object is without having a timer that has to be updated for each one
	struct Message
	{
		Text text;

		std::chrono::time_point<std::chrono::system_clock> timeCreated;
	};
	std::vector<Message> messages;

	MessageManager();

	// Function implementations
	void sendMessageImpl(std::string message, Priority priority);
	void updateImpl();
	void renderImpl();
};