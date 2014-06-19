#pragma once

#include <GL\glew.h>
#include "Singleton.h"
#include <string>
#include <map>

class ShaderMgr : public Singleton<ShaderMgr>
{
public:
	// Shader variables
	static const std::string c_verticesAttr;
	static const std::string c_colorAttr;
	static const std::string c_texCoordAttr;
	static const std::string c_normalAttr;

	// Shader uniforms
	static const std::string c_uniMatrices;
	static const std::string c_uniPVM;
	static const std::string c_uniProj;
	static const std::string c_uniView;
	static const std::string c_uniModel;

	ShaderMgr(void);
	~ShaderMgr(void);

	GLuint createProgram(std::string vShaderPath, std::string fShaderPath);
	void useShader(GLuint shaderProgram);
	GLuint getAttribLocation(std::string attribute);
	void debugGL(const char * function);
	const GLuint getUBO() const;

private:
	const char * readFile(const char * path);

	std::map<std::string, GLuint> m_shaders;
	std::map<std::string, GLuint> m_attributesLocations;

	GLuint m_globalMatricesLoc;
	GLuint m_globalMatricesUBO;

	GLuint m_activeShader;
};

