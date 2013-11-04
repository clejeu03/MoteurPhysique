#ifndef _H_IMAC3_LEAPFROGSOLVER_H_
#define _H_IMAC3_LEAPFROGSOLVER_H_

#include "renderer/ParticleRenderer2D.hpp"
#include "ParticleManager.hpp"

namespace imac3 
{

class LeapFrogSolver
{
private :

public :
	void solve(ParticleManager& pm, float dt);
};

}

#endif // _H_IMAC3_LEAPFROGSOLVER_H_

