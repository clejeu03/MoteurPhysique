#ifndef _H_IMAC3_PARTICLEMANAGER_H_
#define _H_IMAC3_PARTICLEMANAGER_H_

#include <vector>
#include <iostream>
#include <glm/glm.hpp>
#include "renderer/ParticleRenderer2D.hpp"

namespace imac3
{

typedef std::vector<std::pair<unsigned int, unsigned int>> ParticleGraph;

class ParticleManager
{
    
private:
	std::vector<glm::vec2> m_PositionArray;
    std::vector<float> m_MassArray;
    std::vector<glm::vec2> m_SpeedArray;
    std::vector<glm::vec3> m_ColorArray;
    std::vector<glm::vec2> m_ForceBuffer;
public:
    size_t addParticle(glm::vec2 pos, float mass, glm::vec2 speed, glm::vec3 color);
    void addRandomParticles(unsigned int count);
    void drawParticles(ParticleRenderer2D& renderer); 
    void applyTo(size_t index, glm::vec2 force);
    inline size_t getNumberParticles() const { return m_PositionArray.size(); }
    void showForce(size_t index) {
    	std::cout << m_ForceBuffer[index].x << ", " << m_ForceBuffer[index].y << std::endl;
    }
    void drawParticleGraph(ParticleGraph& graph, ParticleRenderer2D& renderer);
    
    inline glm::vec2 getSpeed(size_t index) const { return m_SpeedArray[index]; }
    inline glm::vec2 getPosition(size_t index) const { return m_PositionArray[index]; }
    inline glm::vec2 getForceBuffer(size_t index) const { return m_ForceBuffer[index]; }
    inline float getMass(size_t index) const { return m_MassArray[index]; }
    
    inline void setSpeed(size_t index, glm::vec2 speed) { m_SpeedArray[index] = speed; }
    inline void setPosition(size_t index, glm::vec2 pos) { m_PositionArray[index] = pos; }
    inline void setForceBuffer(size_t index, glm::vec2 force) { m_ForceBuffer[index] = force; }
};

}

#endif // _H_IMAC3_PARTICLEMANAGER_H_ 
