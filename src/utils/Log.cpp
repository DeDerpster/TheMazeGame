#include "Log.h"


#ifdef IS_ON_WINDOWS
	#include <windows.h>
	#include <conio.h>
//COLORS LIST
//1: Blue
//2: Green
//3: Cyan
//4: Red
//5: Purple
//6: Yellow (Dark)
//7: Default white
//8: Gray/Grey
//9: Bright blue
//10: Bright green
//11: Bright cyan
//12: Bright red
//13: Pink/Magenta
//14: Yellow
//15: Bright white
#else
	#define RESET       "\033[0m"
	#define BLACK       "\033[30m"        /* Black */
	#define RED         "\033[31m"        /* Red */
	#define GREEN       "\033[32m"        /* Green */
	#define YELLOW      "\033[33m"        /* Yellow */
	#define BLUE        "\033[34m"        /* Blue */
	#define MAGENTA     "\033[35m"        /* Magenta */
	#define CYAN        "\033[36m"        /* Cyan */
	#define WHITE       "\033[37m"        /* White */
	#define BOLDBLACK   "\033[1m\033[30m" /* Bold Black */
	#define BOLDRED     "\033[1m\033[31m" /* Bold Red */
	#define BOLDGREEN   "\033[1m\033[32m" /* Bold Green */
	#define BOLDYELLOW  "\033[1m\033[33m" /* Bold Yellow */
	#define BOLDBLUE    "\033[1m\033[34m" /* Bold Blue */
	#define BOLDMAGENTA "\033[1m\033[35m" /* Bold Magenta */
	#define BOLDCYAN    "\033[1m\033[36m" /* Bold Cyan */
	#define BOLDWHITE   "\033[1m\033[37m" /* Bold White */
#endif

#include "LogHeaders.h"

bool Log::outputting = false;

Log::Log()
{
	time_t     rawtime;
	struct tm *timeinfo;
	char       buffer[80];

	time(&rawtime);
	timeinfo = localtime(&rawtime);

	strftime(buffer, sizeof(buffer), "%d-%m-%Y %H-%M-%S", timeinfo);
	std::string currentTime(buffer);

	if(!std::filesystem::exists("logs"))
	{
		std::cout << "Logs directory doesn't exist... creating one\n";
		std::filesystem::create_directory("logs");
		if(std::filesystem::exists("logs"))
			std::cout << "Created directory\n";
	}
	logFile = "logs/" + currentTime + ".log";

	variableImpl("Initialised logging system", logFile);
}

#ifdef IS_ON_WINDOWS
void Log::setConsoleColour(LogColour c)
{
	WORD colour = 7;
	switch(c) {
		case LogColour::critical:
			colour = FOREGROUND_RED | FOREGROUND_INTENSITY;
			break;
		case LogColour::error:
			colour = FOREGROUND_RED;
			break;
		case LogColour::warning:
			colour = 14;
			break;
		case LogColour::debug:
			colour = FOREGROUND_GREEN;
			break;
		case LogColour::variable:
			colour = FOREGROUND_INTENSITY | 13;
			break;
		default:
			colour = 7;
			break;
	}
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), colour);
}
#else
void Log::setConsoleColour(LogColour c)
{
	switch(c) {
		case LogColour::critical:
			std::cout << BOLDRED;
			break;
		case LogColour::error:
			std::cout << RED;
			break;
		case LogColour::warning:
			std::cout << YELLOW;
			break;
		case LogColour::info:
			std::cout << BOLDRED;
			break;
		case LogColour::debug:
			std::cout << GREEN;
			break;
		case LogColour::variable:
			std::cout << BOLDMAGENTA;
			break;
		default:
			std::cout << RESET;
			break;
		}
}
#endif

void Log::output(const std::string &type, const char *message, const char *filepath, int line, LogColour colour)
{
	// This is to stop conflicts of two threads trying to access the logging system at the same time
	while(outputting) { std::this_thread::sleep_for(std::chrono::milliseconds(100)); };
	outputting = true;
#ifdef DEBUG
	setConsoleColour(colour);
	std::cout << "[" << type << "] " << message;
	if(line != -1)
		std::cout << " " << filepath << ":" << line;
	setConsoleColour(LogColour::reset);
	std::cout << std::endl;
#endif
	std::ofstream file(logFile, std::ios_base::app);   // The std::ios_base::app allows it to write at end
	file << "[" << type << "] " << message;
	if(line != -1)
		file << " " << filepath << ":" << line;
	file << std::endl;
	file.close();
	outputting = false;
}

void Log::criticalImpl(const char *message, const char *file, int line)
{
	output(criticalMessage, message, file, line, LogColour::critical);
#ifdef DEBUG
	#ifdef IS_ON_WINDOWS
	__debugbreak();
	#else
	__builtin_trap();
	#endif
#endif
}

void Log::errorImpl(const char *message, const char *file, int line)
{
	output(errorMessage, message, file, line, LogColour::error);
}

void Log::warningImpl(const char *message)
{
	output(warningMessage, message, "", -1, LogColour::warning);
}

void Log::infoImpl(const char *message)
{
	output(defaultMessage, message, "", -1, LogColour::info);
}

void Log::debugImpl(const char *message)
{
	output(debugMessage, message, "", -1, LogColour::debug);
}