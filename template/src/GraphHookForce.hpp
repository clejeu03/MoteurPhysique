#ifndef _H_IMAC3_GRAPHHOOKFORCE_H_
#define _H_IMAC3_GRAPHHOOKFORCE_H_

#include "Force.hpp"
#include "ParticleManager.hpp"

namespace imac3
{

class GraphHookForce : public Force
{
private :
	float m_fK;
	float m_fL;
	const ParticleGraph* m_pGraph;

public:
	GraphHookForce(float K, float L, ParticleGraph* pGraph=NULL) : m_fK(K), m_fL(L), m_pGraph(pGraph){}
	void apply(ParticleManager& pm);
	
	float getK() const { return m_fK; }
	float getL() const { return m_fL; }
	void setK(const float& k) { m_fK = k; }
	void setL(const float& l) { m_fL = l; }
	void setGraph(const ParticleGraph* pGraph){ m_pGraph = pGraph;}

};

}

#endif