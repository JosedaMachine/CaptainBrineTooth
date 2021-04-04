#include "GameState.h"
#include "../ecs/Manager.h"


GameState::GameState(Game* a, b2World* mundo, SoundManager* snd)
{
	app = a;
	manager_ = new Manager(mundo); 
	soundController = snd;
}
GameState::~GameState() {
	//Para borrar las entidades , se pueden poner a todas las entidades del juego a dormir para que el manager 
	//Se encargue de borrarlas , el unico problema seria que por ejemplo cada vez que accedes al menu deberia de ser uno nuevo 

	delete manager_;
}
void GameState::render() const {
	manager_->render();
}
void GameState::update() {
	manager_->update();
}
void GameState::refresh() {
	manager_->refresh();
}
Manager* GameState::getMan() { return manager_; }