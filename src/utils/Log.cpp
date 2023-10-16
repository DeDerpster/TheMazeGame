#include "Log.h"

// TODO: Make uniform system for this
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

namespace Log
{
	std::string logFile         = "Logs/Default.log";
	std::string criticalMessage = "CRITICAL";
	std::string errorMessage    = "ERROR";
	std::string warningMessage  = "WARNING";
	std::string debugMessage    = "DEBUG";
	std::string variableMessage = "INFO";
	std::string defaultMessage  = "INFO";

	bool hasInit = false;

#ifdef IS_ON_WINDOWS
	void setConsoleColor(WORD c)
	{
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), c);
	}
#endif

	void output(std::string &type, const char *message, const char *filepath, int line)
	{
		if(!hasInit)   // Checks to see if the Log has been initialised
			return;
#ifdef DEBUG
		std::cout << "[" << type << "] " << message;
		if(line != -1)
			std::cout << " " << filepath << ":" << line;
	#ifdef IS_ON_LINUX
		std::cout << RESET;
	#else
		setConsoleColor(7);
	#endif
		std::cout << std::endl;
#endif
		std::ofstream file(logFile, std::ios_base::app);   // The std::ios_base::app allows it to write at end
		file << "[" << type << "] " << message;
		if(line != -1)
			file << " " << filepath << ":" << line;
		file << std::endl;
		file.close();
	}

	void critical(const char *message, const char *file, int line)
	{
#ifdef IS_ON_LINUX
		std::cout << BOLDRED;
#else
		setConsoleColor(FOREGROUND_RED | FOREGROUND_INTENSITY);
#endif
		output(criticalMessage, message, file, line);
#ifdef DEBUG
	#ifdef IS_ON_WINDOWS
		__debugbreak();
	#else
		__builtin_trap();
	#endif
#endif
	}

	void error(const char *message, const char *file, int line)
	{
#ifdef IS_ON_LINUX
		std::cout << RED;
#else
		setConsoleColor(FOREGROUND_RED);
#endif
		output(errorMessage, message, file, line);
	}

	void warning(const char *message)
	{
#ifdef IS_ON_LINUX
		std::cout << YELLOW;
#else
		setConsoleColor(14);
#endif
		output(warningMessage, message, "", -1);
	}

	void info(const char *message)
	{
#ifdef IS_ON_LINUX
		std::cout << RESET;
#endif
		output(defaultMessage, message, "", -1);
	}

	void debug(const char *message)
	{
#ifdef IS_ON_LINUX
		std::cout << YELLOW;
#else
		setConsoleColor(FOREGROUND_GREEN);
#endif
		output(debugMessage, message, "", -1);
	}

	template <typename T>
	void variable(const char *name, T var)
	{
		std::stringstream ss;
		ss << name << ": " << var;
#ifdef IS_ON_LINUX
		std::cout << BOLDMAGENTA;
#else
		setConsoleColor(FOREGROUND_INTENSITY | 13);
#endif
		output(variableMessage, ss.str().c_str(), "", -1);
	}

	template void variable<int>(char const *, int);
	template void variable<unsigned int>(char const *, unsigned int);
	template void variable<double>(char const *, double);
	template void variable<const std::string &>(char const *, const std::string &);
	template void variable<unsigned char const *>(char const *, unsigned char const *);
	template void variable<float>(char const *, float);
	template void variable<bool>(char const *, bool);

	void init()
	{
		time_t     rawtime;
		struct tm *timeinfo;
		char       buffer[80];

		time(&rawtime);
		timeinfo = localtime(&rawtime);

		strftime(buffer, sizeof(buffer), "%d-%m-%Y %H-%M-%S", timeinfo);
		std::string currentTime(buffer);

		if(!std::filesystem::exists("Logs"))
		{
			std::cout << "Logs directory doesn't exist... creating one\n";
			std::filesystem::create_directory("Logs");
			if(std::filesystem::exists("Logs"))
				std::cout << "Created directory\n";
		}
		logFile = "Logs/" + currentTime + ".log";

		hasInit = true;

		variable("Initialised logging system", logFile);
	}
}   // namespace Log
