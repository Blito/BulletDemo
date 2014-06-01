#include "Cloth.h"
#include <SDL2/SDL_opengl.h>

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
	softBody->setTotalMass(3.0);
	softBody->m_cfg.viterations = 20;
	softBody->m_cfg.piterations = 20;
}


Cloth::~Cloth(void)
{
}

void Cloth::render(glm::mat4 parentTransform) {
	glColor3f(1.0, 0.0, 1.0);
	glBegin(GL_TRIANGLES);
	// For each face in the cloth
	for (int i = 0; i < softBody->m_faces.size(); i++) {
		// For each vertex in the face
		for (int j = 0; j < 3; j++) {
			glVertex3f(softBody->m_faces[i].m_n[j]->m_x.x(),
					   softBody->m_faces[i].m_n[j]->m_x.y(),
					   softBody->m_faces[i].m_n[j]->m_x.z());
		}
	}
	glEnd();
	glColor3f(0.0, 0.0, 1.0);
	glBegin(GL_LINES);
	// For each link in the cloth
	for (int i = 0; i < softBody->m_links.size(); i++) {
		// For each vertex in the link
		for (int j = 0; j < 2; j++) {
			glVertex3f(softBody->m_links[i].m_n[j]->m_x.x(),
					   softBody->m_links[i].m_n[j]->m_x.y(),
					   softBody->m_links[i].m_n[j]->m_x.z());
		}
	}
	glEnd();
}

bool Cloth::addToWorld(btDynamicsWorld * world) {
	if (world->getWorldType() == btDynamicsWorldType::BT_SOFT_RIGID_DYNAMICS_WORLD) {
		((btSoftRigidDynamicsWorld*)world)->addSoftBody(softBody);
		return true;
	}
	return false;
}