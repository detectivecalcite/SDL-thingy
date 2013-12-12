//camera implementation file


#include "Camera.h"
using namespace std;


Camera::Camera(uint pos_x, uint pos_y, uint w, uint h, bool dynamicBorderWidth) : w(w), h(h), x(pos_x), y(pos_y), dynamicBorderWidth(dynamicBorderWidth)
{ 
	vel = { 0, 0 };
}