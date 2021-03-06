#ifndef _H_IMAC3_POLYGONFORCE_H_
#define _H_IMAC3_POLYGONFORCE_H_

#include "Force.hpp"
#include "Polygon.hpp"
#include "LeapFrogSolver.hpp"
#include "ParticleManager.hpp"

namespace imac3
{

class PolygonForce : public Force
{
private :
	const Polygon* m_Polygon;
	float m_fElasticity;
	const LeapFrogSolver* m_Solver;
	float m_fDt;
public:
	PolygonForce(const Polygon& polygon, float elasticity, const LeapFrogSolver& solver) :
		m_Polygon(&polygon),
		m_fElasticity(elasticity),
		m_Solver(&solver),
		m_fDt(0){};
		
	void setDt(const float& dt);
	void apply(ParticleManager& pm);
};

}

#endif
