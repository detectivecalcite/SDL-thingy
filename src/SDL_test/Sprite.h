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

		} */
	};
}

//abstract base sprite class
//contains texture ptr, vector of clips (source rects), and destination rects - all ready for rendering
class Sprite_base
{
	friend class Canvas;
public:
	enum Collision
	{
		TOP, 
		RIGHT, 
		BOTTOM,
		LEFT
	};

	//sheet-utilizing sprite constructor
	Sprite_base(const std::string& filename, const std::vector<SDL_Rect>& clips, int pos_x, int pos_y, bool solid = true, ushort z = 0);

	//plain sprite constructor
	Sprite_base(const std::string& filename, uint w, uint h, int pos_x, int pos_y, bool solid = true, ushort z = 0);

	virtual ~Sprite_base()
	{
		//unique_ptr destroys texture automatically, ya goof
	}

	//basic collision function uses boundRect
	//override in derived types that use more complicated collision detection
	//accepts vector of rects
	virtual Collision checkCollide(const std::vector<SDL_Rect>& collisionRects);

	virtual void update(uint deltaTicks) = 0;

	//basic draw function
	//override if rotation is needed
	virtual void draw(double interpolation);

	std::vector<SDL_Rect>::const_iterator currentClip;
	SDL_Rect dest;

protected:
	//default constructor
	Sprite_base();

	//texture
	std::unique_ptr<SDL_Texture, void (*)(SDL_Texture*)> texture;

	std::vector<SDL_Rect> clips;

	//borderRect and iterator
	std::map<std::vector<SDL_Rect>::const_iterator, SDL_Rect> borderRects;
	std::map<std::vector<SDL_Rect>::const_iterator, SDL_Rect>::const_iterator currentBorderRect;

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

	Sprite_bounds(const std::string& filename, const std::vector<SDL_Rect>& clips, int pos_x, int pos_y, const boundRect_map& bounds, bool solid = true, ushort z = 0) : bounds(bounds), Sprite_base(filename, clips, pos_x, pos_y, solid, z) 
	{
		//init currentBounds!
	}
	Sprite_bounds(const std::string& filename, uint w, uint h, int pos_x, int pos_y, const boundRect_map& bounds, bool solid = true, ushort z = 0) : bounds(bounds), Sprite_base(filename, w, h, pos_x, pos_y, solid, z) 
	{
		//init currentBounds!
	}
	
	boundRect_map bounds;
	boundRect_map::const_iterator currentBounds;

	virtual Collision checkCollide(const std::vector<SDL_Rect>& collisionRects);
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

protected:
	physics::vector vel;
	physics::vector acc;
};

//sprite class w/ scaling and rotation
class Sprite_scale : virtual public Sprite_base
{
public:
	Sprite_scale(const std::string& filename, uint w, uint h, int pos_x, int pos_y, double scaleFactor, const SDL_Point& rotPoint, double rotAngle, bool solid = true, ushort z = 0): Sprite_base(filename, w, h, pos_x, pos_y, solid, z), scaleFactor(scaleFactor), rotPoint(rotPoint), rotAngle(rotAngle) {}
	Sprite_scale(const std::string& filename, const std::vector<SDL_Rect>& clips, int pos_x, int pos_y, double scaleFactor, const SDL_Point& rotPoint, double rotAngle, bool solid = true, ushort z = 0): Sprite_base(filename, clips, pos_x, pos_y, solid, z), scaleFactor(scaleFactor), rotPoint(rotPoint), rotAngle(rotAngle) {}
protected:
	//scale variable
	double scaleFactor;

	//rotation point
	SDL_Point rotPoint;

	//clockwise angle
	double rotAngle;
};

//text sprite
//ready for use
class Sprite_text : public Sprite_base
{
public:
	//w and h are not mandatory!
	//write constructor
	Sprite_text(const std::string& filename, ushort pt, const std::string& text, SDL_Color color, int pos_x, int pos_y, ushort z, uint w = OPTIONAL_ARG, uint h = OPTIONAL_ARG);

	virtual void update(uint deltaTicks);

protected:
	TTF_Font* font;
	ushort pt;

	std::string text;
};

#endif