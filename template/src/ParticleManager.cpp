#include "ParticleManager.hpp"

#include <glm/glm.hpp>
#include <glm/gtx/norm.hpp>
#include <glm/gtc/random.hpp>

namespace imac3
{

  ParticleGraph createString(glm::vec2 A, glm::vec2 B, glm::vec3 color, uint32_t discFactor, ParticleManager& particleManager)
  {
    ParticleGraph graph;
    float lenghtAB = glm::l2Norm(glm::vec3(A, 0), glm::vec3(B, 0));
    float dist = lenghtAB / (discFactor + 1 );

    //Create the particules on the segment AB
    for (int i = 0; i<= discFactor+1; ++i){
      float x = (A.x + B.x) / (i*dist);
      float y = (A.y + B.y) / (i*dist);
      size_t addedParticule = particleManager.addParticle(glm::vec2(x, y), 1.f, glm::vec2(0.0, 0.0), color);
      
      //Link the particules into a graph
      if (addedParticule != 0){
        graph.push_back(std::make_pair(addedParticule-1, addedParticule));
      }

    }
    return graph;
  }

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
    		addParticle(randomPos, glm::linearRand(0.5f, 1.5f), glm::vec2(0, 0), glm::vec3(1, 1, 1));
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
    
    void ParticleManager::drawParticleGraph(ParticleGraph& graph, ParticleRenderer2D& renderer)
    { 
        renderer.drawLines(graph.size(),
                           &graph[0],
                           m_PositionArray.size(),
                           &m_PositionArray[0],
                           &m_ColorArray[0]);
    }

}
