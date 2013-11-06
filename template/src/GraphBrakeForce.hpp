#ifndef _H_IMAC3_GRAPHBRAKEFORCE_H_
#define _H_IMAC3_GRAPHBRAKEFORCE_H_

#include "Force.hpp"
#include "ParticleManager.hpp"

namespace imac3
{

class GraphBrakeForce : public Force
{
private:
	float m_fV;
	float m_fDT;
	const ParticleGraph* m_pGraph;

public:
	GraphBrakeForce(float V, float DT, ParticleGraph* pGraph=NULL) : m_fV(V), m_fDT(DT), m_pGraph(pGraph){}
	void apply(ParticleManager& pm);	

	void setDt(float dt) {m_fDT = dt; }
	void setGraph(const ParticleGraph* pGraph){ m_pGraph = pGraph;}
};

}

#endif