//(c)2013 Paul Siicu


#ifndef SPRITE_H
#define SPRITE_H

#include "Top.h"


namespace physics
{
	struct vector
	{
		double x;
		double y;

/*		vector& operator+(const vector& vr)
		{

		}*/
	};
}

struct Quad
{
	Quad(int tl, int br) : tl(tl), br(br) {}

	int tl, br;
};

//abstract base sprite class
//contains texture ptr, vector of clips (source rects), and destination rects - all ready for rendering
class Sprite_base
{
	friend class Canvas;
public:

	//sheet-utilizing sprite constructor
	Sprite_base(const std::string& filename, 
				const std::vector<SDL_Rect>& clips, 
				int pos_x, 
				int pos_y, 
				bool solid = true, 
				ushort z = 0);

	//plain sprite constructor
	Sprite_base(const std::string& filename, 
				uint w, 
				uint h, 
				int pos_x, 
				int pos_y, 
				bool solid = true, 
				ushort z = 0);

	virtual ~Sprite_base()
	{
		//unique_ptr destroys texture automatically, ya goof
	}

	//basic collision function uses borderRect
	//override in derived types that use more complicated collision detection
	//accepts vector of rects
	virtual bool checkCollide(const std::vector<SDL_Rect>& collisionRects);

	//basic draw function
	//override when rotation/interpolation are needed
	virtual void draw(double interpolation);

	virtual void update(uint deltaTicks) = 0;

	std::vector<SDL_Rect>::const_iterator currentClip;

	std::map<std::vector<SDL_Rect>::const_iterator, SDL_Rect>::const_iterator currentBorderRect;

	SDL_Rect translatedBorderRect;

	SDL_Rect dest;

protected:
	//default constructor
	//empty
	Sprite_base() {}

	std::unique_ptr <SDL_Texture, void (*)(SDL_Texture*)> texture;
	std::unique_ptr <SDL_Surface, void (*)(SDL_Surface*)> surface;

	std::vector<SDL_Rect> clips;

	//borderRects for each spritesheet clip
	std::map<std::vector<SDL_Rect>::const_iterator, SDL_Rect> borderRects;

	//depth variable; bigger goes on top
	ushort z;

	bool solid;

	std::string filename;
	uint numClips;
};

//class for sprites with bounding rectangles
class Sprite_bounds : virtual public Sprite_base
{
public:
	//map of bounding rectangles
	//k: clip, v: bounds
	typedef std::map<std::vector<SDL_Rect>::const_iterator, SDL_Rect> boundRect_map;

	Sprite_bounds(const std::string& filename, const std::vector<SDL_Rect>& clips, int pos_x, int pos_y, const boundRect_map& bounds, bool solid = true, ushort z = 0) : Sprite_base(filename, clips, pos_x, pos_y, solid, z), bounds(bounds), currentBounds(this->bounds.cbegin())
	{
		//ok
	}
	Sprite_bounds(const std::string& filename, uint w, uint h, int pos_x, int pos_y, const boundRect_map& bounds, bool solid = true, ushort z = 0) : Sprite_base(filename, w, h, pos_x, pos_y, solid, z), bounds(bounds), currentBounds(this->bounds.cbegin())
	{
		//ok
	}
	
	boundRect_map::const_iterator currentBounds;

	virtual bool checkCollide(const std::vector<SDL_Rect>& collisionRects);

protected:
	boundRect_map bounds;
};

//class with velocity physics::vector
class Sprite_move : virtual public Sprite_base
{
public:
	Sprite_move(const std::string& filename, const std::vector<SDL_Rect>& clips, int pos_x, int pos_y, bool solid = true, ushort z = 0) : Sprite_base(filename, clips, pos_x, pos_y, solid, z) 
	{
		vel.x = 0;
		vel.y = 0;
		acc.x = 0;
		acc.y = 0;
	}

	Sprite_move(const std::string& filename, uint w, uint h, int pos_x, int pos_y, bool solid = true, ushort z = 0) : Sprite_base(filename, w, h, pos_x, pos_y, solid, z) 
	{
		vel.x = 0;
		vel.y = 0;
		acc.x = 0;
		acc.y = 0;
	}

	virtual void draw(double interpolation)
	{
		//implement me sometime pls
	}

	virtual void update(uint deltaTicks)
	{
		vel.x += acc.x;
		vel.y += acc.y;

		dest.x += vel.x;
		dest.y += vel.y;
	}

protected:
	physics::vector vel;
	physics::vector acc;
};

//sprite class w/ scaling and rotation
class Sprite_rot : virtual public Sprite_base
{
	friend class Canvas;
public:
	Sprite_scale(const std::string& filename,
				 const std::vector<SDL_Rect>& clips,
				 int pos_x,
				 int pos_y,
				 const SDL_Point& rotPoint,
				 double rotAngle,
				 SDL_RendererFlip flip,
				 bool solid = true,
				 ushort z = 0);

	Sprite_scale(const std::string& filename,
				 uint w,
				 uint h,
				 int pos_x,
				 int pos_y,
				 const SDL_Point& rotPoint,
				 double rotAngle,
				 SDL_RendererFlip flip,
				 bool solid = true,
				 ushort z = 0);

	virtual void draw(double interpolation);

	virtual bool checkCollide(const std::vector<SDL_Rect>& collisionRects);
protected:
	//rotation point
	//relative to unscaled sprite
	SDL_Point rotPoint;

	//clockwise angle
	double rotAngle;

	SDL_RendererFlip flip;

	//
};

//text sprite
//ready for use
class Sprite_text : public Sprite_base
{
public:
	//w and h are not mandatory!
	Sprite_text(const std::string& filename,
				const std::string& text, 
				ushort pt, 
				SDL_Color color, 
				int pos_x, 
				int pos_y, 
				ushort z, 
				uint w = OPTIONAL_ARG, 
				uint h = OPTIONAL_ARG);

	virtual void update()
	{
		//empty
		//implementations for text that scrolls or pops in letter by letter
	}

protected:
	std::unique_ptr<TTF_Font, void(*)(TTF_Font*)> font;
	ushort pt;

	std::string text;
};

//finish
class Sprite_final : public Sprite_bounds, public Sprite_move, public Sprite_rot
{
	Sprite_final(const std::string& filename,
				 const std::vector<SDL_Rect>& clips,
				 int pos_x,
				 int pos_y,

	Sprite_final();

	virtual bool checkCollide(const std::vector<SDL_Rect>& collisionRects);
	virtual void draw(double interpolation);
};

#endif