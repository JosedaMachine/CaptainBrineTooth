#pragma once
#include "GameState.h"
#include "box2d.h"
class Entity;

class PescaState :public GameState
{
public:
	PescaState(App* a, std::shared_ptr<b2World> mundo, SoundManager* snd) : GameState(a, mundo, snd) {};
	virtual void init();
	virtual void update();

	void createPlayer(const Config& playerConfig);
	void createBoxFloor(const Config& entityConfig);
	void createPompeyWorm(const Config& enemy1Config);
	void createElfShark(const Config& entityConfig);
	void createPesca(const Config& entityConfig);
protected:
	SDL_Rect cam;
};
