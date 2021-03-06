// This file is part of the course TPV2@UCM - Samir Genaim

#pragma once
#include <memory>
#include "box2d.h"
#include "SDL.h"

#include "../ecs/ecs.h"
#include "../ecs/Manager.h"
#include "../utils/Vector2D.h"
#include "../ecs/Entity.h"

#include "../sdlutils/InputHandler.h"
#include "../sdlutils/SDLUtils.h"
#include "..//components/DamageCollisionCallbacks.h"
//#include "../components/BoxCollider.h"

class GameState;
class StateMachine;
const Vector2D window(1280, 720);
// when you are not using the methods of a class, just
// say that it exists, that saves time when parsing files
class Manager;

class App {
public:
	static float camera_Zoom_Out;	//Zoom de la camara si asi soy, lo siento Joseda | pero por qu�Eme tomais por un dictador, que no soy �ngel.
	static SDL_Rect camera;
	static const int FPS = 16;
	App();
	virtual ~App();
	void init();
	void start();
	void ShakeCamera(int time);
	void createBackGround(const std::string& spriteId, const int & fils, const int & cols);
	static void fullScreen();
	void exitGame() { exit = true; }
	void changeToPesca();
	bool IsSavedGame() { return loadSavedGame; }
	void setIsSavedGame(bool value) {  loadSavedGame = value; }


	Entity* createBasicEntity(const Vector2D & pos, const Vector2D & size, const float & rotation, const Vector2D & vel);
	StateMachine* getStateMachine() { return stateMachine; }
	inline float getCameraZoomOut() const { return camera_Zoom_Out; }
	inline void setCameraZoomOut(float zoom) { 
		camera_Zoom_Out = zoom; 
		camera = { 0 ,0,(int)(window.getX() * camera_Zoom_Out),(int)(window.getY() * camera_Zoom_Out) };
	}

private:
	
	std::shared_ptr<b2World> world_;
	DamageCollisionCallbacks collisionListener;
	std::unique_ptr<Manager> mngr_;
	//Control de los estados en los que esta el juego 
	StateMachine* stateMachine;

	bool exit = false;

	MapProcedural* map;

	float brightness = 0;

	SoundManager* sndProvisional;

	bool loadSavedGame;
};