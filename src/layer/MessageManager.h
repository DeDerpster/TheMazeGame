#pragma once

#include "gui/object/Text.h"

#include <chrono>

class MessageManager
{
  public:
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

	static void sendMessage(std::string message, Priority priority) { get().sendMessageImpl(message, priority); };
	static void update() { get().updateImpl(); };
	static void render() { get().renderImpl(); };

  private:
	struct Message
	{
		Text text;

		std::chrono::time_point<std::chrono::system_clock> timeCreated;
	};
	std::vector<Message> messages;

	MessageManager();

	void sendMessageImpl(std::string message, Priority priority);
	void updateImpl();
	void renderImpl();
};