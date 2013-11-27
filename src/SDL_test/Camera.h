//(c)2013 Paul Siicu


#ifndef CAMERA_H
#define CAMERA_H

#include "Top.h"


class Camera
{
public:
	Camera(uint pos_x, uint pos_y, bool dynamicBorderWidth);

protected:
	uint w, h;
	uint x, y;
	physics::vector vel;

	//width of rendering border around camera
	//making this dynamic means it resizes with biggest nearby object
	uint borderWidth;
	bool dynamicBorderWidth;
};

#endif