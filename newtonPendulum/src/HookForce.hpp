#ifndef _H_IMAC3_HOOKFORCE_H_
#define _H_IMAC3_HOOKFORCE_H_

#include "Force.hpp"
#include "ParticleManager.hpp"

namespace imac3
{
	extern int PENDULUM_NUMBER;
	extern float e;

class HookForce : public Force
{
private :
	float m_fK;
	float m_fL;

public:
	HookForce(float K, float L) : m_fK(K), m_fL(L){}
	void apply(ParticleManager& pm);
	
	float getK() const { return m_fK; }
	float getL() const { return m_fL; }
	void setK(const float& k) { m_fK = k; }
	void setL(const float& l) { m_fL = l; }

};

}

#endif