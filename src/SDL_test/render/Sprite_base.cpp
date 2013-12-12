#include "Sprite.h"
using namespace std;


//texture unique_ptr assigned to nullptr with SDL_DestroyTexture() deleter
//asigned in constructor body
Sprite_base::Sprite_base(const string& filename,
						 const vector<SDL_Rect>& clips,
						 int pos_x,
						 int pos_y,
						 bool solid,
						 ushort z) : filename(filename),
									 clips(clips),
									 currentClip(this->clips.cbegin()),
									 numClips(this->clips.size()),
									 texture(unique_ptr<SDL_Texture, void(*)(SDL_Texture*)>(nullptr, SDL_DestroyTexture)),
									 surface(unique_ptr<SDL_Surface, void(*)(SDL_Surface*)>(nullptr, SDL_FreeSurface)),
									 solid(solid),
									 z(z)
{
	dest.x = pos_x;
	dest.y = pos_y;
	dest.w = currentClip->w;
	dest.h = currentClip->h;

	texture.reset(Canvas::loadImage_texture(filename));
	surface.reset(Canvas::loadImage_surface(filename));

	//initalize borderRects
	//iterate through clip rectangles
	for (auto itElem = clips.cbegin(); itElem != clips.cend(); ++itElem)
	{
		//get surface under clip rect
		//temp surface created
		SDL_Surface* brSurface = Canvas::cropSurface(surface.get(), itElem->x, itElem->y, itElem->w, itElem->h);

		//get borderRect and add it to the Sprite's
		SDL_Rect br = Canvas::get_borderRect(brSurface);
		borderRects.insert(pair<vector<SDL_Rect>::const_iterator, SDL_Rect>(itElem, br));

		//free temporary surface
		SDL_FreeSurface(brSurface);

		//assign burrentBorderRect the first iteration through
		if (itElem == clips.cbegin())
			currentBorderRect = borderRects.cbegin();
	}

	//init translatedBorderRect here
	translatedBorderRect.x = currentBorderRect->second.x + dest.x;
	translatedBorderRect.y = currentBorderRect->second.y + dest.y;
	translatedBorderRect.w = currentBorderRect->second.w;
	translatedBorderRect.h = currentBorderRect->second.h;
	
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

	//only one border rect / clip rect
	borderRects.insert(pair<vector<SDL_Rect>::const_iterator, SDL_Rect>(clips.cbegin(), Canvas::get_borderRect(surface.get())));
	currentBorderRect = borderRects.cbegin();

	//init translatedBorderRect
	translatedBorderRect.x = currentBorderRect->second.x + dest.x;
	translatedBorderRect.y = currentBorderRect->second.y + dest.y;
	translatedBorderRect.w = currentBorderRect->second.w;
	translatedBorderRect.h = currentBorderRect->second.h;
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
		if ((itElem->x >= translatedBorderRect.x) && (itElem->x <= translatedBorderRect.x + translatedBorderRect.w) && (itElem->y >= translatedBorderRect.y) && (itElem->y >= translatedBorderRect.y) && (itElem->y <= translatedBorderRect.y + translatedBorderRect.h))
			return true;
		//if tr point fits into rect
		if ((itElem->x + itElem->w >= translatedBorderRect.x) && (itElem->x + itElem->w <= translatedBorderRect.x + translatedBorderRect.w) && (itElem->y >= translatedBorderRect.y) && (itElem->y < translatedBorderRect.y + translatedBorderRect.h))
			return true;
		//if bl point fits into rect
		if ((itElem->x >= translatedBorderRect.x) && (itElem->x <= translatedBorderRect.x + translatedBorderRect.w) && (itElem->y + itElem->h >= translatedBorderRect.y) && (itElem->y + itElem->h <= translatedBorderRect.y + translatedBorderRect.h))
			return true;
		//if br point fits into rect
		if ((itElem->x + itElem->w >= translatedBorderRect.x) && (itElem->x + itElem->w <= translatedBorderRect.x + translatedBorderRect.w) && (itElem->y + itElem->h >= translatedBorderRect.y) && (itElem->y + itElem->h <= translatedBorderRect.y + translatedBorderRect.h))
			return true;
		else
			return false;
	}
}