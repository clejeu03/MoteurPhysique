#include "PolygonForce.hpp"

namespace imac3
{

	void PolygonForce::setDt(const float& dt)
	{
		m_fDt = dt;
	}
	
	// Cas d'une collision normale entre des particules et un objet
	// void PolygonForce::apply(ParticleManager& pm)
	// {
	// 	// For each edge
	// 	size_t verticescount = m_Polygon->getNumberVertices();
		
	// 	if(m_Polygon->isInner())
	// 	{
	// 		for(size_t v = 0; v < verticescount; ++v)
	// 		{
	// 			glm::vec2 A = m_Polygon->getVertex(v);
	// 			glm::vec2 B = m_Polygon->getVertex((v+1)%(verticescount));
	// 			// For each particle
	// 			size_t particlecount = pm.getNumberParticles();
	// 			for(size_t p = 0; p < particlecount; ++p)
	// 			{
	// 				// Intersection test
	// 				glm::vec2 P1 = pm.getPosition(p);
	// 				glm::vec2 P2 = m_Solver->getNextState(p, pm, m_fDt).position;
	// 				glm::vec2 intersection, normal;
	// 				if(m_Polygon->intersect(P1, P2, A, B, &intersection, &normal))
	// 				{


	// 					// Calculate force
	// 					glm::vec2 nextvel = m_Solver->getNextState(p, pm, m_fDt).velocity;
	// 					//std::cout << "nextVel : " << nextvel.x << ", " << nextvel.y << std::endl;
	// 					float m = pm.getMass(p);
	// 					//std::cout << "mass : " << m << std::endl;
						
	// 					//std::cout << "intersect : " << intersection.x << ", " << intersection.y << std::endl;
						
	// 					glm::vec2 F = m_fElasticity * glm::dot(nextvel, -normal) * m/m_fDt * normal;
	// 					//std::cout << "force to apply : " << F.x << ", " << F.y << std::endl;
	// 					pm.applyTo(p, F);
	// 				}
	// 			}
	// 		}
	// 	}
	// 	else
	// 	{
	// 		for(int v = verticescount-1; v >= 0; --v)
	// 		{
	// 			glm::vec2 A = m_Polygon->getVertex(v);
	// 			size_t id2;
	// 			if(v == 0)
	// 			{
	// 				id2 = verticescount-1;
	// 			}
	// 			else
	// 			{
	// 				id2 = v-1;
	// 			}

	// 			glm::vec2 B = m_Polygon->getVertex(id2);

	// 			// For each particle
	// 			size_t particlecount = pm.getNumberParticles();
	// 			for(size_t p = 0; p < particlecount; ++p)
	// 			{
	// 				// Intersection test
	// 				glm::vec2 P1 = pm.getPosition(p);
	// 				glm::vec2 P2 = m_Solver->getNextState(p, pm, m_fDt).position;
	// 				glm::vec2 intersection, normal;
	// 				if(m_Polygon->intersect(P1, P2, A, B, &intersection, &normal))
	// 				{
	// 					// Calculate force
	// 					glm::vec2 nextvel = m_Solver->getNextState(p, pm, m_fDt).velocity;
	// 					//std::cout << "nextVel : " << nextvel.x << ", " << nextvel.y << std::endl;
	// 					float m = pm.getMass(p);
	// 					//std::cout << "mass : " << m << std::endl;
						
	// 					//std::cout << "intersect : " << intersection.x << ", " << intersection.y << std::endl;
						
	// 					glm::vec2 F = m_fElasticity * glm::dot(nextvel, -normal) * m/m_fDt * normal;
	// 					//std::cout << "force to apply : " << F.x << ", " << F.y << std::endl;
	// 					pm.applyTo(p, F);
	// 				}
	// 			}
	// 		}
	// 	}
	//}

	//Cas des collisions lors d'un jet d'eau sur l'objet
	void PolygonForce::apply(ParticleManager& pm)
	{
	// For each edge
		size_t verticescount = m_Polygon->getNumberVertices();
		
		if(m_Polygon->isInner())
		{
			std::cout << "Please make the polygon isInner value false." << std::endl;
		}
		else
		{
			for(int v = verticescount-1; v >= 0; --v)
			{
				glm::vec2 A = m_Polygon->getVertex(v);
				size_t id2;
				if(v == 0)
				{
					id2 = verticescount-1;
				}
				else
				{
					id2 = v-1;
				}

				glm::vec2 B = m_Polygon->getVertex(id2);

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

						std::cout << "intersection" << std::endl;
						// Force exercée par un jet d'eau sur un obstacle:
						// F (jet / obstacle ) = masseVolumique * debitMassique * deltaV
						// avec deltaV = vitesseFluide - vitesseReculObstacle = vitessFluide - 0 (m/s)
						// et debitMassique (m^3/s) 
						float mu = 1000; // Masse volumique de l'eau = 1000 kg/m^3
						glm::vec2 nextvel = m_Solver->getNextState(p, pm, m_fDt).velocity;
						glm::vec2 F = mu * m_fDebit * glm::dot(nextvel, normal) * normal;
						pm.applyTo(p, -F);
					}
				}
			}
		}
	}
}
