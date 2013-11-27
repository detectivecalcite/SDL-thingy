//(c)2013 Paul Siicu


#ifndef STAGE_H
#define STAGE_H

#include "Top.h"


class Stage
{
public:
	//add other assets (sounds, fonts, etc.)

	//sort sprite vector at Stage construction by z member
	Stage(const std::vector<Sprite_base*>& sprites);

	//vectors of specific assets
	std::vector<Sprite_base*> sprites;

	void loop();

protected:
	void draw();
	void update();
};

#endif