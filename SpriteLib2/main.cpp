#include "controller.h"
#include "KeyEvents.h"
#include "RayEngine.h"
#include "SoundSystem.h"
#include "Line.h"
#include "cooldown.h"
#include <fstream>
#include <streambuf>
#include <iostream>
#include "Button.h"
#include "Destroyer.h"
#include "Recon.h"
#include "Medic.h"
#include "Weaver.h"
#define DEVELOPER_TOOLS TRUE

using namespace spritelib;
using namespace std;

void IterativeSelectionSort(std::vector<Sprite*>& a_sprites);
void KeyboardFunc(Window::Key a_key, Window::EventType a_eventType);
void MouseFunc(Window::Button a_button, int a_mouseX, int a_mouseY, Window::EventType a_eventType);
void DrawSprites(std::vector<Sprite*> sprites);
void addSprite(SpriteMap&, std::string);
void setSpriteAnimation(SpriteMap& map, std::string name, std::string animation_id, int start_x, int start_y, int x_spacing, int y_spacing, int num_frames);

enum class GameStates {
	Tutorial,
	Pause,
	StartMenu,
	MageMenu,
	MilitianMenu,
	MapSelect,
	MatchStart,
	GameOver
};

//global game states and maps because I am bad a programming
GameStates gameState = GameStates::StartMenu;
GameStates stateBeforePause;
std::string selectedMap = "MatchMap3";

enum Points {
	Top,
	Middle,
	Bottom,
	Left,
	Right
};

void setMap1() {
	selectedMap = "MatchMap1";
	gameState = GameStates::MatchStart;
}
void setMap2() {
	selectedMap = "MatchMap2";
	gameState = GameStates::MatchStart;
}
void setMap3() {
	selectedMap = "MatchMap3";
	gameState = GameStates::MatchStart;
}

void startTutorial() {
	gameState = GameStates::Tutorial;
	selectedMap = "TutorialMap";
}

void startMatch() {
	gameState = GameStates::MageMenu;
}
void resume() { gameState = stateBeforePause; }

void pause() {
	stateBeforePause = gameState;
	gameState = GameStates::Pause;
}

void quitGame() {
	Window::get_game_window().get_sfml_window().close();
	std::cout << "Game exited successfully.\nPress any key followed by ENTER to terminate program.\n";
	std::cin.get();
}

