#include "BrakeForce.hpp"

namespace imac3
{

	void BrakeForce::apply(ParticleManager& pm)
	{
		// For each cell
		for(size_t c = 0; c < pm.getNumGridCell(); ++c)
		{
			// For each particle in this cell
			for(size_t p1 = 0; p1 < pm.getNumberParticles(c); ++p1)
			{
				// Apply the force for each other particles in the same cell
				for(size_t p2 = p1+1; p2 < pm.getNumberParticles(c); ++p2)
				{
					glm::vec2 v1 = pm.getSpeed(c, p1);
					glm::vec2 v2 = pm.getSpeed(c, p2);
					if(m_fDT > 0)
					{
						glm::vec2 F = m_fV * ((v2-v1) / m_fDT);
						pm.applyTo(c, p1, F);
						pm.applyTo(c, p2, -F);
					}
				}
			}
		}
	}

}