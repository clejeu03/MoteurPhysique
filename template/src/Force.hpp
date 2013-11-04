#ifndef _H_IMAC3_FORCE_H_
#define _H_IMAC3_FORCE_H_

#include "ParticleManager.hpp"

namespace imac3 
{

class Force
{
private :

public:
	virtual ~Force(){};
	virtual void apply(ParticleManager& pm) = 0;

};

}

#endif // _H_IMAC3_FORCE_H_
