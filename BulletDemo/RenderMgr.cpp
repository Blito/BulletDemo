#include "RenderMgr.h"

namespace LittleLab {

	namespace Graphics {

		const glm::mat4 & RenderMgr::pushMatrix(glm::mat4 m) {
			const glm::mat4 newTop = m_globalTransf.top() * m;
			m_globalTransf.push(newTop);
			return newTop;
		}

		const glm::mat4 & RenderMgr::popMatrix() {
			m_globalTransf.pop();
			return m_globalTransf.top();
		}

		const glm::mat4 & RenderMgr::currentMatrix() const {
			return m_globalTransf.top();
		}

	}

}