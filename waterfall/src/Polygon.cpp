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
	
	void Polygon::translate(glm::vec2 trans)
	{
		for(size_t i = 0; i < m_vertices.size(); ++i)
		{
			m_vertices.at(i) += trans;
		}

	}

	bool Polygon::intersect(const glm::vec2& P1,
						  const glm::vec2& P2,
						  const glm::vec2& A,
						  const glm::vec2& B,
						  glm::vec2* intersection,
						  glm::vec2* normal)
	{
		// TODO : Refaire le calcul de la normale
		glm::vec2 normalAB( -(B.y - A.y), (B.x - A.x));
		normalAB = glm::normalize(normalAB);
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

	bool Polygon::mouseOn(float mouseX, float mouseY)
	{
		// Iterate through each edge to detect collision
		for(size_t i = 1; i < m_vertices.size(); i++)
		{
			glm::vec2 C(mouseX, mouseY);
			glm::vec2 A = m_vertices.at(i);
			glm::vec2 B = m_vertices.at(i-1);
			glm::vec2 AC = C - A;
			glm::vec2 AB = B - A;
			glm::vec2 CA = A - C;
			glm::vec2 CB = B - C;

			float d = sqrt(AC.x*AB.y - AB.x*AC.y * AC.x*AB.y - AB.x*AC.y);

			// Check 1 : C is on the line (P1, P2)
			if(d <= 0.2)
			{
				// Check 2 : C between A and B
				if(glm::dot(CA, CB) <= 0)
				{
					select();
					return true;
				}
			}
		}
		return false;
	}

}