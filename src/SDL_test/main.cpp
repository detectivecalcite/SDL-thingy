#include "render\Sprite.h"
#include "render\Canvas.h"
using namespace std;

int main(int argc, char *argv[])
{
	try
	{
		Canvas::init("SDL", 1280, 1024);

	}
	catch (runtime_error err)
	{
		Canvas::logError(err.what());
		Canvas::quit();
		exit(EXIT_FAILURE);
	}

	Canvas::quit();
}