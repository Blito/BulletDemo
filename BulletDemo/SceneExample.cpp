#include "SceneExample.h"
#include "Box.h"
#include "Plane.h"
#include "Cloth.h"
#include "RigidObject.h"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#define ANGLEH 0
#define ANGLEV 0
#define XCAM 0
#define YCAM 0
#define ZCAM 5
#define MOV_SPEED 0.0045f
#define ROT_SPEED 0.004f
#define MOUSE_SENSITIVITY 0.8f
#define FONT_FILE "resources/consola.ttf"

using namespace LittleLab;

SceneExample::SceneExample() : 
	angleH(ANGLEH), 
	angleV(ANGLEV),
	xCam(XCAM),
	yCam(YCAM),
	zCam(ZCAM),
	quit(false),
	timeElapsed(0), 
	loops(0),
	mov_speed(MOV_SPEED),
	rot_speed(ROT_SPEED),
	mouse_sensitivity(MOUSE_SENSITIVITY)
{

	lx = sin(angleH);
	ly = sin(angleV);
	lz = -cos(angleH);

	// Managers init
	// TODO: GET THIS OUT OF THE SCENEEXAMPLE!1!!

	// Init fonts
	if (TTF_Init() != 0) {
		//logSDLError(std::cout, "TTF_Init");
		return;
	}

	//Open the font
	font = TTF_OpenFont(FONT_FILE, 12);
	if (font == nullptr) {
		//logSDLError(std::cout, "TTF_OpenFont");
		return;
	}

	// physics init
	world = physicsMgr.getWorld();

	proj = glm::perspective(45.0f, 800.0f / 600.0f, 0.1f, 200.0f);


	shaderMgr.debugGL("glBufferSubData");

	GLuint shaderProgram = shaderMgr.createProgram("../BulletDemo/shaders/vertex.shader", "../BulletDemo/shaders/fragment.shader");
	
	glBindBuffer(GL_UNIFORM_BUFFER, shaderMgr.getUBO());
    glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(glm::mat4), glm::value_ptr(proj));
    glBindBuffer(GL_UNIFORM_BUFFER, 0);

	RigidObject::load();
	Box::load(shaderProgram);
	Plane::load(shaderProgram);
	Cloth::load(shaderProgram);

	RenderedObject * plane = new Plane(-2);
	plane->addToWorld(world);
	toRender.push_back(plane);
	
	// Create cloth
	float s = 3.0, h = 5.0;
	RenderedObject * cloth = new Cloth(world->getWorldInfo(), 
				btVector3(-s, h, -s), btVector3(s, h, -s), btVector3(-s, h, s), btVector3(s, h, s),
				10, 10, 4+8);
	cloth->addToWorld(world);
	toRender.push_back(cloth);

	// Create cubes
	
	createBox(1, 2, 1,-3,0,0, 2);
	createBox(2, 1, 1,0,0,0,5);
	createBox(1, 1, 3,0,3,0.7,3);
	createBox(1, 1, 1,3,0,0,7);

	// Create house
	std::string filename = "../resources/teapot.obj";
	RenderedObject * house = new RigidObject(filename);
	//toRender.push_back(house);
}

SceneExample::~SceneExample() {
	//Clean up the surface and font
	//SDL_FreeSurface(surf);
	TTF_CloseFont(font);
}

