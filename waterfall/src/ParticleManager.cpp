#include "ParticleManager.hpp"

#include <glm/gtx/norm.hpp>
#include <glm/gtc/random.hpp>
#include <stdlib.h>

#include "Polygon.hpp"

#include <cmath>

namespace imac3
{

  ParticleGraph createStringGraph(glm::vec2 A, size_t stringIndex, glm::vec3 color, ParticleManager& particleManager)
  {

    ParticleGraph graph;

    size_t indexA = particleManager.addParticle(A, 1.f, glm::vec2(0.0, 0.0), color, true);
    //Link the particules into a graph
    graph.push_back(std::make_pair(indexA, stringIndex));

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
    float angularStep = 2*PI/nbSeg;
    for (int i = 0; i < nbSeg; ++i)
    {
      glm::vec2 P(C.x + radius * cos(PI/2.f + i*angularStep), C.y + radius * sin(PI/2.f + i*angularStep));
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
    glm::vec2 randomPos = glm::vec2(glm::linearRand(-0.8f, 0.8f), glm::linearRand(-30.f, 3.f));
    glm::vec3 randomColor = glm::vec3(randomPos.x, randomPos.y, randomPos.x*randomPos.y);
    addParticle(randomPos, glm::linearRand(0.5f, 1.5f), glm::vec2(0, 0), glm::vec3(1, 1, 1));
  }	
}

void ParticleManager::createWaterfallParticles(unsigned int count, float width, float height)
{
  if(m_PositionArray.size() + count < m_PositionArray.max_size())
  {
    for(size_t i = 0; i < count; i++)
    {
      glm::vec2 randomPos = glm::vec2(glm::linearRand(-width/2, width/2), glm::linearRand(0.8-(height/2), 0.8+(height/2) ));
      glm::vec3 randomColor = glm::vec3(0, glm::linearRand(0.2f, 0.5f), 1);
      addParticle(randomPos, glm::linearRand(0.1f, 0.2f), glm::vec2(0, 0), randomColor);
    }
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

void ParticleManager::deleteParticle(size_t index)
{
  m_PositionArray.erase(m_PositionArray.begin() + index);
  m_MassArray.erase(m_MassArray.begin() + index);
  m_SpeedArray.erase(m_SpeedArray.begin() + index);
  m_ColorArray.erase(m_ColorArray.begin() + index);
  m_ForceBuffer.erase(m_ForceBuffer.begin() + index);
  m_ForceImmune.erase(m_ForceImmune.begin() + index);
}

}
