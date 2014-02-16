#ifndef _H_IMAC3_POLYGON_H_
#define _H_IMAC3_POLYGON_H_

#include <vector>
#include <glm/glm.hpp>
#include <glm/gtx/norm.hpp>
#include "renderer/ParticleRenderer2D.hpp"

namespace imac3
{

	extern double PI;

class Polygon
{
private:

	std::vector<glm::vec2> m_vertices;
	glm::vec3 m_color;
	bool m_bisinner;
	bool m_isSelected;
	float m_radius;
	float m_discFactor;
	glm::vec2 m_center;
	
	Polygon(glm::vec3 color, bool isInner = false);
	void addVertex(glm::vec2 position);
	
public:

	Polygon(){}
	static Polygon buildCircle(glm::vec3 color, glm::vec2 center, float radius, size_t discFactor, bool isInner = false);
	void draw(ParticleRenderer2D& renderer, float lineWidth = 1.f) const;
	
	static bool intersect(const glm::vec2& P1, const glm::vec2& P2, const glm::vec2& A, const glm::vec2& B, glm::vec2* intersection, glm::vec2* normal);
						  
	size_t getNumberVertices() const;
	glm::vec2 getVertex(size_t index) const;
	glm::vec3 getColor() const { return m_color; }
	glm::vec2 getCenter() const { return m_center; }

	void translate(glm::vec2 trans);

	bool isInner() const;
	bool isSelected() { return m_isSelected; }

	bool mouseOn(float mouseX, float mouseY);
	void select()
	{
		m_isSelected = true;
		m_color = glm::vec3(1., 1., 0);
	}
	void unselect()
	{
		m_isSelected = false;
		m_color = glm::vec3(1., 0, 0);
	}
	
};

}

#endif // _H_IMAC3_POLYGON_H_
