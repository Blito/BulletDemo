#pragma once
#include "singleton.h"
#include <stack>
#include <glm\glm.hpp>

namespace LittleLab {

	namespace Graphics {

		class RenderMgr :
			public Utils::Singleton<RenderMgr>
		{
		public:
			RenderMgr(void) {};
			virtual ~RenderMgr(void) {};
			const glm::mat4 & pushMatrix(glm::mat4 m);
			const glm::mat4 & popMatrix();
			const glm::mat4 & currentMatrix() const;

		private:
			std::stack<glm::mat4> m_globalTransf;
		};

	}

}
