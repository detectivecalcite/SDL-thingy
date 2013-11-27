//camera implementation file


#include "Camera.h"
using namespace std;


Camera::Camera(uint pos_x, uint pos_y, bool dynamicBorderWidth) : w(Canvas::windowBox.w), h(Canvas::windowBox.h), x(pos_x), y(pos_y), dynamicBorderWidth(dynamicBorderWidth)
{ 
	vel = { 0, 0 };
}