int main()
{
	//set up window, font, and seed random generator
	srand(std::time(0));
	Window& theGame = Window::get_game_window();	//singleton thus 1 window max!

	//Figure out resolution and do some math to store values corresponding to points on screen
	std::vector<sf::VideoMode> modes = sf::VideoMode::getFullscreenModes();

	float halfWidth = modes[0].width * 0.5f;
	float halfHeight = modes[0].height * 0.5f;
	float quarterWidth = modes[0].width * 0.25f;
	float quarterHeight = modes[0].height * 0.25f;

	Point centrePoints[5] = {
		Point(halfWidth, modes[0].height - quarterHeight),	//top middle
		Point(halfWidth, halfHeight),						//middle
		Point(halfWidth, quarterHeight),					//bottom middle
		Point(quarterWidth, halfHeight),					//middle left
		Point(modes[0].width - quarterWidth, halfHeight)	//middle right
	};

	theGame.init("Gun's N' Wands", modes[0].width, modes[0].height, true)
		.set_keyboard_callback(KeyboardFunc)
		.set_mouse_callback(MouseFunc)
		.set_clear_color(255, 255, 255);

	HWND win = Window::get_game_window().get_sfml_window().getSystemHandle();
	ShowWindow(win, SW_MAXIMIZE);
	Text::load_font("assets/times.ttf", "TimesNewRoman");

	//initialize shaders
	Shader* s_vert = new Shader();
	Shader* s_geom = new Shader();
	Shader* s_frag = new Shader();
	ShaderProgram* s_sp = new ShaderProgram();
	s_vert->load_shader(Shader::VERTEX, "assets/BlankVert.glsl");					//vertex shader
	s_vert->compile_shader();
	s_geom->load_shader(Shader::GEOMETRY, "assets/Texture Line Shader.glsl");		//custom shader!
	s_geom->compile_shader();
	s_frag->load_shader(Shader::FRAGMENT, "assets/PassthroughTextureFrag.glsl");	//colour shader
	s_frag->compile_shader();
	s_sp->add_shader(*s_vert);
	s_sp->add_shader(*s_geom);
	s_sp->add_shader(*s_frag);
	s_sp->link_program();

	//load images:
	Sprite skybox1;
	skybox1.load_sprite_image("skybox3").set_position(0, halfHeight).set_scale(modes[0].width * 2, halfHeight);
	Sprite skybox2;
	skybox2.load_sprite_image("skybox3").set_position(0, halfHeight).set_scale(modes[0].width * 2, halfHeight);

	Sprite wand;
	wand.load_sprite_image("Characters/wand_sprite").set_sprite_frame_size(92, 107).push_frame("wand", 0, 0).set_position(halfWidth, 0).set_scale(650, 650).set_animation("wand");

	Sprite gun;
	gun.load_sprite_image("Characters/gun_sprite").set_sprite_frame_size(92, 107).push_frame("gun", 0, 0).set_position(halfWidth, 0).set_scale(650, 650).set_animation("gun");

	//Player player(Point{ 800,800 }, centrePoints[Points::Left], &skybox1, false);
	//Player player2(Point{ 800,1600 }, centrePoints[Points::Right], &skybox2, true);
	std::vector<cooldown> ScoutCooldowns = {
		cooldown(500)
	};

	Destroyer destroyer;
	Destroyer destroyer2;

	Player player(Teams::Mages, Point{ 525, 25 }, 1.57f, centrePoints[Points::Top], &skybox1, &destroyer, ScoutCooldowns, false);
	Player player2(Teams::Militians, Point{ 1575, 1975 }, 4.71f, centrePoints[Points::Bottom], &skybox2, &destroyer2, ScoutCooldowns, true);

	//1. declare map
	SpriteMap Sprites;

	//2. declar data to be put in map (either a positionless texture or a sprite)
	std::vector<std::string> textureNames{
		"wall1",
		"wall2",
		"wall3",
		"floor1",
		"floor2",
		"cannonball",

		"Spells/WaterOrb",
		"Spells/FireOrb",
		"Spells/LightningOrb",
		"Spells/IceOrb",
		"Spells/BloodOrb",

		"Buttons/VersusButt",
		"Buttons/TutorialButt",
		"Buttons/ExitButt",

		"Buttons/Map1Butt",
		"Buttons/Map2Butt",
		"Buttons/Map3Butt",

		"Characters/weaverIdle",
		"Characters/reconIdle"
	};
	decInfo decorationData{
		{ "Characters/A_Scout_Sprites", &player },	//is this magic!? Since we've passed in the address of player, does that mean when player's position gets updated in the
		{ "Characters/W_Scout_Sprites", &player2 }	//gameloop this reference to it also updates as well!!!????
	};

	//3. add the data to the map
	for (int i = 0; i < textureNames.size(); i++) {
		addSprite(Sprites, textureNames[i]);
	}

	for (auto itr = decorationData.begin(); itr != decorationData.end(); itr++) {
		addSprite(Sprites, itr->first);
	}

	Sprites["Characters/W_Scout_Sprites"].set_sprite_frame_size(32, 70).push_frame("run_f", 232, 264 - 98).push_frame("run_f", 200, 264 - 98)
		.push_frame("run_f", 232, 264 - 98).push_frame("run_f", 264, 264 - 98).push_frame("run_f", 296, 264 - 98).push_frame("run_f", 328, 264 - 98)
		.push_frame("run_f", 296, 264 - 98).push_frame("run_f", 264, 264 - 98);

	Sprites["Characters/W_Scout_Sprites"].set_sprite_frame_size(32, 70).push_frame("run_b", 466, 264 - 98).push_frame("run_b", 498, 264 - 98)
		.push_frame("run_b", 466, 264 - 98).push_frame("run_b", 434, 264 - 98).push_frame("run_b", 402, 264 - 98).push_frame("run_b", 370, 264 - 98)
		.push_frame("run_b", 402, 264 - 98).push_frame("run_b", 434, 264 - 98);

	Sprites["Characters/A_Scout_Sprites"].set_sprite_frame_size(32, 70).push_frame("run_f", 220, 198 - 80).push_frame("run_f", 252, 198 - 80)
		.push_frame("run_f", 284, 198 - 80).push_frame("run_f", 316, 198 - 80).push_frame("run_f", 348, 198 - 80).push_frame("run_f", 316, 198 - 80)
		.push_frame("run_f", 284, 198 - 80).push_frame("run_f", 252, 198 - 80);

	Sprites["Characters/A_Scout_Sprites"].set_sprite_frame_size(32, 70).push_frame("run_b", 390, 198 - 80).push_frame("run_b", 422, 198 - 80)
		.push_frame("run_b", 454, 198 - 80).push_frame("run_b", 486, 198 - 80).push_frame("run_b", 518, 198 - 80).push_frame("run_b", 486, 198 - 80)
		.push_frame("run_b", 454, 198 - 80).push_frame("run_b", 422, 198 - 80);

	Sprites["Characters/W_Scout_Sprites"].set_sprite_frame_size(45, 50).push_frame_row("run_l", 322, 166 - 75, -45, 0, 8);
	Sprites["Characters/W_Scout_Sprites"].set_sprite_frame_size(45, 50).push_frame_row("run_r", 368, 166 - 75, 45, 0, 8);

	Sprites["Characters/A_Scout_Sprites"].set_sprite_frame_size(45, 50).push_frame_row("run_l", 341, 118 - 45, -45, 0, 8);
	Sprites["Characters/A_Scout_Sprites"].set_sprite_frame_size(45, 50).push_frame_row("run_r", 386, 118 - 45, 45, 0, 8);

	Sprites["Characters/A_Scout_Sprites"].set_sprite_frame_size(32, 70).push_frame("idle_f", 284, 198 - 80);
	Sprites["Characters/A_Scout_Sprites"].set_sprite_frame_size(32, 70).push_frame("idle_b", 454, 198 - 80);
	Sprites["Characters/A_Scout_Sprites"].set_sprite_frame_size(45, 50).push_frame("idle_l", 341, 0);
	Sprites["Characters/A_Scout_Sprites"].set_sprite_frame_size(45, 50).push_frame("idle_r", 386, 0);

	Sprites["Characters/W_Scout_Sprites"].set_sprite_frame_size(32, 70).push_frame("idle_f", 264, 264 - 98);
	Sprites["Characters/W_Scout_Sprites"].set_sprite_frame_size(32, 70).push_frame("idle_b", 434, 264 - 98);
	Sprites["Characters/W_Scout_Sprites"].set_sprite_frame_size(45, 50).push_frame("idle_l", 323, 10);
	Sprites["Characters/W_Scout_Sprites"].set_sprite_frame_size(45, 50).push_frame("idle_r", 368, 10);

	float startTime;
	float elapsedTime = 0;
	float gameOverTimer;

	std::vector<Point> lerpPoints = { Point{ 525, 650 }, Point{ 800, 650 }, Point{ 800, 900 } , Point{ 600, 900 }, Point{ 600, 1200 },
		Point{ 800, 1200 }, Point{ 900, 1975 }, Point{ 1575, 1975 }, Point{ 1575, 1350 }, Point{ 1300, 1350 }, Point{ 1300, 1100 }, Point{ 1500, 1100 },
		Point{ 1500, 800 }, Point{ 1300, 800 }, Point{ 1150, 25 }, Point{ 525, 25 } };
	std::vector<float> lerpRot = { 0.0f, 1.57f, 3.14f, 1.57f, 0.0f, 1.57f, 0.0f, -1.57f, -3.14f, -1.57f, 0.0f, -1.57f, -3.14f, -1.57f, -3.14f, 1.57f };
	std::vector<float> lerpTimes = { 4.0f, 2.0f, 2.0f, 2.0f, 2.0f, 2.0f, 5.0f, 3.0f, 4.0f, 2.0f, 2.0f, 2.0f, 2.0f, 2.0f, 5.0f, 3.0f };

	Point test(player.getX(), player.getY());
	startTime = clock();
	int i = 0;

	//tell classes to copy this sprite cache and store positions of non-moving sprites (decorations)
	Player::loadSprites(Sprites, decorationData);

	//initialize objects
	XBox::XBoxInput xin;

	//load the map and give certain map data to everyone else
	Level matchMap1("MatchMap1"), matchMap2("MatchMap2"), matchMap3("MatchMap3"), tutorialMap("TutorialMap");
	matchMap1.loadColouredLines("assets/maps/map1.txt");
	matchMap2.loadColouredTexturedLines("assets/maps/map2.txt");
	matchMap3.loadColouredTexturedLines("assets/maps/map3.txt");
	tutorialMap.loadColouredTexturedLines("assets/maps/TutorialLevel.txt");
	std::unordered_map<std::string, Level> mapMap{
		{ "MatchMap1", matchMap1 },
		{ "MatchMap2", matchMap2 },
		{ "MatchMap3", matchMap3 },
		{ "TutorialMap", tutorialMap }
	};

	RayEngine::setMap(mapMap[selectedMap]);
	RayEngine::setResolution(modes[0]);
	RayEngine rayEngine(s_sp, theGame.m_proj);

	Button resume(850, 400, 150, resume);
	Button startMenuButtons[3] = {
		Button(500, 400, 150, startMatch),
		Button(300, 400, 150, startTutorial),
		Button(100, 400, 150, quitGame)
	};
	startMenuButtons[0].setPicture(Sprites["Buttons/VersusButt"]);
	startMenuButtons[1].setPicture(Sprites["Buttons/TutorialButt"]);
	startMenuButtons[2].setPicture(Sprites["Buttons/ExitButt"]);
	unsigned int numStartButtons = sizeof startMenuButtons / sizeof Button;

	Button mapButtons[3] = {
		Button(100, 800, 300, 150, setMap1),
		Button(500, 800, 300, 150, setMap2),
		Button(900, 800, 300, 150, setMap3)
	};
	mapButtons[0].setPicture(Sprites["Buttons/Map1Butt"]);
	mapButtons[1].setPicture(Sprites["Buttons/Map2Butt"]);
	mapButtons[2].setPicture(Sprites["Buttons/Map3Butt"]);
	unsigned int numMapButtons = sizeof mapButtons / sizeof Button;

	bool matchStarted = false;	//major part of combined logic to handle game states. Set to false when player looses lives and if false gameState = StartMenu
	while (theGame.update(30)) {

		if (!matchStarted && gameState != GameStates::Tutorial)
			gameState = GameStates::StartMenu;

		if (matchStarted) {
			if (gameState == GameStates::MapSelect) {
				for (int i = 0; i < numMapButtons; i++) {
					mapButtons[i].onClick();
					mapButtons[i].draw();
				}
			}

			else if (RayEngine::getMapName() == selectedMap && gameState == GameStates::MatchStart) {
				///player.update(&player2);
				///player2.update(&player);
				///rayEngine.castRays(player);
				///player.render2D(player, player2);
				if (player.getLives() == 0 || player2.getLives() == 0)
				{
					gameOverTimer = clock();
					gameState = GameStates::GameOver;
				}

				glViewport(0.0f, halfHeight, modes[0].width, halfHeight);
				spritelib::Shapes::set_color(0.3f, 0.3f, 0.3f);
				spritelib::Shapes::draw_rectangle(true, 0, 0, modes[0].width, modes[0].height / 2);

				player.update(&player2);
				rayEngine.castRays(player);
				player.render3D(&player2);

				gun.draw();
				Text::draw_string(to_string(player.getLives()), "TimesNewRoman", halfWidth * 2 - 50, halfHeight * 2 - 50);

				Text::set_color(1, 0, 0);
				Text::draw_string(to_string(player.getHealth()), "TimesNewRoman", 0, 100.0f);
				spritelib::Shapes::set_color(0, 1, 0);
				spritelib::Shapes::draw_circle(true, 15.0f, halfWidth, halfHeight);

				glViewport(0.0f, 0.0f, modes[0].width, halfHeight);
				spritelib::Shapes::set_color(0.3f, 0.3f, 0.3f);
				spritelib::Shapes::draw_rectangle(true, 0, 0, modes[0].width, modes[0].height / 2);

				player2.update(&player);
				rayEngine.castRays(player2);
				player2.render3D(&player);

				Text::set_color(1, 0, 0);
				Text::draw_string(to_string(player2.getHealth()), "TimesNewRoman", 0, 100.0f);
				spritelib::Shapes::set_color(0, 1, 0);
				spritelib::Shapes::draw_circle(true, 15.0f, halfWidth, halfHeight);
				wand.draw();
				Text::draw_string(to_string(player2.getLives()), "TimesNewRoman", halfWidth * 2 - 50, halfHeight * 2 - 50);
			}

			else if (RayEngine::getMapName() != selectedMap) {
				RayEngine::setMap(mapMap[selectedMap]);
			}

			else if (gameState == GameStates::GameOver)
			{
				glViewport(0.0f, 0.0f, modes[0].width, modes[0].height);
				float madness = clock() - gameOverTimer;

				//lerp background!
				player.getSkyBox()->draw();
				elapsedTime = (clock() - startTime) / 1000;

				spritelib::Shapes::set_color(0.3f, 0.3f, 0.3f);
				spritelib::Shapes::draw_rectangle(true, 0, 0, modes[0].width, modes[0].height / 2);

				rayEngine.castRays(player);

				if (i == lerpTimes.size())
				{
					i = 0;
					test = Point(player.getX(), player.getY());
					startTime = clock();
				}

				if (elapsedTime <= lerpTimes[i])
				{
					player.getSkyBox()->draw();
					player.lerp(test, lerpPoints[i], lerpRot[i], lerpTimes[i], elapsedTime);

					spritelib::Shapes::set_color(0.3f, 0.3f, 0.3f);
					spritelib::Shapes::draw_rectangle(true, 0, 0, modes[0].width, modes[0].height / 2);

					rayEngine.castRays(player);
				}

				else
				{
					i++;
					test = Point(player.getX(), player.getY());
					startTime = clock();
				}

				if (player.getLives() == 0)
				{
					spritelib::Text::draw_string("Mages Win", "TimesNewRoman", halfWidth - 200, halfHeight * 1.25, 3.0f);
				}

				else
				{
					spritelib::Text::draw_string("Militians Win", "TimesNewRoman", halfWidth - 250, halfHeight * 1.25, 3.0f);
				}

				if (madness >= 10000)
				{
					gameState = GameStates::StartMenu;
					matchStarted = false;
				}
			}
		}

		else if (gameState == GameStates::StartMenu) {
			if (RayEngine::getMapName() == "MatchMap3") {
				//lerp background!
				elapsedTime = (clock() - startTime) / 1000;
				player.getSkyBox()->draw();
				spritelib::Shapes::set_color(0.3f, 0.3f, 0.3f);
				spritelib::Shapes::draw_rectangle(true, 0, 0, modes[0].width, modes[0].height / 2);

				rayEngine.castRays(player);

				//reset if we've reached the last node
				if (i == lerpTimes.size())
				{
					i = 0;
					test = Point(player.getX(), player.getY());
					startTime = clock();
				}

				if (elapsedTime <= lerpTimes[i])
				{
					player.lerp(test, lerpPoints[i], lerpRot[i], lerpTimes[i], elapsedTime);
					spritelib::Shapes::set_color(0.3f, 0.3f, 0.3f);
					spritelib::Shapes::draw_rectangle(true, 0, 0, modes[0].width, modes[0].height / 2);

					rayEngine.castRays(player);

				}

				else
				{
					i++;
					test = Point(player.getX(), player.getY());
					startTime = clock();
				}
			}
			else {
				RayEngine::setMap(mapMap[selectedMap]);
				glViewport(0.0f, 0.0f, modes[0].width, modes[0].height);
				player.getPos() = Point{ 525, 650 };	//set to start of lerp
			}
			//handle buttons
			for (unsigned int i = 0; i < numStartButtons; i++) {
				startMenuButtons[i].draw();
				startMenuButtons[i].onClick();
			}
			if (startMenuButtons[0].clicked()) {
				gameState = GameStates::MatchStart;
				player.shift(525, 25);
				matchStarted = true;
			}
		}
		else if (gameState == GameStates::Tutorial) {
			if (RayEngine::getMapName() == "TutorialMap") {

				//Store the player position and rotation
				Point& pPos = player.getPos();
				float& pRot = player.getRot();

				//"Touch the shimmering wall to advance.", cases to teleport to the next room:
				if (pPos._y > 500 && pPos._y < 560) {
					pPos = Point(280, 750);
					pRot = 1.57f;
				}
				else if (pPos._y > 1220 && pPos._y < 1280) {
					pPos = Point(280, 1480);
					pRot = 1.57f;
				}
				else if (pPos._y > 1950) {
					gameState = GameStates::StartMenu;
					selectedMap = "MatchMap3";
				}

				//ground based on player's y coordinate:
				if (pPos._y < 560) {
					spritelib::Shapes::set_color(0.3f, 0.3f, 0.3f);
					spritelib::Shapes::draw_rectangle(true, 0, 0, modes[0].width, modes[0].height / 2);
					spritelib::Text::set_color(1.0f, 0.0f, 0.0f);
					spritelib::Text::draw_string("Welcome to Gun-Wandia", "TimesNewRoman", 900, 1000);
				}
				else if (pPos._y < 1280) {
					spritelib::Shapes::set_color(0.6f, 0.6f, 0.3f);
					spritelib::Shapes::draw_rectangle(true, 0, 0, modes[0].width, modes[0].height / 2);
				}
				else if (pPos._y < 2000) {
					spritelib::Shapes::set_color(0.8f, 0.9f, 0.0f);
					spritelib::Shapes::draw_rectangle(true, 0, 0, modes[0].width, modes[0].height / 2);
				}

				player.update(&player2);
				rayEngine.castRays(player);
				player.render3D(&player2);

				//text based on player's y coordinates
				if (pPos._y < 560) {
					spritelib::Text::draw_string("W-A-S-D or left analog stick for directional movement", "TimesNewRoman", 650, 1000);
					spritelib::Text::draw_string("Move the mouse or right analog to rotate", "TimesNewRoman", 725, 900);
					spritelib::Text::draw_string("(Touch the shimmering wall to advance rooms)", "TimesNewRoman", 700, 800);
				}
				else if (pPos._y < 1280) {
					spritelib::Text::draw_string("Press 1,2,3,4,Space or X,Y,A,B,RB to fire projectiles", "TimesNewRoman", 650, 900);
				}
				else if (pPos._y < 2000) {
					spritelib::Text::draw_string("Feel free to play around in this room, hit that wall once your ready to decide the fate of Gun-Wandia!", "TimesNewRoman", 400, 800);
				}
			}
			else {
				RayEngine::setMap(mapMap[selectedMap]);
				player.getPos() = Point(280, 50);
				player.getRot() = 1.57f;
				glViewport(0.0f, 0.0f, modes[0].width, modes[0].height);
			}
		}

		

		else if (gameState == GameStates::Pause) {
			resume.onClick();
			resume.draw();
		}


	}
	s_sp->cleanup();
	return 0;
}

