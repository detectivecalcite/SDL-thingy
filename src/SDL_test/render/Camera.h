//(c)2013 Paul Siicu


#ifndef CAMERA_H
#define CAMERA_H

#include "Top.h"


class Camera
{
	friend class Stage;
public:
	Camera(uint pos_x, uint pos_y, uint w, uint h, bool dynamicBorderWidth);

	void update();

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