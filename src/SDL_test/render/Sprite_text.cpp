#include "Sprite.h"
using namespace std;



//Base-class parameters need to be initialized in constructor body (not in initalizer list)
//Sprite_base default constructor gets called, the rest is left to Sprite_text's constructor
Sprite_text::Sprite_text(const string& filename,
						 const string& text, 
						 ushort pt, 
						 SDL_Color color, 
						 int pos_x, 
						 int pos_y, 
						 ushort z, 
						 uint w = OPTIONAL_ARG, 
						 uint h = OPTIONAL_ARG) : Sprite_base(), 
												  text(text), 
												  pt(pt), 
												  solid(false),
												  font(std::unique_ptr<TTF_Font, void(*)(TTF_Font*)>(nullptr, TTF_CloseFont))
{
	this->filename = filename;
	
	font.reset(TTF_OpenFont(filename.c_str(), pt));

	//font rendered to temp surface and transformed to texture
	//don't forget to destroy temp surface!!
	SDL_Surface* textSurface = TTF_RenderText_Solid(font.get(), this->text.c_str(), color);
	texture.reset(SDL_CreateTextureFromSurface(Canvas::renderer.get(), textSurface));
	SDL_FreeSurface(textSurface);

	//make clip
	SDL_Rect clip;
	clip.x = 0;
	clip.y = 0;
	SDL_QueryTexture(texture.get(), NULL, NULL, &clip.w, &clip.h);
	//if w and h are given explicit values...
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