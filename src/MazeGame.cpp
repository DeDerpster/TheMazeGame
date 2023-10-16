#include "Core.h"

#include "Application.h"
#include "Log.h"
#include "event/Event.h"

int main(void)
{
	Event::init();   // Initialises the events (in Event.h)

	Log::info("Initialised program");

	Application::setupLayers();

	if(Application::errorHasOccurred())
		Log::info("Exiting program due to error");
	else
		Application::gameLoop();   // Starts the game loop

	return 0;
}