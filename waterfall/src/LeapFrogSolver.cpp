#include "LeapFrogSolver.hpp"
#include <iostream>

namespace imac3 
{

const float MAX_SPEED = -0.3f;

	void LeapFrogSolver::solve(ParticleManager& pm, float dt)
	{
		size_t count = pm.getNumberParticles();
		for (size_t i = 0; i < count; ++i){
			glm::vec2 newSpeed = pm.getSpeed(i) + dt * pm.getForceBuffer(i)/pm.getMass(i);
			if(newSpeed.y < MAX_SPEED)
			{
				newSpeed.y = MAX_SPEED;
			}
			glm::vec2 newPos = pm.getPosition(i) + dt * newSpeed;
			pm.setSpeed(i, newSpeed);
			pm.setPosition(i, newPos);
			pm.setForceBuffer(i, glm::vec2(0, 0));
			if(newPos.y < -1)
			{
				pm.deleteParticle(i);
			}
		}
	}
	
	ParticleState LeapFrogSolver::getNextState(uint32_t id, ParticleManager& pm, float dt) const
	{
		//std::cout << "calculate next state of particle" << pm.getPosition(id).x << ", " << pm.getPosition(id).y << std::endl;
		glm::vec2 newSpeed = pm.getSpeed(id) + dt * pm.getForceBuffer(id)/pm.getMass(id);
		//std::cout << "new speed is " << newSpeed.x << ", " << newSpeed.y << std::endl;
		//std::cout << "force bufer is " << pm.getForceBuffer(id).x << ", " << pm.getForceBuffer(id).y << std::endl;
		//std::cout << "dt " << dt << std::endl;
		glm::vec2 newPos = pm.getPosition(id) + dt * newSpeed;
		//std::cout << "new pos is " << newPos.x << ", " << newPos.y << std::endl;
		return ParticleState(newPos, newSpeed);
	}

}
