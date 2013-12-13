#include "Sprite.h"
using namespace std;


Sprite_base::Sprite_base(const string& filename,
						 const vector<SDL_Rect>& clips,
						 int pos_x,
						 int pos_y,
						 const map<uint, SDL_Rect>& bounds,
						 bool solid,
						 ushort z) : filename(filename),
									 clips(clips),
									 clipCurrent(this->clips.cbegin()),
									 numClips(this->clips.size()),
									 texture(unique_ptr<SDL_Texture, void(*)(SDL_Texture*)>(nullptr, SDL_DestroyTexture)),	//texture & surface initialized to nullptr, 
									 surface(unique_ptr<SDL_Surface, void(*)(SDL_Surface*)>(nullptr, SDL_FreeSurface)),		//then actually assigned in body
									 solid(solid),
									 z(z)
{
	dest.x = pos_x;
	dest.y = pos_y;
	dest.w = clipCurrent->w;
	dest.h = clipCurrent->h;

	texture.reset(Canvas::loadImage_texture(filename));
	if (texture.get() == nullptr)
		throw runtime_error("Texture load error");
	surface.reset(Canvas::loadImage_surface(filename));
	if (surface.get() == nullptr)
		throw runtime_error("Surface load error");

	//init bounds
	//if the user just wants the border rect
	if (bounds == std::map<uint, SDL_Rect>())
	{
		for (auto it : clips)
		{
			SDL_Surface* cropped = Canvas::cropSurface(surface.get(), it.x, it.y, it.w, it.h);

		}
	}
	//if the user did provide a bounds map
	else
	{
		this->bounds = bounds;
	}

	//translate initial bounds
	
}

Sprite_base::Sprite_base(const string& filename,
						 uint w,
						 uint h,
						 int pos_x,
						 int pos_y,
						 bool solid,
						 ushort z) : filename(filename),
									 numClips(1),
									 texture(unique_ptr<SDL_Texture, void(*)(SDL_Texture*)>(nullptr, SDL_DestroyTexture)),
									 surface(unique_ptr<SDL_Surface, void(*)(SDL_Surface*)>(nullptr, SDL_FreeSurface)),
									 solid(solid),
									 z(z)
{
	SDL_Rect clip;
	clip.x = 0;
	clip.y = 0;
	clip.w = w;
	clip.h = h;
	clips.push_back(clip);
	currentClip = clips.cbegin();

	dest.x = pos_x;
	dest.y = pos_y;
	dest.w = currentClip->w;
	dest.h = currentClip->h;

	texture.reset(Canvas::loadImage_texture(filename));
	surface.reset(Canvas::loadImage_surface(filename));

	//init bounds

	//translate bounds here
}

//interpolation not used in basic draw function
void Sprite_base::draw(double interpolation)
{
	Canvas::copy(*this);
}

//basic collision
//no rotation, etc.
//use borderRect!
//optimize/thread pls
bool Sprite_base::checkCollide(const vector<SDL_Rect>& collisionRects)
{
	for (auto itElem = collisionRects.cbegin(); itElem != collisionRects.cend(); itElem++)
	{
		//if tl point fits into rect
		if ((itElem->x >= currentBorderRect_translated.x) && (itElem->x <= currentBorderRect_translated.x + currentBorderRect_translated.w) && (itElem->y >= currentBorderRect_translated.y) && (itElem->y >= currentBorderRect_translated.y) && (itElem->y <= currentBorderRect_translated.y + currentBorderRect_translated.h))
			return true;
		//if tr point fits into rect
		if ((itElem->x + itElem->w >= currentBorderRect_translated.x) && (itElem->x + itElem->w <= currentBorderRect_translated.x + currentBorderRect_translated.w) && (itElem->y >= currentBorderRect_translated.y) && (itElem->y < currentBorderRect_translated.y + currentBorderRect_translated.h))
			return true;
		//if bl point fits into rect
		if ((itElem->x >= currentBorderRect_translated.x) && (itElem->x <= currentBorderRect_translated.x + currentBorderRect_translated.w) && (itElem->y + itElem->h >= currentBorderRect_translated.y) && (itElem->y + itElem->h <= currentBorderRect_translated.y + currentBorderRect_translated.h))
			return true;
		//if br point fits into rect
		if ((itElem->x + itElem->w >= currentBorderRect_translated.x) && (itElem->x + itElem->w <= currentBorderRect_translated.x + currentBorderRect_translated.w) && (itElem->y + itElem->h >= currentBorderRect_translated.y) && (itElem->y + itElem->h <= currentBorderRect_translated.y + currentBorderRect_translated.h))
			return true;
		else
			return false;
	}
}