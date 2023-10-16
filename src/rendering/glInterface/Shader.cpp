#include "glDebug.h"

#include "Shader.h"

#include <fstream>
#include <iostream>
#include <sstream>
#include <string>

#include "Log.h"

Shader::Shader(const std::string &filepath)
	: m_FilePath(filepath), m_RendererID(0)
{
	Log::variable("Loading shader", filepath);
	// Reads the shader file and splits them into their two types
	auto [vertexShader, fragmentShader] = parseShader(filepath);
	// Compiles the shader files
	m_RendererID                        = createShader(vertexShader, fragmentShader);
}
Shader::~Shader()
{
	GLCall(glDeleteProgram(m_RendererID));
}

int Shader::createShader(const std::string &vertexShader, const std::string &fragmentShader)
{
	// Creates the program the shaders will link to
	GLCall(uint32_t program = glCreateProgram());
	// Compiles the each shader
	uint32_t vs = compileShader(GL_VERTEX_SHADER, vertexShader);
	uint32_t fs = compileShader(GL_FRAGMENT_SHADER, fragmentShader);

	GLCall(glAttachShader(program, vs));
	GLCall(glAttachShader(program, fs));
	GLCall(glLinkProgram(program));
	GLCall(glValidateProgram(program));

	// Checks if the program linked correctly
	GLint isLinked = 0;
	glGetProgramiv(program, GL_LINK_STATUS, (int *) &isLinked);
	if(isLinked == GL_FALSE)
	{
		// If there was an error, it gets the information and logs it
		GLint maxLength = 0;
		glGetProgramiv(program, GL_INFO_LOG_LENGTH, &maxLength);

		// The maxLength includes the NULL character
		std::vector<GLchar> infoLog(maxLength);
		glGetProgramInfoLog(program, maxLength, &maxLength, &infoLog[0]);

		// We don't need the program anymore.
		glDeleteProgram(program);
		// Don't leak shaders either.
		glDeleteShader(vs);
		glDeleteShader(fs);

		Log::error(infoLog.data(), LOGINFO);
		Log::critical("Shader link failure!", LOGINFO);
	}

	glDetachShader(program, vs);
	glDetachShader(program, fs);

	GLCall(glDeleteShader(vs));
	GLCall(glDeleteShader(fs));

	return program;
}

uint32_t Shader::compileShader(uint32_t type, const std::string &source)
{
	// Compiles and generates an id for the shader
	uint32_t     id  = glCreateShader(type);
	const char * src = source.c_str();
	glShaderSource(id, 1, &src, nullptr);
	glCompileShader(id);

	// Checks for any errors when compiling
	int result;
	glGetShaderiv(id, GL_COMPILE_STATUS, &result);
	if(result == GL_FALSE)
	{
		// Gets the information and logs it
		int length;
		glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length);
		char *message = (char *) alloca(length * sizeof(char));
		glGetShaderInfoLog(id, length, &length, message);
		std::stringstream ss;
		ss << "Failed to compile "
		   << (type == GL_VERTEX_SHADER ? "vertex" : "fragment")
		   << " shader! "
		   << message << std::endl;
		Log::critical(ss.str().c_str(), LOGINFO);
		glDeleteShader(id);
		return 0;
	}

	return id;
}

std::tuple<std::string, std::string> Shader::parseShader(const std::string &filepath)
{
	// Checks if the file exists
	std::ifstream stream(filepath);
	if(!stream.good())
	{
		Log::error("Shader file not found!", LOGINFO);
		Log::variable("Shader filename", filepath);
		return {"", ""};
	}

	enum class ShaderType
	{
		NONE     = -1,
		VERTEX   = 0,
		FRAGMENT = 1
	};

	// Reads each line from the file
	std::string       line;
	std::stringstream ss[2];
	ShaderType        type = ShaderType::NONE;
	while(getline(stream, line))
	{
		if(line.find("#shader") != std::string::npos)   // Checks to find the label for which shader it it
		{
			// Sets the type to the current shader labelled
			if(line.find("vertex") != std::string::npos)
				type = ShaderType::VERTEX;
			else if(line.find("fragment") != std::string::npos)
				type = ShaderType::FRAGMENT;
		}
		else
		{
			// Checks to see if the shader type is set up
			if(type == ShaderType::NONE)
			{
				// Logs a warning and ignores the line
				Log::warning("Shader file not set up correctly");
				continue;
			}
			// Adds the line to the stream
			ss[(int) type] << line << '\n';
		}
	}

	// Returns both the vertex and fragment shaders
	return {ss[(int) ShaderType::VERTEX].str(), ss[(int) ShaderType::FRAGMENT].str()};
}

void Shader::bind() const
{
	GLCall(glUseProgram(m_RendererID));
}

void Shader::unbind() const
{
	GLCall(glUseProgram(0));
}

// For each type of variable it will find the location of the uniform and call the relevent function
void Shader::setUniform1i(const std::string &name, int value)
{
	bind();
	GLCall(glUniform1i(getUniformLocation(name), value));
}

void Shader::setUniform1iv(const std::string &name, int count, const int *value)
{
	bind();
	GLCall(glUniform1iv(getUniformLocation(name), count, value));
}

void Shader::setUniform1f(const std::string &name, float value)
{
	bind();
	GLCall(glUniform1f(getUniformLocation(name), value));
}

void Shader::setUniform4f(const std::string &name, float v0, float v1, float v2, float v3)
{
	bind();
	GLCall(glUniform4f(getUniformLocation(name), v0, v1, v2, v3));
}
void Shader::setUniformMat2f(const std::string &name, const glm::mat2 &matrix)
{
	bind();
	GLCall(glUniformMatrix2fv(getUniformLocation(name), 1, GL_FALSE, &matrix[0][0]));
}

void Shader::setUniformMat4f(const std::string &name, const glm::mat4 &matrix)
{
	bind();
	GLCall(glUniformMatrix4fv(getUniformLocation(name), 1, GL_FALSE, &matrix[0][0]));
}

int Shader::getUniformLocation(const std::string &name)
{
	// If the uniform has already been found, it will return that
	if(m_UniformLocationCache.find(name) != m_UniformLocationCache.end())
		return m_UniformLocationCache[name];

	// Otherwise it will get the location of the uniform
	GLCall(int location = glGetUniformLocation(m_RendererID, name.c_str()));
	if(location == -1)   // -1 is returned when that uniform does not exist
	{
		std::stringstream ss;
		ss << "Uniform '"
		   << name << "' doesn't exist!";
		Log::warning(ss.str().c_str());
	}
	// Adds the name to the map so it can be easily gained next time
	m_UniformLocationCache[name] = location;
	return location;
}