#ifndef _H_IMAC3_CONSTANTFORCE_H_
#define _H_IMAC3_CONSTANTFORCE_H_

#include "Force.hpp"
#include <glm/glm.hpp>


namespace imac3 
{

class ConstantForce : public Force
{

private :
	glm::vec2 m_force;

public :
	ConstantForce(glm::vec2 force);
	~ConstantForce(){}
	void apply(ParticleManager & pm);
	
};

}

#endif // _H_IMAC3_CONSTANTFORCE_H_

