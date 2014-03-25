#include "PolygonForce.hpp"

namespace imac3
{

	void PolygonForce::setDt(const float& dt)
	{
		m_fDt = dt;
	}
	
	// Cas d'une collision normale entre des particules et un objet
	void PolygonForce::apply(ParticleManager& pm)
	{
		// For each edge
		size_t verticescount = m_Polygon->getNumberVertices();
		
		if(m_Polygon->isInner())
		{
			for(size_t v = 0; v < verticescount; ++v)
			{
				glm::vec2 A = m_Polygon->getVertex(v);
				glm::vec2 B = m_Polygon->getVertex((v+1)%(verticescount));
				
				// For each cell
				for(size_t c = 0; c < pm.getNumGridCell(); ++c)
				{
					// For each particle in this cell
					for(size_t p = 0; p < pm.getNumberParticles(c); ++p)
					{
						// Intersection test
						glm::vec2 P1 = pm.getPosition(c, p);
						glm::vec2 P2 = m_Solver->getNextState(c, p, pm, m_fDt).position;
						glm::vec2 intersection, normal;
						if(m_Polygon->intersect(P1, P2, A, B, &intersection, &normal))
						{
							// Calculate force
							glm::vec2 nextvel = m_Solver->getNextState(c, p, pm, m_fDt).velocity;
							float m = pm.getMass(c, p);							
							glm::vec2 F = m_fElasticity * glm::dot(nextvel, -normal) * m/m_fDt * normal;
							pm.applyTo(c, p, F);
							glm::vec3 new_color;
							new_color.r = (m_Polygon->getColor().r + pm.getColor(c, p).r)/2;
							new_color.g = (m_Polygon->getColor().g + pm.getColor(c, p).g)/2;
							new_color.b = (m_Polygon->getColor().b + pm.getColor(c, p).b)/2;
							pm.setColor(c, p, m_Polygon->getColor());
						}
					}
				}
			}
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

				// For each cell
				for(size_t c = 0; c < pm.getNumGridCell(); ++c)
				{
					// For each particle
					for(size_t p = 0; p < pm.getNumberParticles(c); ++p)
					{
						// Intersection test
						glm::vec2 P1 = pm.getPosition(c, p);
						glm::vec2 P2 = m_Solver->getNextState(c, p, pm, m_fDt).position;
						glm::vec2 intersection, normal;
						if(m_Polygon->intersect(P1, P2, A, B, &intersection, &normal))
						{
							// Calculate force
							glm::vec2 nextvel = m_Solver->getNextState(c, p, pm, m_fDt).velocity;
							float m = pm.getMass(c, p);							
							glm::vec2 F = m_fElasticity * glm::dot(nextvel, -normal) * m/m_fDt * normal;
							pm.applyTo(c, p, F);
							glm::vec3 new_color = glm::normalize(m_Polygon->getRealColor() + pm.getColor(c, p));
							pm.setColor(c, p, new_color);
						}
					}
				}
			}
		}
	}

	//Cas des collisions lors d'un jet d'eau sur l'objet
	void PolygonForce::applyRealPhysicFormula(ParticleManager& pm)
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



				// For each cell
				for(size_t c = 0; c < pm.getNumGridCell(); ++c)
				{
					// For each particle
					for(size_t p = 0; p < pm.getNumberParticles(c); ++p)
					{
						// Intersection test
						glm::vec2 P1 = pm.getPosition(c, p);
						glm::vec2 P2 = m_Solver->getNextState(c, p, pm, m_fDt).position;
						glm::vec2 intersection, normal;
						if(m_Polygon->intersect(P1, P2, A, B, &intersection, &normal))
						{
							// Force exercée par un jet d'eau sur un obstacle:
						// F (jet / obstacle ) = masseVolumique * debitMassique * deltaV
						// avec deltaV = vitesseFluide - vitesseReculObstacle = vitessFluide - 0 (m/s)
						// et debitMassique (m^3/s) 

						//Calcul du debit dans le cas d'une configuration dite déversoir
						// Q = nu * largeurChute * hauteurEauAmont * sqrt( 2 * g)
						// avec nu = (2/3) * (0.602) dans notre cas
						// et g la constante gravitationnelle
						float nu = (2.f/3.f)* 0.602;
						float Q = nu * m_fWidth * m_fHeight * glm::sqrt(2*9.81) * 1000; // x1000 si hypothèse mesures en cm 
						float mu = 1000; // Masse volumique de l'eau = 1000 kg/m^3
						glm::vec2 F = mu * Q * 1000 * glm::dot(pm.getSpeed(c, p), normal) * normal;
						pm.applyTo(c, p, -F);
						}
					}
				}
			}
		}
	}
}
