#pragma once

#include <sstream>
#include <string>

#define LOGINFO __FILE__, __LINE__   // This is for just quickly getting information needed if there is an erro

enum LogColour
{
	reset = 0,
	critical,
	error,
	warning,
	info,
	debug,
	variable
};

class Log
{
  private:
	static Log s_Instance;

	Log();

	std::string logFile         = "Logs/Default.log";
	std::string criticalMessage = "CRITICAL";
	std::string errorMessage    = "ERROR";
	std::string warningMessage  = "WARNING";
	std::string debugMessage    = "DEBUG";
	std::string variableMessage = "INFO";
	std::string defaultMessage  = "INFO";

	void output(std::string &type, const char *message, const char *filepath, int line);
	void setConsoleColour(LogColour c);

	void criticalImpl(const char *message, const char *file, int line);
	void errorImpl(const char *message, const char *file, int line);
	void warningImpl(const char *message);
	void infoImpl(const char *message);
	void debugImpl(const char *message);

	template <typename T>
	void variableImpl(const char *name, T var)
	{
		std::stringstream ss;
		ss << name << ": " << var;
		setConsoleColour(LogColour::variable);
		output(variableMessage, ss.str().c_str(), "", -1);
	}

  public:
	Log(const Log &) = delete;

	static void critical(const char *message, const char *file, int line) { get().criticalImpl(message, file, line); }
	static void error(const char *message, const char *file, int line) { get().errorImpl(message, file, line); }
	static void warning(const char *message) { get().warningImpl(message); }
	static void info(const char *message) { get().infoImpl(message); }
	static void debug(const char *message) { get().debugImpl(message); }

	template <typename T>
	static void variable(const char *name, T var)
	{
		get().variableImpl(name, var);
	}

	static Log &get()
	{
		static Log instance;
		return instance;
	}
};