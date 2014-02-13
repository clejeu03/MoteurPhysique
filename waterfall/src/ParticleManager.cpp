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


    size_t ParticleManager::addParticle(glm::vec2 pos, float mass, glm::vec2 speed, glm::vec3 color, bool isImmune)
    {
        Particle* p = new Particle(pos, speed, color, glm::vec2(), mass, isImmune);
        glm::vec2 indexes = p->updateGridIndexes(m_numGridLines, m_numGridColumns);
        // Add particle in the good vector.
        //fprintf(stderr, "(%f, %f)\n", indexes.x, indexes.y);
        int t = static_cast<int>(indexes.y) * m_numGridColumns + static_cast<int>(indexes.x);
        if(m_particles[t].size() + 1 < m_particles[t].max_size())
        {
            m_particles[t].push_back(p);
        }
        return id;
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
        for(size_t i = 0; i < count; i++)
        {
            glm::vec2 randomPos = glm::vec2(glm::linearRand(-width/2, width/2), glm::linearRand(0.8-(height/2), 0.8+(height/2) ));
            glm::vec3 randomColor = glm::vec3(0, glm::linearRand(0.2f, 0.5f), 1);
            addParticle(randomPos, glm::linearRand(0.1f, 0.2f), glm::vec2(0.1, 0), randomColor);
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


    inline int ParticleManager::getNumberParticles() const
    {
        int nb = 0;
        for(size_t i = 0; i < m_particles.size(); i++)
        {
            nb += m_particles[i].size();
        }
        return nb;
    }

    void ParticleManager::deleteParticle(size_t id)
    {
        for(size_t i = 0; i < m_particles.size(); i++)
        {
            for(size_t j = 0; j < m_particles[i].size(); j++)
            {
                if(m_particles[i][j]->id == id)
                {
                    Particle* p = m_particles[i][j];
                    m_particles[i].erase(m_particles[i].begin() + j);
                    delete p;
                    return;
                }
            }
        }
        fprintf(stderr, "Error : You are trying to delete article with id %lu which does not exist. End of program.\n", id);
        exit(EXIT_FAILURE);
    }

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

}
