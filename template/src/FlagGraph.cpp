#include "FlagGraph.hpp"
#include "ParticleManager.hpp"

namespace imac3
{

	FlagGraph createFlag(glm::vec2 P, float width, float height, , uint32_t discLat, uint32_t discHeight, ParticleManager& particleManager)
	{

		FlagGraph flag;

		/*for(uint32_t j = 0; j < discHeight; ++j)
		{
			for(uint32_t i = 0; i < discLat; ++i)
			{
				glm::vec2 particle((float)i*width/(float)discLat, (float)j*height/(float)discHeight);
				uint32_t index = particleManager.addParticle(particle, 1.f, glm::vec2(0.0, 0.0), glm::vec3(1.f, 1.f, 1.f));

				flag.getGridFlag().push_back(std::make_pair(index-1, index));

			}
		}*/

	}

}
