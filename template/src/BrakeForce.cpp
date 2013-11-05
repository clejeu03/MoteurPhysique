#include "BrakeForce.hpp"

namespace imac3
{

	void BrakeForce::apply(ParticleManager& pm)
	{

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

	}

}