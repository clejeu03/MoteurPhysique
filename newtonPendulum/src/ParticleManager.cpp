#include "ParticleManager.hpp"

#include <glm/gtx/norm.hpp>
#include <glm/gtc/random.hpp>
#include <stdlib.h>

#include "Polygon.hpp"

#include <cmath>

namespace imac3
{

  ParticleGraph createStringGraph(glm::vec2 A, glm::vec2 B, glm::vec3 color, ParticleManager& particleManager)
  {

    ParticleGraph graph;

    size_t indexA = particleManager.addParticle(A, 1.f, glm::vec2(0.0, 0.0), color, true);
    size_t indexB = particleManager.addParticle(B, 1.f, glm::vec2(0.0, 0.0), color);

    //Link the particules into a graph
    graph.push_back(std::make_pair(indexA, indexB));

    return graph;
  }

  ParticleGraph createCircleGraph(glm::vec2 C, float radius, glm::vec3 color, uint32_t nbSeg, ParticleManager& particleManager)
  {
    ParticleGraph graph;
    const size_t circleInitialIndex = particleManager.getNumberParticles();
    if(nbSeg < 3)
    {
      std::cerr << "createCircleGraph : Not enough segments (must be > 3). End of program." << std::cerr;
      exit(EXIT_FAILURE);
    }
    size_t addedParticule;
    for (int i = 0; i < nbSeg; ++i)
    {
      glm::vec2 P(C.x + radius * cos(i*2*PI/nbSeg), C.y + radius * sin(i*2*PI/nbSeg));
      addedParticule = particleManager.addParticle(P, 1.f, glm::vec2(0.0, 0.0), color);
      //Link the particules into a graph
      if (addedParticule != circleInitialIndex)
      {
        graph.push_back(std::make_pair(addedParticule-1, addedParticule));
      }
      if (addedParticule == circleInitialIndex + nbSeg-1)
      {
        graph.push_back(std::make_pair(addedParticule, addedParticule-nbSeg+1));
      }
    }
    return graph;
  }



  size_t ParticleManager::addParticle(glm::vec2 pos, float mass, glm::vec2 speed, glm::vec3 color, bool isImmune)
  {
    m_PositionArray.push_back(pos);
    m_MassArray.push_back(mass);
    m_SpeedArray.push_back(speed);
    m_ColorArray.push_back(color);
    m_ForceBuffer.push_back(glm::vec2(0, 0));
    m_ForceImmune.push_back(isImmune);
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
  if(!m_ForceImmune[index])
  {
    m_ForceBuffer[index] = m_ForceBuffer[index] + force;
  }
}

void ParticleManager::drawParticleGraph(const ParticleGraph& graph, ParticleRenderer2D& renderer)
{ 
  renderer.drawLines(graph.size(),
   &graph[0],
   m_PositionArray.size(),
   &m_PositionArray[0],
   &m_ColorArray[0]);
}

}
