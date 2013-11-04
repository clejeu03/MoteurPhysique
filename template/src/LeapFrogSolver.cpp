#include "LeapFrogSolver.hpp"

namespace imac3 
{

	void LeapFrogSolver::solve(ParticleManager& pm, float dt)
	{
		size_t count = pm.getNumberParticles();
		for (size_t i = 0; i < count; ++i){
			glm::vec2 newSpeed = pm.getSpeed(i) + dt * pm.getForceBuffer(i)/pm.getMass(i);
			glm::vec2 newPos = pm.getPosition(i) + dt * newSpeed;
			pm.setSpeed(i, newSpeed);
			pm.setPosition(i, newPos);
			pm.setForceBuffer(i, glm::vec2(0, 0));
		}
	}

}
