#pragma once

#include <sstream>
#include <string>

#define LOGINFO __FILE__, __LINE__   // This is for just quickly getting information needed if there is an erro

class Log
{
  public:
	// Defines the different colours used (only used within the log class)
	enum class LogColour
	{
		Reset = 0,
		Critical,
		Error,
		Warning,
		Info,
		Debug,
		Variable
	};

  private:
	Log();

	static bool outputting;   // This is to make sure that it is only outputting one message at a time - when multithreading
	std::string logFile = "Logs/Default.log";
	// These are the different messages that define the type of log in the log file
	static const std::string criticalMessage;
	static const std::string errorMessage;
	static const std::string warningMessage;
	static const std::string debugMessage;
	static const std::string variableMessage;
	static const std::string defaultMessage;

	// This is the output function that does all the outputing to log files and terminal (if in debug mode)
	void output(const std::string &type, const char *message, const char *filepath, int line, LogColour colour);
	// This is used as a way to set the console colour to a colour predefined on windows and linux (as the two systems are different)
	void setConsoleColour(LogColour c);

  public:
	Log(const Log &) = delete;

	// This is for creating only one instance of the class
	static Log &get()
	{
		static Log instance;
		return instance;
	}

	static void critical(const char *message, const char *file, int line)
	{
		get().output(criticalMessage, message, file, line, LogColour::Critical);
		// This is to create a breakpoint in debug mode - for easier debugging
#ifdef DEBUG
	#ifdef IS_ON_WINDOWS
		__debugbreak();
	#else
		__builtin_trap();
	#endif
#endif
	}

	// Different error functions, uses the output to print the error
	static void error(const char *message, const char *file, int line)
	{
		get().output(errorMessage, message, file, line, LogColour::Error);
	}

	static void warning(const char *message)
	{
		get().output(warningMessage, message, "", -1, LogColour::Warning);
	}

	static void info(const char *message)
	{
		get().output(defaultMessage, message, "", -1, LogColour::Info);
	}

	static void debug(const char *message)
	{
		get().output(debugMessage, message, "", -1, LogColour::Debug);
	}

	// This is an ouput that allows any variable to be outputted (if they have overloaded the << operator)
	template <typename T>
	static void variable(const char *name, T &var)
	{
		std::stringstream ss;
		ss << name << ": " << var;
		get().output(variableMessage, ss.str().c_str(), "", -1, LogColour::Variable);
	}

	template <typename T>
	static void variable(const char *name, T *var)
	{
		std::stringstream ss;
		ss << name << ": " << var;
		get().output(variableMessage, ss.str().c_str(), "", -1, LogColour::Variable);
	}
};