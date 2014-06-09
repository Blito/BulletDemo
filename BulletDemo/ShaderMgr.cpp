#include "ShaderMgr.h"

#include <fstream>


ShaderMgr::ShaderMgr(void) {
}


ShaderMgr::~ShaderMgr(void) {
}

GLuint ShaderMgr::createProgram(std::string vShaderPath, std::string fShaderPath) {

	std::string key = vShaderPath + ":" + fShaderPath; // in this case, legibility > efficiency

	if (m_shaders.find(key) != m_shaders.end()) { // shader combination already created
		return m_shaders[key];
	}

	//pre: new shader combination
	const char * vertexSource = readFile(vShaderPath.c_str());
	const char * fragmentSource = readFile(fShaderPath.c_str());

	if (vertexSource == nullptr || fragmentSource == nullptr)
		return 0;

	// Create and compile the vertex shader
    GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexSource, NULL);
    glCompileShader(vertexShader);

    // Create and compile the fragment shader
    GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentSource, NULL);
    glCompileShader(fragmentShader);

    // Link the vertex and fragment shader into a shader program
    GLuint shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glBindFragDataLocation(shaderProgram, 0, "outColor");
    glLinkProgram(shaderProgram);

	m_shaders[key] = shaderProgram;

	return shaderProgram;
}

const char * ShaderMgr::readFile(const char * path) {
	std::ifstream t;
	int length;

	t.open("file.txt");
	if (t.fail()) {
		return nullptr;
	}

	t.seekg(0, std::ios::end); 
	length = t.tellg();        
	t.seekg(0, std::ios::beg); 
	char * buffer = new char[length]; 
	t.read(buffer, length);    
	t.close();           

	return buffer;
}