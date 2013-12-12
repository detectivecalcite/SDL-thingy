#include "Sprite.h"
using namespace std;


Sprite_rot(const std::string& filename,
			 const std::vector<SDL_Rect>& clips,
			 int pos_x,
			 int pos_y,
			 const SDL_Point& rotPoint,
			 double rotAngle,
			 SDL_RendererFlip flip,
			 bool solid,
			 ushort z) : Sprite_base(filename, clips, pos_x, pos_y, solid, z),
						 rotPoint(rotPoint),
						 rotAngle(rotAngle),
						 flip(flip) 
{
	//rotate and stretch everything pls
}


Sprite_rot(const std::string& filename,
			 uint w,
			 uint h,
			 int pos_x,
			 int pos_y,
			 const SDL_Point& rotPoint,
			 double rotAngle,
			 SDL_RendererFlip flip,
			 bool solid,
			 ushort z) : Sprite_base(filename, w, h, pos_x, pos_y, solid, z),
						 rotPoint(rotPoint),
						 rotAngle(rotAngle),
						 flip(flip) 
{
	//rotate and stretch everything pls
}

void Sprite_rot::draw(double interpolation)
{
	Canvas::copyEx(*this);
}

bool Sprite_rot::checkCollide(const vector<SDL_Rect>& collisionRects)
{

}