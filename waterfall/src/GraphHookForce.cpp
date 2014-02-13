#include "GraphHookForce.hpp"
#include "HookForce.hpp"
#include <glm/glm.hpp>
#include <glm/gtx/norm.hpp>

namespace imac3
{

	void GraphHookForce::apply(ParticleManager& pm)
	{
		//Version with particule Graph
		if (m_pGraph != NULL)
		{
			for (ParticleGraph::const_iterator it = m_pGraph->begin() ; it != m_pGraph->end(); ++it)
			{
				std::pair<int, unsigned int> pair1 = std::get<0>(*it);
				std::pair<int, unsigned int> pair2 = std::get<1>(*it);
				glm::vec2 P1 = pm.getPosition(pair1.first, pair1.second);
				glm::vec2 P2 = pm.getPosition(pair1.first, pair1.second);
				glm::vec2 P1P2 = P2 - P1;
				float distP1P2 = glm::l2Norm(glm::vec3(P1P2, 0));
				glm::vec2 F = m_fK * (1 - ( m_fL / glm::max( distP1P2 , e ))) * P1P2;
				pm.applyTo(pair1.first, pair1.second, F);
				pm.applyTo(pair2.first, pair2.second, -F);
			}
		}
	}
}
