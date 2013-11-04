#include "ParticleManager.hpp"

#include <glm/gtc/random.hpp>

namespace imac3
{

	size_t ParticleManager::addParticle(glm::vec2 pos, float mass, glm::vec2 speed, glm::vec3 color)
	{
		m_PositionArray.push_back(pos);
		m_MassArray.push_back(mass);
		m_SpeedArray.push_back(speed);
		m_ColorArray.push_back(color);
		m_ForceBuffer.push_back(glm::vec2(0, 0));
		return m_PositionArray.size()-1;
	}
	
	void ParticleManager::addRandomParticles(unsigned int count)
	{
    	for(size_t i = 0; i < count; ++i)
    	{
    		glm::vec2 randomPos = glm::vec2(glm::linearRand(-0.8f, 0.8f), glm::linearRand(-0.8f, 0.8f));
    		glm::vec3 randomColor = glm::vec3(randomPos.x, randomPos.y, randomPos.x*randomPos.y);
    		addParticle(randomPos, 1.f, glm::vec2(0, 0), randomColor);
    	}	
    }
    
    void ParticleManager::drawParticles(ParticleRenderer2D& renderer)
    {
        renderer.drawParticles(m_PositionArray.size(),
                               &m_PositionArray[0],
                               &m_MassArray[0],
                               &m_ColorArray[0]);
    }
    
    void ParticleManager::applyTo(size_t index, glm::vec2 force) 
    {
    	m_ForceBuffer[index] = m_ForceBuffer[index] + force;
    }
    


}
