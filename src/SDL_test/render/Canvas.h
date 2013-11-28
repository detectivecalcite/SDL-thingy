//(c)2013 Paul Siicu


#ifndef CANVAS_H
#define CANVAS_H

#include "Top.h"


//all-static class that handles SDL - window, renderer, dimensions, and clean-up
//also image-loading functions
class Canvas
{
	friend class Sprite_base;
	friend class Sprite_text;
	friend class Stage;
	friend class Camera;
public:
	//init and quit
	static void init(const std::string& window_name, uint w, uint h);
	static void quit();

	//fps stuff
	static uint FPS;
	static const uint UPS = 30;
	static const uint INTERVAL = 1000/UPS;
	static const uint MAX_FRAMESKIP = 5;

	//window rect
	static SDL_Rect windowBox;

	//loading and cropping textures/surfaces
	//cropSurface() allocates a new texture
	static SDL_Texture* loadImage_texture(const std::string& filename);
	static SDL_Surface* loadImage_surface(const std::string& filename);
	static SDL_Surface* cropSurface(SDL_Surface* source, uint x, uint y, uint w, uint h);

	//function that returns borderRect from a surface
	static SDL_Rect get_borderRect(const SDL_Surface* srf);
	
	static SDL_Texture* get_textureFromSurface(SDL_Surface* source);

	//renderer access
	static void clear();
	static void copy(const Sprite_base& sprite);
	static void present();

	static void logError(const std::string& error_string);
private:
	static std::unique_ptr <SDL_Window, void (*)(SDL_Window*)> window;
	static std::unique_ptr <SDL_Renderer, void (*)(SDL_Renderer*)> renderer;

	static std::unique_ptr <Stage> currentStage;

	//stream for error output
	static std::ofstream errorFile;
};

#endif