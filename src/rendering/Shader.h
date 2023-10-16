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
	std::tuple<std::string, std::string> ParseShader(const std::string &filepath);
	unsigned int                         CompileShader(unsigned int type, const std::string &source);
	int                                  CreateShader(const std::string &vertexShader, const std::string &fragmentShader);

  public:
	Shader(const std::string &filepath);
	~Shader();

	void bind() const;
	void unbind() const;

	void SetUniform1i(const std::string &name, int value);
	void SetUniform1iv(const std::string &name, int count, const int *value);
	void SetUniform1f(const std::string &name, float value);
	void SetUniform4f(const std::string &name, float v0, float v1, float f2, float f3);
	void SetUniformMat4f(const std::string &name, const glm::mat4 &matrix);
	void SetUniformMat2f(const std::string &name, const glm::mat2 &matrix);
};