void IterativeSelectionSort(std::vector<Sprite*>& a_sprites)
{
	for (unsigned int i = 0; i < a_sprites.size(); i++)
	{
		int minIndex = i;
		for (unsigned int j = i; j < a_sprites.size(); j++)
		{
			if (a_sprites[j]->get_depth() < a_sprites[minIndex]->get_depth())
			{
				minIndex = j;
			}
		}
		std::swap(a_sprites[minIndex], a_sprites[i]);
	}
}

void KeyboardFunc(Window::Key a_key, Window::EventType a_eventType)
{
	switch (a_eventType)
	{
	case Window::EventType::KeyPressed:
	{
		KeyEvents::keysDown[a_key] = true;
#if DEVELOPER_TOOLS
		if (a_key == Window::Key::Escape)
		{
			std::cout << "Escape pressed!\n";
			quitGame();
		}
#endif
		else if (a_key == Window::Key::P) {
			pause();
		}
	}
	break;
	case Window::EventType::KeyReleased:
	{
		KeyEvents::keysDown[a_key] = false;
	}
	break;
	}
}

void MouseFunc(Window::Button a_button, int a_mouseX, int a_mouseY, Window::EventType a_eventType)
{
	switch (a_eventType)
	{
	case Window::EventType::MouseMoved:
	{

	}
	break;
	case Window::EventType::MouseButtonReleased:
	{

	}
	break;
	}
}

void DrawSprites(std::vector<Sprite*> sprites)
{
	IterativeSelectionSort(sprites);	//sort sprites back to front before drawing because of transparency

	unsigned int size = sprites.size();
	unsigned int i = 0;
	for (; i<size; i++)
		sprites[i]->draw();
}

//write another function to take in the same arguments as push frame row.
void addSprite(SpriteMap& map, std::string name)	//necessary to format images and populate a list of Sprites rather than Textures.
{
	spritelib::Sprite image;
	image.load_sprite_image(name);
	image.set_center(0.5f, 0.5f);
	map.insert(std::make_pair(name, image));
}

void setSpriteAnimation(SpriteMap& map, std::string name, std::string animation_id, int start_x, int start_y, int x_spacing, int y_spacing, int num_frames)	//necessary to format images and populate a list of Sprites rather than Textures.
{
	map[name].set_sprite_frame_size(x_spacing, y_spacing);
	map[name].push_frame_row(animation_id, start_x, start_y, x_spacing, y_spacing, num_frames);
}