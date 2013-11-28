//sprite implementation file


#include "Sprite.h"
using namespace std;


//texture unique_ptr assigned to nullptr with SDL_DestroyTexture() deleter
//asigned in constructor body
Sprite_base::Sprite_base(const string& filename, const vector<SDL_Rect>& clips, int pos_x, int pos_y, bool solid, ushort z) : filename(filename), clips(clips), currentClip(this->clips.cbegin()), numClips(clips.size()), texture(unique_ptr<SDL_Texture, void(*)(SDL_Texture*)>(nullptr, SDL_DestroyTexture)), solid(solid), z(z)
{
	dest.x = pos_x;
	dest.y = pos_y;
	dest.w = currentClip->w;
	dest.h = currentClip->h;

	texture.reset(Canvas::loadImage_texture(filename));
}

Sprite_base::Sprite_base(const string& filename, uint w, uint h, int pos_x, int pos_y, bool solid, ushort z) : filename(filename), numClips(1), texture(unique_ptr<SDL_Texture, void(*)(SDL_Texture*)>(nullptr, SDL_DestroyTexture)), solid(solid), z(z)
{
	dest.x = pos_x;
	dest.y = pos_y;
	dest.w = currentClip->w;
	dest.h = currentClip->h;

	SDL_Rect clip;
	clip.x = 0;
	clip.y = 0;
	clip.w = w;
	clip.h = h;
	clips.push_back(clip);
	currentClip = clips.cbegin();

	texture.reset(Canvas::loadImage_texture(filename));
}

void Sprite_base::draw(float interpolation)
{
	Canvas::copy(*this);
}

Sprite_base::Collision Sprite_base::checkCollide(const std::vector<SDL_Rect>& collisionRects)
{
	//basic collision
	//no rotation, etc.
}

//Base-class parameters need to be initialized in constructor body (not in initalizer list)
//Sprite_base default constructor gets called, leave the rest to Sprite_text's constructor
Sprite_text::Sprite_text(const string& filename, ushort pt, const string& text, SDL_Color color, int pos_x, int pos_y, ushort z, uint w = OPTIONAL_ARG, uint h = OPTIONAL_ARG) : Sprite_base(), text(text), pt(pt)
{
	this->filename = filename;
	
	font = TTF_OpenFont(filename.c_str(), pt);

	//don't forget to destroy this surface!!
	SDL_Surface* textSurface = TTF_RenderText_Solid(font, this->text.c_str(), color);
	texture.reset(SDL_CreateTextureFromSurface(Canvas::renderer.get(), textSurface));
	SDL_FreeSurface(textSurface);

	//make clip
	SDL_Rect clip;
	clip.x = 0;
	clip.y = 0;
	SDL_QueryTexture(texture.get(), NULL, NULL, &clip.w, &clip.h);
	//if w are given explicit values...
	if (w != OPTIONAL_ARG)
		clip.w = w;
	if (h != OPTIONAL_ARG)
		clip.h = h;
	clips.push_back(clip);
	currentClip = clips.cbegin();

	//make dest
	dest.x = pos_x;
	dest.y = pos_y;
	dest.w = clip.w;
	dest.h = clip.h;
}