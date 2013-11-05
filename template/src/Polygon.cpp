#include "Polygon.hpp"
#include <math.h> 
#include <iostream>
namespace imac3
{

	double PI = 3.1415926535897;

	Polygon::Polygon(glm::vec3 color, bool isInner)
		: m_color(color),
		  m_bisinner(isInner)
	{  
	}
	
	void Polygon::addVertex(glm::vec2 position)
	{
		m_vertices.push_back(position);
	}

	Polygon Polygon::buildBox(glm::vec3 color, glm::vec2 position, float width, float height, bool isInner)
	{
		Polygon poly(color, isInner);
		poly.addVertex(position);
		poly.addVertex(glm::vec2(position.x, position.y + height));
		poly.addVertex(glm::vec2(position.x + width, position.y + height));
		poly.addVertex(glm::vec2(position.x + width, position.y));
		return poly;
	}
	
	Polygon Polygon::buildCircle(glm::vec3 color, glm::vec2 center, float radius, size_t discFactor, bool isInner)
	{
		Polygon poly(color, isInner);
		for(size_t i = 0; i < discFactor; ++i)
		{
			poly.addVertex(center + glm::vec2(radius*cos(i*2*PI/discFactor), radius*sin(i*2*PI/discFactor)));
		}
		return poly;
	}
	
	void Polygon::draw(ParticleRenderer2D& renderer, float lineWidth) const
	{
		renderer.drawPolygon(m_vertices.size(), &m_vertices[0], m_color, lineWidth);
	}
	
	bool Polygon::intersect(const glm::vec2& P1,
						  const glm::vec2& P2,
						  const glm::vec2& A,
						  const glm::vec2& B,
						  glm::vec2* intersection,
						  glm::vec2* normal)
	{
		// TODO : Refaire le calcul de la normale
		glm::vec2 normalAB( -B.y - A.y, B.x - A.x);
		
		glm::vec2 AP1 = P1 - A;
		glm::vec2 P1P2 = P2 - P1;
		float k = glm::dot(-AP1, normalAB) / glm::dot(P1P2, normalAB);
		
		// Verification 1 : k € [0;1]
		if(k>=0 && k<=1)
		{
			glm::vec2 M = P1 + k*P1P2;
			glm::vec2 AM = M - A;
			glm::vec2 AB = B - A;
			// Vérification 2 : 0 <= AM scalaire Ab <= (normeAB)²
			if(0 <= glm::dot(AM, AB) && glm::dot(AM, AB) <= glm::l2Norm(glm::vec3(AB, 0)) * glm::l2Norm(glm::vec3(AB, 0)))
			{
				*intersection = M;
				*normal = normalAB;
				return true;
			}
		}				
		return false;
	}
	
	size_t Polygon::getNumberVertices() const
	{
		return m_vertices.size();
	}
	
	glm::vec2 Polygon::getVertex(size_t index) const
	{
		return m_vertices[index];
	}
	
	bool Polygon::isInner() const
	{
		return m_bisinner;
	}
	

}

