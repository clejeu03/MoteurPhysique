#include "PolygonForce.hpp"

namespace imac3
{

	void PolygonForce::setDt(const float& dt)
	{
		m_fDt = dt;
	}
	
	void PolygonForce::apply(ParticleManager& pm)
	{
		// For each edge
		size_t verticescount = m_Polygon->getNumberVertices();
		
		/*if(m_Polygon->isInner())
		{*/
			for(size_t v = 0; v < verticescount; ++v)
			{
				glm::vec2 A = m_Polygon->getVertex(v);
				glm::vec2 B = m_Polygon->getVertex((v+1)%(verticescount));
				// For each particle
				size_t particlecount = pm.getNumberParticles();
				for(size_t p = 0; p < particlecount; ++p)
				{
					// Intersection test
					glm::vec2 P1 = pm.getPosition(p);
					glm::vec2 P2 = m_Solver->getNextState(p, pm, m_fDt).position;
					glm::vec2 intersection, normal;
					if(m_Polygon->intersect(P1, P2, A, B, &intersection, &normal))
					{
						std::cout << "interection between edge (" << A.x << "," << A.y << ")(" << B.x << "," << B.y <<") ";
						std::cout << "and traj (" << P1.x << "," << P1.y << ")(" << P2.x << "," << P2.y <<")" << std::endl;
						
						// Calculate force
						glm::vec2 nextvel = m_Solver->getNextState(p, pm, m_fDt).velocity;
						std::cout << "nextVel : " << nextvel.x << ", " << nextvel.y << std::endl;
						float m = pm.getMass(p);
						std::cout << "mass : " << m << std::endl;
						
						std::cout << "normal : " << normal.x << ", " << normal.y << std::endl;
						std::cout << "intersect : " << intersection.x << ", " << intersection.y << std::endl;
						
						glm::vec2 F = m_fElasticity * glm::dot(nextvel, -normal) * m/m_fDt * normal;
						std::cout << "force to apply : " << F.x << ", " << F.y << std::endl;
						pm.applyTo(p, F);
					}
				}
			}
		/*}*/
		/*else
		{
			for(size_t v = verticescount-1; v >= 0; --v)
			{
				glm::vec2 P1 = m_Polygon->getVertex(v);
				glm::vec2 P2;
				if(v > 0)
				{
					P2 = m_Polygon->getVertex(v-1);
				}
				else
				{
					P2 = m_Polygon->getVertex(verticescount-1);
				}
				
				// For each particle
				size_t particlecount = pm.getNumberParticles();
				for(size_t p = 0; p < particlecount; ++p)
				{
					// Intersection test
					glm::vec2 A = pm.getPosition(p);
					glm::vec2 B = m_Solver->getNextState(p, pm, m_fDt).position;
					glm::vec2 intersection, normal;
					std::cout << "test intersection between edge (" << P1.x << "," << P1.y << ")(" << P2.x << "," << P2.y <<") ";
					std::cout << "and particle (" << A.x << "," << A.y << ")(" << B.x << "," << B.y <<")" << std::endl;
					if(m_Polygon->intersect(P1, P2, A, B, &intersection, &normal))
					{
						std::cout << "intersect" << std::endl;
						// Calculate force
						glm::vec2 nextvel = m_Solver->getNextState(p, pm, m_fDt).velocity;
						float m = pm.getMass(p);
						glm::vec2 F = m_fElasticity * glm::dot(nextvel, -normal) * m/m_fDt * normal;
						pm.applyTo(p, F);
					}
				}
			}
		}*/
	}
	
}
