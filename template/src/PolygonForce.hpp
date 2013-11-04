#ifndef _H_IMAC3_POLYGONFORCE_H_
#define _H_IMAC3_POLYGONFORCE_H_

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
	PolygonForce(const Polygon& polygon, float elasticity, const LeapfrogSolver& solver) :
		m_Polygon(polygon),
		m_fElasticity(elasticity),
		m_Solver(solver),
		m_fDt(0);
}

}

#endif
