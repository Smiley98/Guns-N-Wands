#pragma once

#if _DEBUG
	#pragma comment(lib, "sfml-window-s-d.lib")
	#pragma comment(lib, "sfml-graphics-s-d.lib")
	#pragma comment(lib, "sfml-system-s-d.lib")
	#pragma comment(lib, "sfml-audio-s-d.lib")
#else
	#pragma comment(lib, "sfml-window-s.lib")
	#pragma comment(lib, "sfml-graphics-s.lib")
	#pragma comment(lib, "sfml-system-s.lib")
	#pragma comment(lib, "sfml-audio-s.lib")
#endif

//window
#pragma comment(lib, "opengl32.lib")
#pragma comment(lib, "winmm.lib")
#pragma comment(lib, "gdi32.lib")

//graphics
#pragma comment(lib, "freetype.lib")
#pragma comment(lib, "jpeg.lib")
#pragma comment(lib, "glew32s.lib")

//audio
#pragma comment(lib, "openal32.lib")
#pragma comment(lib, "flac.lib")
#pragma comment(lib, "vorbisenc.lib")
#pragma comment(lib, "vorbisfile.lib")
#pragma comment(lib, "vorbis.lib")
#pragma comment(lib, "ogg.lib")

//physics and collisions!
#pragma comment(lib, "Box2D.lib")

//Xinput!
#pragma comment(lib, "Xinput9_1_0.lib")

//fmod
#pragma comment(lib, "fmod_vc.lib")

//library headers
#define SFML_STATIC
#include <SFML\Window.hpp>
#include <SFML\Graphics.hpp>
#include <SFML\Audio.hpp>
#include <Box2D\Box2D.h>
#include <fmod\fmod.hpp>

//gl wrangler
#define GLEW_STATIC
#include <GL\glew.h>

//headers
#include "shader.h"
#include "sprite.h"
#include "matrix4x4.h"
#include "vector3.h"
#include "text.h"
#include "debug.h"
#include "window.h"
#include "shapes.h"

typedef sf::Keyboard Input;