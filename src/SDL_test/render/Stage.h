//(c)2013 Paul Siicu


#ifndef STAGE_H
#define STAGE_H

#include "Top.h"


class Stage
{
public:
	//add other assets (sounds, etc.)

	//sort sprite vector at Stage construction by z member
	Stage(const std::vector<Sprite_base*>& sprites);

	//vectors of assets
	std::vector<std::unique_ptr<Sprite_base>> sprites;

	void loop();

protected:
	void draw(double interpolation);

	virtual void updateEnvironment() = 0;
	void updateAssets();
};

#endif