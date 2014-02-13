#include "ConstantForce.hpp"

namespace imac3
{

	ConstantForce::ConstantForce(glm::vec2 force) : m_force(force){}

	void ConstantForce::apply(ParticleManager & pm)
	{
		size_t count = pm.getNumberParticles();
		pm.applyToAll(m_force);	
	}

}
