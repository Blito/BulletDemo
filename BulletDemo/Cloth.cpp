#include "Cloth.h"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <vector>
#include <iostream>

// Vertex Shader
const GLchar* Cloth::vertexSource = 
	"#version 150 core\n"
	"in vec3 position;"
	"in vec3 color;"
	"out vec3 Color;"
	"uniform mat4 pvm;"
	"void main() {"
	"	Color = color;"
	"   gl_Position = pvm * vec4(position, 1.0);"
	"}";

// Fragment Shader
const GLchar* Cloth::fragmentSource = 
	"#version 150 core\n"
	"in vec3 Color;"
	"out vec4 outColor;"
	"void main() {"
	"   outColor = vec4(Color, 1.0);"
	"}";

GLint Cloth::uniPVM = 0;
GLuint Cloth::vbo = 0;
GLint Cloth::posAttrib = 0;
GLint Cloth::colAttrib = 0;
GLuint Cloth::shaderProgram = 0;

bool Cloth::load() {
	// Create Vertex Array Object
    GLuint vao;
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

	// Create a Vertex Buffer Object and copy the vertex data to it
    glGenBuffers(1, &vbo);

    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    //glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);*/
	
    // Create and compile the vertex shader
    GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexSource, NULL);
    glCompileShader(vertexShader);

    // Create and compile the fragment shader
    GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentSource, NULL);
    glCompileShader(fragmentShader);

    // Link the vertex and fragment shader into a shader program
    shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glBindFragDataLocation(shaderProgram, 0, "outColor");
    glLinkProgram(shaderProgram);
    glUseProgram(shaderProgram);

    // Specify the layout of the vertex data
	// in vec3 position;
    posAttrib = glGetAttribLocation(shaderProgram, "position");
    glEnableVertexAttribArray(posAttrib);
    glVertexAttribPointer(posAttrib, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), 0);

	// in vec3 color;
	colAttrib = glGetAttribLocation(shaderProgram, "color");
	glEnableVertexAttribArray(colAttrib);
	glVertexAttribPointer(colAttrib, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (void*)(3 * sizeof(GLfloat)));

	uniPVM = glGetUniformLocation(shaderProgram, "pvm");
	
	return true;
}

Cloth::Cloth(btSoftBodyWorldInfo& worldInfo,
		const btVector3& corner00,
		const btVector3& corner10,
		const btVector3& corner01,
		const btVector3& corner11,
		int resx,
		int resy,
		int fixeds)
{
	softBody = btSoftBodyHelpers::CreatePatch(worldInfo, corner00, corner10, corner01, corner11, resx, resy, fixeds, true);
	softBody->setTotalMass(1.0);
	softBody->m_cfg.viterations = 20;
	softBody->m_cfg.piterations = 20;
}


Cloth::~Cloth(void)
{
}

void Cloth::render(glm::mat4 parentTransform) {

	int vertCount = softBody->m_faces.size()*3*8;
	GLfloat * vertices = new GLfloat[vertCount];

	// For each face in the cloth
	for (int i = 0; i < softBody->m_faces.size(); i++) {
		// For each vertex in the face
		for (int j = 0; j < 3; j++) {
			vertices[(i*3+j)*8] = softBody->m_faces[i].m_n[j]->m_x.x();   // x 
			vertices[(i*3+j)*8+1] = softBody->m_faces[i].m_n[j]->m_x.y(); // y 
			vertices[(i*3+j)*8+2] = softBody->m_faces[i].m_n[j]->m_x.z(); // z 
			vertices[(i*3+j)*8+3] = 1.0f;								  // r 
			vertices[(i*3+j)*8+4] = 0.0f;								  // g 
			vertices[(i*3+j)*8+5] = 1.0f;								  // b 
			vertices[(i*3+j)*8+6] = 0.0f;								  // u 
			vertices[(i*3+j)*8+7] = 0.0f;								  // v 
		}
	}

	glm::mat4 pvm = parentTransform;
	
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat)*vertCount, &(*vertices), GL_DYNAMIC_DRAW);
	
	glVertexAttribPointer(posAttrib, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), 0);
	glVertexAttribPointer(colAttrib, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (void*)(3 * sizeof(GLfloat)));

	glUniformMatrix4fv(uniPVM, 1, GL_FALSE, glm::value_ptr(pvm));

	glDrawArrays(GL_TRIANGLES, 0, vertCount);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

bool Cloth::addToWorld(btDynamicsWorld * world) {
	if (world->getWorldType() == btDynamicsWorldType::BT_SOFT_RIGID_DYNAMICS_WORLD) {
		((btSoftRigidDynamicsWorld*)world)->addSoftBody(softBody);
		return true;
	}
	return false;
}