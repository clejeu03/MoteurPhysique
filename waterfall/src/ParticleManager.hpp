#ifndef _H_IMAC3_PARTICLEMANAGER_H_
#define _H_IMAC3_PARTICLEMANAGER_H_

#include <vector>
#include <iostream>
#include <glm/glm.hpp>
#include "renderer/ParticleRenderer2D.hpp"

namespace imac3
{

typedef std::vector<std::pair<std::pair<int, unsigned int>, std::pair<int, unsigned int>>> ParticleGraph;

struct Particle
{
    glm::vec2 pos;
    glm::vec2 speed;
    glm::vec3 color;
    glm::vec2 fbuffer;
    float mass;
    bool forceimmune;
    int nonmovableduration;
    glm::vec2 gridIndexes;

    Particle(glm::vec2 ipos, glm::vec2 ispeed, glm::vec3 icolor, glm::vec2 buffer, float imass, bool immune) :
        pos(ipos), speed(ispeed), color(icolor), fbuffer(buffer), mass(imass), forceimmune(immune), nonmovableduration(0)
    {
    }

    glm::vec2 updateGridIndexes(int numGridLines, int numGridColumns)
    {
        gridIndexes.x = floor((pos.x + 1.) / (2./(float)numGridColumns));
        gridIndexes.y = floor((pos.y + 1.) / (2./(float)numGridLines));
        if(gridIndexes.x < 0) gridIndexes.x = 0;
        if(gridIndexes.y < 0) gridIndexes.y = 0;
        // Debug
        // color = glm::vec3(((int)gridIndexes.x)%2, ((int)gridIndexes.y)%2, 0);
        return gridIndexes;
    }
};

class ParticleManager
{
  
private:

    std::vector<std::vector<Particle*>> m_particles;
    int m_numGridLines; // The number of column in the grid
    int m_numGridColumns; // The number of line in the grid

public:

    //
    // Contructor & Destructor
    //

    ParticleManager(int numGridLines, int numGridColumns);
    ~ParticleManager();

    //
    // Addind Particles
    //

    std::pair<int, unsigned int> addParticle(glm::vec2 pos, float mass, glm::vec2 speed, glm::vec3 color, bool isImmune = false);
    void addRandomParticles(unsigned int count);
    void createWaterfallParticles(unsigned int count, float width, float height, glm::vec3 color);
    
    //
    // Render particles
    //

    void drawParticles(ParticleRenderer2D& renderer);
    void drawGrid(ParticleRenderer2D& renderer);
    void drawParticleGraph(const ParticleGraph& graph, ParticleRenderer2D& renderer);

    //
    // Physics Management
    //

    void applyTo(int cell_id, size_t index, glm::vec2 force);
    void applyToAll(glm::vec2 force);

    //
    // Getters
    //

    inline int getNumGridCell() const
    {
        return m_numGridLines * m_numGridColumns;
    }

    inline int getNumberParticles() const
    {
        int nb = 0;
        for(size_t i = 0; i < m_particles.size(); i++)
        {
            nb += m_particles[i].size();
        }
        return nb;
    }

    inline int getNumberParticles(int cell_id) const
    {
        return m_particles[cell_id].size();
    }

    inline glm::vec2 getSpeed(int cell_id, size_t index) const
    {
        return m_particles[cell_id].at(index)->speed;
    }

    inline glm::vec2 getPosition(int cell_id, size_t index) const
    {
        return m_particles[cell_id].at(index)->pos;
    }

    inline glm::vec2 getForceBuffer(int cell_id, size_t index) const
    {
        return m_particles[cell_id].at(index)->fbuffer;
    }

    inline float getMass(int cell_id, size_t index) const
    {
        return m_particles[cell_id].at(index)->mass;
    }

    inline int getNonMovableDuration(int cell_id, size_t index) const
    {
        return m_particles[cell_id].at(index)->nonmovableduration;
    }

    inline glm::vec3 getColor(int cell_id, size_t index) const
    {
        return m_particles[cell_id].at(index)->color;
    }

    //
    // Setters
    //

    inline void setSpeed(int cell_id, size_t index, glm::vec2 speed)
    {
        m_particles[cell_id].at(index)->speed = speed;
    }

    inline void setPosition(int cell_id, size_t index, glm::vec2 pos)
    {
        m_particles[cell_id].at(index)->pos = pos;
    }

    inline void setColor(int cell_id, size_t index, glm::vec3 color)
    {
        m_particles[cell_id].at(index)->color = color;
    }

    inline void setForceBuffer(int cell_id, size_t index, glm::vec2 force)
    {
        m_particles[cell_id].at(index)->fbuffer = force;
    }

    inline void incrementNonMovableDuration(int cell_id, size_t index, int increment)
    {
        m_particles[cell_id].at(index)->nonmovableduration += increment;
    }

    inline void resetNonMovableDuration(int cell_id, size_t index)
    {
        m_particles[cell_id].at(index)->nonmovableduration = 0;
    }

    void updateGridIndexes(int cell_id, size_t index);

    void deleteParticle(int cell_id, size_t index);
};

ParticleGraph createCircleGraph(glm::vec2 C, float radius, glm::vec3 color, uint32_t nbSeg, ParticleManager& particleManager);


}

#endif // _H_IMAC3_PARTICLEMANAGER_H_