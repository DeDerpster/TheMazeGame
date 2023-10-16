#pragma once

#define LOGINFO __FILE__, __LINE__   // This is for just quickly getting information needed if there is an erro

namespace Log
{   // This allows any part of the program to quickly output a message or error to a file or terminal - if in debug mode
	void init();

	void critical(const char *message, const char *file, int line);

	void error(const char *message, const char *file, int line);

	void warning(const char *message);

	void info(const char *message);

	void debug(const char *message);

	template <typename T>
	void variable(const char *name, T var);
}   // namespace Log
