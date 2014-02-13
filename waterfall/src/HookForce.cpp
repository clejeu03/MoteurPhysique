#include "HookForce.hpp"
#include <glm/glm.hpp>
#include <glm/gtx/norm.hpp>

namespace imac3
{

	int PENDULUM_NUMBER = 5;
	float e = 0.001;

	void HookForce::apply(ParticleManager& pm)
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
					glm::vec2 P1 = pm.getPosition(c, p1);
					glm::vec2 P2 = pm.getPosition(c, p2);
					glm::vec2 P1P2 = P2 - P1;
					float distP1P2 = glm::l2Norm(glm::vec3(P1P2, 0));
					glm::vec2 F = m_fK * (1 - ( m_fL / glm::max( distP1P2 , e ))) * P1P2;
					pm.applyTo(c, p1, F);
					pm.applyTo(c, p2, -F);
				}
			}
		}
	}

}