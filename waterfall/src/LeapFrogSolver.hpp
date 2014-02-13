#ifndef _H_IMAC3_LEAPFROGSOLVER_H_
#define _H_IMAC3_LEAPFROGSOLVER_H_

#include "renderer/ParticleRenderer2D.hpp"
#include "ParticleManager.hpp"
#include <glm/glm.hpp>
#include <glm/gtx/norm.hpp>

namespace imac3 
{

struct ParticleState
{
	glm::vec2 position;
	glm::vec2 velocity;
	ParticleState(glm::vec2 pos, glm::vec2 vel) : position(pos), velocity(vel){}
};

class LeapFrogSolver
{
private :

public :
	void solve(ParticleManager& pm, float dt);
	ParticleState getNextState(int cell_id, uint32_t index, ParticleManager& pm, float dt) const;
};

}

#endif // _H_IMAC3_LEAPFROGSOLVER_H_

