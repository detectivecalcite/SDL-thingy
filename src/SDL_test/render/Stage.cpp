//stage implementation file


#include "Stage.h"
using namespace std;


Stage::Stage(const vector<Sprite_base*>& sprites)
{
	//sort and assign Sprite_base*'s
	for (vector<Sprite_base*>::const_iterator itElem = sprites.cbegin(); itElem != sprites.cend(); ++itElem)
	{
		this->sprites.push_back(unique_ptr<Sprite_base>(*itElem));
	}
}

void Stage::updateAssets()
{
	//deltaTicks should be pretty much 0 on first execution
	static uint loopTicks = SDL_GetTicks();

	for (std::vector<unique_ptr<Sprite_base>>::const_iterator itElem = sprites.cbegin(); itElem != sprites.cend(); ++itElem)
		itElem->get()->update(SDL_GetTicks() - loopTicks);

	//record ticks after update() execution so that deltaTicks can get calculated next run-through
	loopTicks = SDL_GetTicks();
}

void Stage::draw(double interpolation)
{
	for (std::vector<unique_ptr<Sprite_base>>::const_iterator itElem = sprites.cbegin(); itElem != sprites.cend(); ++itElem)
		itElem->get()->draw(interpolation);
}

void Stage::loop()
{
	//
}