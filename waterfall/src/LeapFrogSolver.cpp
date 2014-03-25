#include "LeapFrogSolver.hpp"
#include <iostream>

namespace imac3 
{

const float MAX_SPEED = -0.3f;

	void LeapFrogSolver::solve(ParticleManager& pm, float dt)
	{
		int nb_cells = pm.getNumGridCell();
		
		// For each cell
		for (size_t c = 0; c < nb_cells; ++c)
		{

			// For each particle in this cell
			for(int p = 0; p < pm.getNumberParticles(c); ++p)
			{
				//fprintf(stderr, "solve particle %d of cell %lu (size = %d)\n", p, c, pm.getNumberParticles(c));
				glm::vec2 newSpeed = pm.getSpeed(c, p) + dt * pm.getForceBuffer(c, p)/pm.getMass(c, p);
				if(newSpeed.y < MAX_SPEED)
				{
					newSpeed.y = MAX_SPEED;
				}
				glm::vec2 newPos = pm.getPosition(c, p) + dt * newSpeed;
				pm.setSpeed(c, p, newSpeed);
				pm.setPosition(c, p, newPos);
				pm.setForceBuffer(c, p, glm::vec2(0, 0));
				if(newSpeed.y == 0)
				{
					pm.incrementNonMovableDuration(c, p, 1);
					if(pm.getNonMovableDuration(c, p) > 1)
					{
						pm.deleteParticle(c, p);
					}
				}
				if(newPos.y < -1)
				{
					pm.deleteParticle(c, p);
				}
				else
				{
					pm.resetNonMovableDuration(c, p);
				}			
			}			
		}

		for (size_t c = 0; c < nb_cells; ++c)
		{
			// For each particle in this cell
			for(int p = 0; p < pm.getNumberParticles(c); ++p)
			{
				pm.updateGridIndexes(c, p);
			}			
		}
	}
	
	ParticleState LeapFrogSolver::getNextState(int cell_id, uint32_t index, ParticleManager& pm, float dt) const
	{
		glm::vec2 newSpeed = pm.getSpeed(cell_id, index) + dt * pm.getForceBuffer(cell_id, index)/pm.getMass(cell_id, index);
		glm::vec2 newPos = pm.getPosition(cell_id, index) + dt * newSpeed;
		return ParticleState(newPos, newSpeed);
	}

}
