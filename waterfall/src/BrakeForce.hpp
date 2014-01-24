#ifndef _H_IMAC3_BRAKEFORCE_H_
#define _H_IMAC3_BRAKEFORCE_H_

#include "Force.hpp"
#include "ParticleManager.hpp"

namespace imac3
{

class BrakeForce : public Force
{
private :
	float m_fV;
	float m_fDT;

public:
	BrakeForce(float V, float DT) : m_fV(V), m_fDT(DT){}
	void apply(ParticleManager& pm);

	void setDt(float dt) {m_fDT = dt; }
};

}

#endif