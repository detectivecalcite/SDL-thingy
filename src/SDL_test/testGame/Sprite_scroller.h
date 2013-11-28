//(c)2013 Paul Siicu


#ifndef SPRITE_SCROLLER_H
#define SPRITE_SCROLLER_H

#include "Sprite.h"
#include "Canvas.h"


namespace scroller
{
	//side-scroller sprite class
	class Sprite_player : public Sprite_move, public Sprite_bounds
	{
	public:
		//event flags
		//to be sent back to update()
		enum Event
		{
			KEY_W,
			KEY_A,
			KEY_S,
			KEY_D,
			KEY_SPACE
		};

		//flags for describing sprite state
		//STill
		//MoVing
		//JumPing
		enum State
		{
			ST_RIGHT,
			ST_LEFT,
			MV_RIGHT,
			MV_LEFT,
			JP_RIGHT,
			JP_LEFT
		};

		Sprite_player(const std::string& filename, const std::vector<SDL_Rect>& clips, int pos_x, int pos_y, const Sprite_bounds::boundRect_map& bounds) : Sprite_base(filename, clips, pos_x, pos_y), Sprite_bounds(filename, clips, pos_x, pos_y, bounds), Sprite_move(filename, clips, pos_x, pos_y) {}
		Sprite_player(const std::string& filename, int w, int h, int pos_x, int pos_y, const Sprite_bounds::boundRect_map& bounds) : Sprite_base(filename, w, h, pos_x, pos_y), Sprite_bounds(filename, w, h, pos_x, pos_y, bounds), Sprite_move(filename, w, h, pos_x, pos_y) {}

		virtual void update(Event e);
	};
}

#endif