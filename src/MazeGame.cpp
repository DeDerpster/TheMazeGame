#include "Core.h"

#include "Application.h"
#include "Log.h"
#include "event/Event.h"

int main(void)
{
	// TODO: Add check to see if there was a problem booting up

	Event::init();   // Initialises the events (in Event.h)

	Log::info("Initialised program");

	Application::setupLayers();

	Application::gameLoop();   // Starts the game loop

	return 0;
}