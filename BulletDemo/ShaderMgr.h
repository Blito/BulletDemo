#pragma once

#include <GL\glew.h>
#include "Singleton.h"
#include <string>
#include <map>

class ShaderMgr : public Singleton<ShaderMgr>
{
public:
	static const std::string c_verticesAttr;
	static const std::string c_colorAttr;
	static const std::string c_texCoordAttr;
	static const std::string c_normalAttr;

	ShaderMgr(void);
	~ShaderMgr(void);

	GLuint createProgram(std::string vShaderPath, std::string fShaderPath);
	void useShader(GLuint shaderProgram);
	GLuint getAttribLocation(std::string attribute);
	void debugGL(const char * function);

private:
	const char * readFile(const char * path);

	std::map<std::string, GLuint> m_shaders;
	std::map<std::string, GLuint> m_attributesLocations;

	GLuint m_activeShader;
};

