#include "ParticleManager.hpp"

#include <glm/gtx/norm.hpp>
#include <glm/gtc/random.hpp>
#include <stdlib.h>

#include "Polygon.hpp"

#include <cmath>

namespace imac3
{

    

    ParticleManager::ParticleManager(int numGridLines, int numGridColumns)
    {
        m_numGridLines = numGridLines;
        m_numGridColumns = numGridColumns;
        // Create one vector for each cell
        for(size_t i = 0; i < m_numGridLines*m_numGridColumns; i++)
        {
            m_particles.push_back(std::vector<Particle*>());
        }
    }

    ParticleManager::~ParticleManager()
    {
        for(size_t i = 0; i < m_particles.size(); i++)
        {
            for(size_t j = 0; j < m_particles[i].size(); j++)
            {
                delete m_particles[i][j];
            }
        }
    }


    std::pair<int, unsigned int> ParticleManager::addParticle(glm::vec2 pos, float mass, glm::vec2 speed, glm::vec3 color, bool isImmune)
    {
        Particle* p = new Particle(pos, speed, color, glm::vec2(), mass, isImmune);
        glm::vec2 indexes = p->updateGridIndexes(m_numGridLines, m_numGridColumns);
        // Add particle in the good vector.
        int t = static_cast<int>(indexes.y) * m_numGridColumns + static_cast<int>(indexes.x);
        if(m_particles[t].size() + 1 < m_particles[t].max_size())
        {
            m_particles[t].push_back(p);
        }
        return std::make_pair(t, m_particles[t].size()-1);
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

    void ParticleManager::createWaterfallParticles(unsigned int count, float width, float height, glm::vec3 color)
    {
        for(size_t i = 0; i < count; i++)
        {
            glm::vec2 randomPos = glm::vec2(glm::linearRand(-width/2, width/2), glm::linearRand(0.8-(height/2), 0.8+(height/2) ));
            addParticle(randomPos, glm::linearRand(0.1f, 0.2f), glm::vec2(0.1, 0), color);
        }
    }


    void ParticleManager::drawParticles(ParticleRenderer2D& renderer)
    {
        int count = getNumberParticles();
        std::vector<glm::vec2> allpositions;
        std::vector<float> allmass;
        std::vector<glm::vec3> allcolors;
        for(size_t i = 0; i < m_particles.size(); i++)
        {
            for(size_t j = 0; j < m_particles[i].size(); j++)
            {
                allpositions.push_back(m_particles[i][j]->pos);
                allmass.push_back(m_particles[i][j]->mass);
                allcolors.push_back(m_particles[i][j]->color);
            }
        }
        renderer.drawParticles(allpositions.size(),
            &allpositions[0],
            &allmass[0],
            &allcolors[0]);
    }

    void ParticleManager::drawGrid(ParticleRenderer2D& renderer)
    {

    }

    void ParticleManager::drawParticleGraph(const ParticleGraph& graph, ParticleRenderer2D& renderer)
    { 
        /*renderer.drawLines(graph.size(),
            &graph[0],
            m_PositionArray.size(),
            &m_PositionArray[0],
            &m_ColorArray[0]);*/
    }


    void ParticleManager::applyTo(int cell_id, size_t index, glm::vec2 force) 
    {
        if(index > m_particles[cell_id].size()-1)
        {
            fprintf(stderr, "Error : No particle %lu in the cell %d (max index = %lu). So you can't apply force on it. End of program.\n", index, cell_id, m_particles[cell_id].size());
            exit(EXIT_FAILURE);
        }
        m_particles[cell_id].at(index)->fbuffer += force;
    }

    void ParticleManager::applyToAll(glm::vec2 force)
    {
        for(size_t i = 0; i < m_particles.size(); i++)
        {
            for(size_t j = 0; j < m_particles[i].size(); j++)
            {
                m_particles[i][j]->fbuffer += force;
            }
        }
    }


    void ParticleManager::deleteParticle(int cell_id, size_t index)
    {
        if(index > m_particles[cell_id].size()-1)
        {
            fprintf(stderr, "Error : No particle %lu in the cell %d (max index = %lu). So you can't delete it. End of program.\n", index, cell_id, m_particles[cell_id].size());
            exit(EXIT_FAILURE);
        }
        Particle* p = m_particles[cell_id].at(index);
        m_particles[cell_id].erase(m_particles[cell_id].begin() + index);
        delete p;
    }

    void ParticleManager::updateGridIndexes(int cell_id, size_t index)
    {
        if(index > m_particles[cell_id].size()-1)
        {
            fprintf(stderr, "Error : No particle %lu in the cell %d (max index = %lu). So you can't delete it. End of program.\n", index, cell_id, m_particles[cell_id].size());
            exit(EXIT_FAILURE);
        }

        glm::vec2 indexes = m_particles[cell_id].at(index)->updateGridIndexes(m_numGridLines, m_numGridColumns);
        int new_cell_id = static_cast<int>(indexes.y) * m_numGridColumns + static_cast<int>(indexes.x);
        if(new_cell_id > m_numGridColumns * m_numGridLines - 1)
        {
            deleteParticle(cell_id, index);
            return;
        }
        if(new_cell_id != cell_id)
        {
            Particle* p = m_particles[cell_id].at(index);
            m_particles[cell_id].erase(m_particles[cell_id].begin() + index);
            m_particles[new_cell_id].push_back(p);
        }
    }

    ParticleGraph createCircleGraph(glm::vec2 C, float radius, glm::vec3 color, uint32_t nbSeg, ParticleManager& pm)
    {
        if(nbSeg < 3)
        {
            fprintf(stderr, "createCircleGraph : Not enough segments (must be > 3). End of program.\n");
            exit(EXIT_FAILURE);
        }

        ParticleGraph graph;
        float angularStep = 2*PI/nbSeg;

        // Add the first particle
        glm::vec2 pos(C.x + radius * cos(PI/2.f), C.y + radius * sin(PI/2.f));
        const std::pair<int, unsigned int> startingParticle = pm.addParticle(pos, 1.f, glm::vec2(0.0, 0.0), color);

        // Add the others
        std::pair<int, unsigned int> currentParticle;
        std::pair<int, unsigned int> lastParticle = startingParticle;
        
        for (int i = 1; i < nbSeg; ++i)
        {
            pos.x = C.x + radius * cos(PI/2.f + i*angularStep);
            pos.y = C.y + radius * sin(PI/2.f + i*angularStep);
            currentParticle = pm.addParticle(pos, 1.f, glm::vec2(0.0, 0.0), color);
            
            // Link the current particle with the previous
            graph.push_back(std::make_pair(currentParticle, lastParticle));
            
            // Link the current particle with the starting particle if we reach the end of the graph
            if (i == nbSeg-1)
            {
                graph.push_back(std::make_pair(currentParticle, startingParticle));
            }
        }
        return graph;
    }

}
