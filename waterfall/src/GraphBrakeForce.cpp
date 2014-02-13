#include "GraphBrakeForce.hpp"

namespace imac3
{

	void GraphBrakeForce::apply(ParticleManager& pm)
	{

		if (m_pGraph != NULL)
		{
			for (ParticleGraph::const_iterator it = m_pGraph->begin() ; it != m_pGraph->end(); ++it)
			{
				std::pair<int, unsigned int> pair1 = std::get<0>(*it);
				std::pair<int, unsigned int> pair2 = std::get<1>(*it);
				glm::vec2 v1 = pm.getSpeed(pair1.first, pair1.second);
				glm::vec2 v2 = pm.getSpeed(pair2.first, pair2.second);
				if(m_fDT > 0)
				{
					glm::vec2 F = m_fV * ((v2-v1) / m_fDT);
					pm.applyTo(pair1.first, pair1.second, F);
					pm.applyTo(pair2.first, pair2.second, -F);
				}
			}
		}

	}

}