#pragma once

#include <GL\glew.h>
#include "Singleton.h"
#include <string>
#include <map>

class ShaderMgr : public Singleton<ShaderMgr>
{
public:
	ShaderMgr(void);
	~ShaderMgr(void);

	GLuint createProgram(std::string vShaderPath, std::string fShaderPath);
	void useShader(GLuint shaderProgram);
	void debugGL(const char * function);

private:
	const char * readFile(const char * path);

	std::map<std::string, GLuint> m_shaders;
	GLuint m_activeShader;
};

