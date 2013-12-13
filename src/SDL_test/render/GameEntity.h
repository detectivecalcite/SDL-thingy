//(c)2013 Paul Siicu


#ifndef GAME_ENTITY_H
#define GAME_ENTITY_H

#include "Top.h"


class GameEntity
{
public:
	void update(uint deltaTicks);
	void draw(double interpolation);

protected:
	//sprite
};

#endif