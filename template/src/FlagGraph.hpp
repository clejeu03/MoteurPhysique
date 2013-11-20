#ifndef _H_IMAC3_FLAGGRAPH_H_
#define _H_IMAC3_FLAGGRAPH_H_

#include <vector>
#include <iostream>
#include <glm/glm.hpp>
#include "ParticleManager.hpp"

namespace imac3
{

class FlagGraph
{
    
private:
	ParticleGraph gridGraph;
    ParticleGraph diagonalGraph;
    ParticleGraph shearGraph;
public:
    inline const ParticleGraph& getGridGraph() const{return gridGraph;}
    inline const ParticleGraph& getDiagonalGraph() const{return diagonalGraph;}
    inline const ParticleGraph& getShearGraph() const{return shearGraph;}
    inline void setGridGraph(ParticleGraph grid){gridGraph = grid;}
    inline void setDiagonalGraph(ParticleGraph diagonal){diagonalGraph = diagonal;}
    inline void setShearGraph(ParticleGraph shear){shearGraph = shear;}
    

};

FlagGraph createFlag(glm::vec2 P, float width, float height, uint32_t discLat, uint32_t discHeight, ParticleManager& particleManager);


}

#endif // _H_IMAC3_FLAGGRAPH_H_ 
