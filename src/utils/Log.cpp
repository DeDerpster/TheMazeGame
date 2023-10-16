#include "Log.h"


#ifdef IS_ON_WINDOWS
	#include <windows.h>
	#include <conio.h>
// COLORS LIST
// 1: Blue
// 2: Green
// 3: Cyan
// 4: Red
// 5: Purple
// 6: Yellow (Dark)
// 7: Default white
// 8: Gray/Grey
// 9: Bright blue
// 10: Bright green
// 11: Bright cyan
// 12: Bright red
// 13: Pink/Magenta
// 14: Yellow
// 15: Bright white
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

const std::string Log::criticalMessage = "CRITICAL";
const std::string Log::errorMessage    = "ERROR";
const std::string Log::warningMessage  = "WARNING";
const std::string Log::debugMessage    = "DEBUG";
const std::string Log::variableMessage = "VAR";
const std::string Log::defaultMessage  = "INFO";

Log::Log()
{
	// Initialises the log file and names it by the time of initialisation
	time_t     rawtime;
	struct tm *timeinfo;
	char       buffer[80];

	time(&rawtime);
	timeinfo = localtime(&rawtime);

	strftime(buffer, sizeof(buffer), "%d-%m-%Y %H-%M-%S", timeinfo);
	std::string currentTime(buffer);

	// Checks the file directory exits (if it doesn't it will create one)
	if(!std::filesystem::exists("logs"))
	{
		std::cout << "Logs directory doesn't exist... creating one\n";
		std::filesystem::create_directory("logs");
		if(std::filesystem::exists("logs"))
			std::cout << "Created directory\n";
	}
	// Updates the log file name
	logFile = "logs/" + currentTime + ".log";

	// Logs the first message with the log file name
	// It does not use variable function as it is still creating the class which would cause a loop as that calls get()
	std::stringstream ss;
	ss << "Logging file initialised: " << logFile;
	std::string title = "Hello World!";
	output(title, ss.str().c_str(), "", -1, LogColour::Debug);
}

// Two different functions for changing the colour of the text in the terminal
#ifdef IS_ON_WINDOWS
void Log::setConsoleColour(LogColour c)
{
	WORD colour = 7;
	switch(c) {
	case LogColour::Critical:
		colour = FOREGROUND_RED | FOREGROUND_INTENSITY;
		break;
	case LogColour::Error:
		colour = FOREGROUND_RED;
		break;
	case LogColour::Warning:
		colour = 14;
		break;
	case LogColour::Debug:
		colour = FOREGROUND_GREEN;
		break;
	case LogColour::Variable:
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
	case LogColour::Critical:
		std::cout << BOLDRED;
		break;
	case LogColour::Error:
		std::cout << RED;
		break;
	case LogColour::Warning:
		std::cout << YELLOW;
		break;
	case LogColour::Debug:
		std::cout << GREEN;
		break;
	case LogColour::Variable:
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

	// Tells the program that it is outputting
	outputting = true;

#ifdef DEBUG
	// If in debug mode it will print the message to the console
	setConsoleColour(colour);
	std::cout << "[" << type << "] " << message;
	if(line != -1)
		std::cout << " " << filepath << ":" << line;
	setConsoleColour(LogColour::Reset);
	std::cout << std::endl;
#endif

	// This saves the message to a new line of the log file
	std::ofstream file(logFile, std::ios_base::app);   // The std::ios_base::app allows it to write at end
	file << "[" << type << "] " << message;
	if(line != -1)
		file << " " << filepath << ":" << line;
	file << std::endl;
	file.close();

	// Finishes outputting
	outputting = false;
}