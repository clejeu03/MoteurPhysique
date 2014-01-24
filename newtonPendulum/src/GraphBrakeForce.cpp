#include "GraphBrakeForce.hpp"

namespace imac3
{

	void GraphBrakeForce::apply(ParticleManager& pm)
	{

		//Old Version
		/*
		for (size_t i=0; i<pm.getNumberParticles(); ++i)
		{
			for (size_t j=i+1; j<pm.getNumberParticles(); ++j){

				glm::vec2 v1 = pm.getSpeed(i);
				glm::vec2 v2 = pm.getSpeed(j);
				if(m_fDT > 0)
				{
					glm::vec2 F = m_fV * ((v2-v1) / m_fDT);
					pm.applyTo(i, F);
					pm.applyTo(j, -F);
				}
				
			}
		}
		*/
		//Version with particule Graph
		if (m_pGraph != NULL){
			for (ParticleGraph::const_iterator it = m_pGraph->begin() ; it != m_pGraph->end(); ++it){
				glm::vec2 v1 = pm.getSpeed(std::get<0>(*it));
				glm::vec2 v2 = pm.getSpeed(std::get<1>(*it));
				if(m_fDT > 0)
				{
					glm::vec2 F = m_fV * ((v2-v1) / m_fDT);
					pm.applyTo(std::get<0>(*it), F);
					pm.applyTo(std::get<1>(*it), -F);
				}
			}
		}

	}

}