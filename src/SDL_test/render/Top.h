#define uint	unsigned int
#define ulong	unsigned long
#define ushort	unsigned short

#define VERSION_ENGINE	0.1.1
#define VERSION_GAME	0.1.1

#ifdef __WIN32
//
#endif

#ifdef __linux__
//
#endif

#ifdef __APPLE__
//
#endif

#define OPTIONAL_ARG	0
#define UNDEF_IMAGE		""


#include "SDL.h"
#include "SDL_image.h"
#include "SDL_ttf.h"

#include "Canvas.h"
#include "Sprite.h"
#include "Stage.h"
#include "Camera.h"

#include <cstdlib>
#include <ctime>

#include <vector>
#include <map>
#include <string>

#include <iostream>
#include <fstream>

#include <memory>
#include <stdexcept>