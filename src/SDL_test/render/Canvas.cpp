//canvas implementation file


#include "Canvas.h"
using namespace std;


//definitions
unique_ptr<SDL_Window, void (*)(SDL_Window*)> Canvas::window = std::unique_ptr<SDL_Window, void (*)(SDL_Window*)>(nullptr, SDL_DestroyWindow);
unique_ptr<SDL_Renderer, void (*)(SDL_Renderer*)> Canvas::renderer = std::unique_ptr<SDL_Renderer, void (*)(SDL_Renderer*)>(nullptr, SDL_DestroyRenderer);
SDL_Rect Canvas::windowBox;
ofstream Canvas::errorFile;

uint Canvas::FPS = 0;
const uint Canvas::UPS = 30;
const uint Canvas::INTERVAL = 1000 / UPS;
const uint Canvas::MAX_FRAMESKIP = 5;

void Canvas::init(const std::string& window_name, uint w, uint h)
{
	//open this first so exception error msgs can get recorded right away
	errorFile.open("error.log", fstream::out);
	
	if (SDL_Init(SDL_INIT_EVERYTHING) == -1)
		throw runtime_error("SDL init failed");
	if (TTF_Init() == -1)
		throw runtime_error("TTF init failed");
	if (IMG_Init(IMG_INIT_PNG) == -1)
		throw runtime_error("PNG init failed");

	windowBox.x = SDL_WINDOWPOS_CENTERED;
	windowBox.y = SDL_WINDOWPOS_CENTERED;
	windowBox.w = w;
	windowBox.h = h;

	window.reset(SDL_CreateWindow(window_name.c_str(), windowBox.x, windowBox.y, windowBox.w, windowBox.h, SDL_WINDOW_FULLSCREEN_DESKTOP));
	if (window.get() == nullptr)
		throw runtime_error("Window init failed");

	renderer.reset(SDL_CreateRenderer(window.get(), -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC));
	if (renderer.get() == nullptr)
	{
		throw runtime_error("Renderer init failed");
	}
}

SDL_Texture* Canvas::loadImage_texture(const std::string& filename)
{
	SDL_Texture* texture = nullptr;
	texture = IMG_LoadTexture(Canvas::renderer.get(), filename.c_str());
	if (texture == nullptr)
		throw runtime_error("Texture load error");

	return texture;
}

SDL_Surface* Canvas::loadImage_surface(const std::string& filename)
{
	SDL_Surface* surface = nullptr;
	surface = IMG_Load(filename.c_str());
	if (surface == nullptr)
		throw runtime_error("Surface load error");

	return surface;
}

SDL_Surface* Canvas::cropSurface(SDL_Surface* source, uint x, uint y, uint w, uint h, bool freeSource)
{
	//make and blit onto new texture
	SDL_Surface* dest = SDL_CreateRGBSurface(source->flags, w, h, source->format->BitsPerPixel, source->format->Rmask, source->format->Gmask, source->format->Bmask, source->format->Amask);
	SDL_Rect rect = {x, y, w, h};
	SDL_BlitSurface(source, &rect, dest, NULL);

	if (freeSource)
		SDL_FreeSurface(source);

	return dest;
}

void Canvas::logError(const string& error_string)
{
	errorFile << error_string << endl;
}

void Canvas::quit()
{
	errorFile.clear();
	errorFile.close();

	SDL_DestroyRenderer(renderer.release());
	SDL_DestroyWindow(window.release());

	SDL_Quit();
	TTF_Quit();
}

void Canvas::copy(const Sprite_base& sprite)
{
	SDL_Rect src = *sprite.currentClip;
	SDL_Rect dst = sprite.dest;

	SDL_RenderCopy(renderer.get(), sprite.texture.get(), &src, &dst);
}

void Canvas::copyEx(const Sprite_rot& sprite)
{
	SDL_Rect src = *sprite.currentClip;
	SDL_Rect dst = sprite.dest;

	SDL_RenderCopyEx(renderer.get(), sprite.texture.get(), &src, &dst, sprite.rotAngle, &sprite.rotPoint, sprite.flip);
}

void Canvas::clear()
{
	SDL_RenderClear(renderer.get());
}

void Canvas::present()
{
	SDL_RenderPresent(renderer.get());
}

SDL_Rect Canvas::get_borderRect(const SDL_Surface* surface)
{
	//pixel ptr
	const Uint32* pixel = (Uint32*)surface->pixels;

	//border rect
	//starts out covering entire surface sent
	SDL_Rect border;
	border.x = 0;
	border.y = 0;
	border.w = surface->w;
	border.h = surface->h;
	//variables for vertical and horizontal pixel parsing
	bool top = true, left = true;


	//let's iterate through the image

	//vertical pixel parse
	//start at pixel 0 and run down picture until you hit w*h
	//find top and bottom of borderRect

	//for finding bottom edge of border rect
	//to find the first blank row, so the bottom edge of the rect can be set to the row directly above
	bool colorPixel = true;
	for (const Uint32* row_it = pixel; row_it != pixel + surface->w*surface->h; row_it += surface->w)
	{
		//last row iterated through was blank
		//assign bottom edge of border rect
		if (!colorPixel)
		{
			int bottomEdge = ((row_it - pixel) / surface->w) - 2;
			border.h = bottomEdge - border.y + 1;
		}

		for (const Uint32* el_it = row_it; el_it != row_it + surface->w; ++el_it)
		{
			//for finding the top of the border rectangle...
			if (top)
			{
				if (*el_it != 0xFFFFFF)
				{
					border.y = (row_it - pixel) / surface->w;
					top = false;
					break;
				}
			}
			//for finding the bottom
			else
			{
				//if a non-blank pixel is hit before row end
				if (*el_it != 0xFFFFFF)
					colorPixel = true;
				//if the last pixel in a row is reached and no color pixels were/are encountered
				else if (el_it == row_it + surface->w - 1 && *el_it == 0xFFFFFF)
					colorPixel = false;
			}
		}
	}

	//horizontal pixel parse
	//parse through columns
	colorPixel = true;
	for (const Uint32* col_it = pixel; col_it != pixel + surface->w; ++col_it)
	{
		//last column iterated through was blank
		//assign right edge of border rect
		if (!colorPixel)
		{
			int rightEdge = (col_it - pixel) - 2;
			border.w = rightEdge - border.x + 1;
		}
		for (const Uint32* el_it = col_it; el_it != col_it + surface->h; el_it += surface->w)
		{
			//left side
			if (left)
			{
				if (*el_it == 0xFFFFFF)
				{
					border.x = col_it - pixel;
					left = false;
					break;
				}
			}
			//right side
			else
			{
				//if a non-blank pixel is hit before column end
				if (*el_it != 0xFFFFFF)
					colorPixel = true;
				//if the last pixel in a column is reached and no color pixels were/are encountered
				else if (el_it == col_it + surface->h - 1 && *el_it == 0xFFFFFF)
					colorPixel = false;
			}
		}
	}

	return border;
}