void SceneExample::update(Uint32 elapsedTimeInMillis) {
	SDL_Event e;

	const Uint8 *keys = SDL_GetKeyboardState(NULL);
	while (SDL_PollEvent(&e)){
		if (e.type == SDL_QUIT)
			quit = true;
		/* If a button on the mouse is pressed. */
        if (e.type == SDL_MOUSEBUTTONDOWN)
        {
            /* If the left button was pressed. */
            if (e.button.button == SDL_BUTTON_LEFT) {
				btRigidBody * bullet = createBox(0.5f, 0.5f, 0.5f, xCam, yCam, zCam, 3)->getRigidBody();
				btVector3 direction(lx, ly, lz);
				bullet->setLinearVelocity(direction*12);
				//bullet->applyCentralForce(direction*-1);
			}                
        }
		//if (e.type == SDL_MOUSEMOTION)
		//{
		//	/* If the mouse is moving to the left */
		//	if (e.motion.xrel < 0) {
		//		angleH -= rot_speed * -e.motion.xrel * mouse_sensitivity * elapsedTimeInMillis;
		//	}
		//	/* If the mouse is moving to the right */
		//	else if (e.motion.xrel > 0) {
		//		angleH += rot_speed * e.motion.xrel * mouse_sensitivity * elapsedTimeInMillis;
		//	}
		//	/* If the mouse is moving up */
		//	if (e.motion.yrel < 0) {
		//		if (angleV < 0.5)
		//			angleV += rot_speed * -e.motion.yrel * mouse_sensitivity * elapsedTimeInMillis;
		//		else
		//			angleV = 0.5f;
		//	}
		//	/* If the mouse is moving down */
		//	else if (e.motion.yrel > 0) {
		//		if (angleV > -0.5)
		//			angleV -= rot_speed * e.motion.yrel * mouse_sensitivity * elapsedTimeInMillis;
		//		else
		//			angleV = -0.5f;
		//	}
		//	lx = sin(angleH);
		//	ly = sin(angleV);
		//	lz = -cos(angleH);
		//}
	}
	if (keys[SDL_SCANCODE_W]) {
		xCam += lx * mov_speed * elapsedTimeInMillis;
		zCam += lz * mov_speed * elapsedTimeInMillis;
	}
	if (keys[SDL_SCANCODE_A]) {
		angleH -= rot_speed * elapsedTimeInMillis;
		lx = sin(angleH);
		lz = -cos(angleH);
	}
	if (keys[SDL_SCANCODE_S]) {
		xCam -= lx * mov_speed * elapsedTimeInMillis;
		zCam -= lz * mov_speed * elapsedTimeInMillis;
	}
	if (keys[SDL_SCANCODE_D]) {
		angleH += rot_speed * elapsedTimeInMillis;
		lx = sin(angleH);
		lz = -cos(angleH);
	}
	if (keys[SDL_SCANCODE_Q]) {
		xCam -= cos(angleH) * mov_speed * elapsedTimeInMillis;
		zCam += -sin(angleH) * mov_speed * elapsedTimeInMillis;
	}
	if (keys[SDL_SCANCODE_E]) {
		xCam += cos(angleH) * mov_speed * elapsedTimeInMillis;
		zCam -= -sin(angleH) * mov_speed * elapsedTimeInMillis;
	}
	if (keys[SDL_SCANCODE_UP]) {
		angleV += rot_speed * elapsedTimeInMillis;
		ly = sin(angleV);
	}
	if (keys[SDL_SCANCODE_DOWN]) {
		angleV -= rot_speed * elapsedTimeInMillis;
		ly = sin(angleV);
	}
	if (keys[SDL_SCANCODE_SPACE]) {
		yCam = 1.5;
	}
	if (keys[SDL_SCANCODE_ESCAPE]) {
		quit = true;
	}
	
	yCam = yCam > 0 ? yCam - 0.005f : 0;
	physicsMgr.getWorld()->stepSimulation((float)elapsedTimeInMillis/1000.0f);
}

void SceneExample::render() {

	view = glm::lookAt(
		glm::vec3(xCam, yCam, zCam),
		glm::vec3(xCam+lx, yCam+ly, zCam+lz),
		glm::vec3(0.0f, 1.0f, 0.0f)
		);

	glm::mat4 parentMatrix = proj * view;

	glBindBuffer(GL_UNIFORM_BUFFER, shaderMgr.getUBO());
	glBufferSubData(GL_UNIFORM_BUFFER, sizeof(glm::mat4), sizeof(glm::mat4), glm::value_ptr(view));
	glBindBuffer(GL_UNIFORM_BUFFER, 0);
	
	for (int i = 0; i < toRender.size(); i++) {
		toRender[i]->render(proj, view, parentMatrix);
	}	

	glFlush();
}

Box * SceneExample::createBox(float width, float height, float depth, float x, float y, float z, float mass) {
	Box * box = new Box(width, height, depth, x, y, z, mass);
	box->addToWorld(world);
	toRender.push_back(box);
	return box;
}

/**
* Render the message we want to display to a texture for drawing
* @param message The message we want to display
* @param color The color we want the text to be
* @param renderer The renderer to load the texture in
* @return An SDL_Texture containing the rendered message, or nullptr if something went wrong
*/
void SceneExample::createText(const std::string &message, 
	SDL_Color color)
{
	
	//We need to first render to a surface as that's what TTF_RenderText
	//returns, then load that surface into a texture
	SDL_Surface *surf = TTF_RenderText_Blended(font, message.c_str(), color);
	if (surf == nullptr){
		TTF_CloseFont(font);
		//logSDLError(std::cout, "TTF_RenderText");
	}
	text = SDL_CreateTextureFromSurface(renderer, surf);

}