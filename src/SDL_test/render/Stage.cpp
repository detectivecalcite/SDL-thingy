//stage implementation file


#include "Stage.h"
using namespace std;


Stage::Stage(const vector<Sprite_base*>& sprites) : sprites(sprites)
{
	//something prolly oughta happen here
}

void Stage::update()
{
	static ticks;
	for (auto iSprite : sprites)
	{
		iSprite->update(SDL_GetTicks() - ticks);
	}

	ticks = SDL_GetTicks();
}

void Stage::draw(float interpolation)
{
	for (auto iSprite : sprites)
	{
		iSprite->draw(interpolation);
	}
}

void Stage::loop()
{
	uint nextTick = SDL_GetTicks();
	uint loops;
	float interpolation;

	bool game = true;
	while (game)
	{
		loops = 0;

		while (SDL_GetTicks() > nextTick && loops < Canvas::MAX_FRAMESKIP)
		{
			update();

			nextTick += Canvas::INTERVAL;
			loops++;
		}

		interpolation = float(SDL_GetTicks() + Canvas::INTERVAL - nextTick) / float(Canvas::INTERVAL);
		draw(interpolation);
	}
}