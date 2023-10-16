#pragma once

#include <string>
#include <tuple>
#include <unordered_map>

#include "GLM.h"

class Shader
{
  private:
	std::string                          m_FilePath;
	unsigned int                         m_RendererID;
	std::unordered_map<std::string, int> m_UniformLocationCache;

	int                                  GetUniformLocation(const std::string &name);
	std::tuple<std::string, std::string> parseShader(const std::string &filepath);
	unsigned int                         compileShader(unsigned int type, const std::string &source);
	int                                  createShader(const std::string &vertexShader, const std::string &fragmentShader);

  public:
	Shader(const std::string &filepath);
	~Shader();

	void bind() const;
	void unbind() const;

	void setUniform1i(const std::string &name, int value);
	void setUniform1iv(const std::string &name, int count, const int *value);
	void setUniform1f(const std::string &name, float value);
	void setUniform4f(const std::string &name, float v0, float v1, float f2, float f3);
	void setUniformMat4f(const std::string &name, const glm::mat4 &matrix);
	void setUniformMat2f(const std::string &name, const glm::mat2 &matrix);
};