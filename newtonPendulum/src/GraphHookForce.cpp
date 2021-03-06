#include "GraphHookForce.hpp"
#include "HookForce.hpp"
#include <glm/glm.hpp>
#include <glm/gtx/norm.hpp>

namespace imac3
{

	void GraphHookForce::apply(ParticleManager& pm)
	{
		//Old version
		/*
		for (size_t i=0; i<pm.getNumberParticles(); ++i)
		{
			for (size_t j=i+1; j<pm.getNumberParticles(); ++j){

				glm::vec2 P1 = pm.getPosition(i);
				glm::vec2 P2 = pm.getPosition(j);
				glm::vec2 P1P2 = P2 - P1;
				float distP1P2 = glm::l2Norm(glm::vec3(P1P2, 0));
				glm::vec2 F = m_fK * (1 - ( m_fL / glm::max( distP1P2 , e ))) * P1P2;
				pm.applyTo(i, F);
				pm.applyTo(j, -F);
			}
		}
		*/
		//Version with particule Graph
		if (m_pGraph != NULL){
			for (ParticleGraph::const_iterator it = m_pGraph->begin() ; it != m_pGraph->end(); ++it){
				glm::vec2 P1 = pm.getPosition(std::get<0>(*it));
				glm::vec2 P2 = pm.getPosition(std::get<1>(*it));
				glm::vec2 P1P2 = P2 - P1;
				float distP1P2 = glm::l2Norm(glm::vec3(P1P2, 0));
				glm::vec2 F = m_fK * (1 - ( m_fL / glm::max( distP1P2 , e ))) * P1P2;
				pm.applyTo(std::get<0>(*it), F);
				pm.applyTo(std::get<1>(*it), -F);
			}
		}

	}

}
