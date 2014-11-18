#include "ShaderMgr.h"

#include <fstream>
#include <iostream>
#include <glm\glm.hpp>

namespace LittleLab {

	namespace Graphics {

		const std::string ShaderMgr::c_verticesAttr = "position";
		const std::string ShaderMgr::c_colorAttr = "color";
		const std::string ShaderMgr::c_texCoordAttr = "texCoord";
		const std::string ShaderMgr::c_normalAttr = "normal";

		const std::string ShaderMgr::c_uniPVM = "pvm";
		const std::string ShaderMgr::c_uniProj = "proj";
		const std::string ShaderMgr::c_uniView = "view";
		const std::string ShaderMgr::c_uniModel = "model";
		const std::string ShaderMgr::c_uniMatrices = "GlobalMatrices";


		ShaderMgr::ShaderMgr(void) {
			m_activeShader = 0;

			m_attributesLocations[c_verticesAttr] = 0;
			m_attributesLocations[c_colorAttr] = 1;
			m_attributesLocations[c_texCoordAttr] = 2;
			m_attributesLocations[c_normalAttr] = 3;

			m_globalMatricesLoc = 1;

			glGenBuffers(1, &m_globalMatricesUBO);
			glBindBuffer(GL_UNIFORM_BUFFER, m_globalMatricesUBO);
			glBufferData(GL_UNIFORM_BUFFER, sizeof(glm::mat4) * 2, NULL, GL_STREAM_DRAW);
			glBindBufferRange(GL_UNIFORM_BUFFER, m_globalMatricesLoc, m_globalMatricesUBO, 0, sizeof(glm::mat4) * 2);
			glBindBuffer(GL_UNIFORM_BUFFER, 0);
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
			debugGL("before createProgram");
			// Create and compile the vertex shader
			GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
			glShaderSource(vertexShader, 1, (const char **)&vertexSource, NULL);
			glCompileShader(vertexShader);
			debugGL("compile vertex shader");

			// Create and compile the fragment shader
			GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
			glShaderSource(fragmentShader, 1, (const char **)&fragmentSource, NULL);
			glCompileShader(fragmentShader);
			debugGL("compile fragment shader");

			// Link the vertex and fragment shader into a shader program
			GLuint shaderProgram = glCreateProgram();
			glAttachShader(shaderProgram, vertexShader);
			glAttachShader(shaderProgram, fragmentShader);

			// Bind the shader attributes
			for (auto it = m_attributesLocations.begin(); it != m_attributesLocations.end(); it++) {
				glBindAttribLocation(shaderProgram, it->second, it->first.c_str());
			}
			glBindFragDataLocation(shaderProgram, 0, "outColor");

			glLinkProgram(shaderProgram);

			delete vertexSource, fragmentSource;

			GLint status;
			glGetProgramiv(shaderProgram, GL_LINK_STATUS, &status);

			std::cout << "shaderProgram: " << shaderProgram << " " << status << "=?" << GL_TRUE << std::endl;
			GLchar errorLog[1024] = { 0 };
			glGetProgramInfoLog(shaderProgram, 1024, NULL, errorLog);
			std::cout << errorLog << std::endl;

			m_shaders[key] = shaderProgram;

			debugGL("link program");

			GLuint matricesIndex = glGetUniformBlockIndex(shaderProgram, c_uniMatrices.c_str());
			glUniformBlockBinding(shaderProgram, matricesIndex, m_globalMatricesLoc);

			debugGL("uni block binding");

			return shaderProgram;
		}

		void ShaderMgr::useShader(GLuint shaderProgram) {
			if (m_activeShader != shaderProgram) {
				std::cout << m_activeShader << " " << shaderProgram << std::endl;
				if (glIsProgram(shaderProgram) != GL_TRUE)
					std::cout << "shaderProgram not a program" << std::endl;
				glUseProgram(shaderProgram);
				m_activeShader = shaderProgram;

				debugGL("use shader");
			}
		}

		GLuint ShaderMgr::getAttribLocation(std::string attribute) {
			return m_attributesLocations.find(attribute) != m_attributesLocations.end() ?
				m_attributesLocations[attribute] :
				-1;
		}

		const char * ShaderMgr::readFile(const char * path) {
			std::ifstream t;
			int length;

			t.open(path);
			if (t.fail()) {
				return nullptr;
			}

			t.seekg(0, std::ios::end);
			length = t.tellg();
			t.seekg(0, std::ios::beg);
			char * buffer = new char[length + 1];
			memset(buffer, 0, sizeof(char)*(length + 1));
			t.read(buffer, length);
			t.close();

			buffer[length] = '\0';
			return buffer;
		}

		void ShaderMgr::debugGL(const char * function) {
			GLenum err;
			std::cerr << function << " ";
			while ((err = glGetError()) != GL_NO_ERROR) {
				std::cerr << err << " ";
			}
			std::cerr << std::endl;
		}

		const GLuint ShaderMgr::getUBO() const {
			return m_globalMatricesUBO;
		}

	}

}