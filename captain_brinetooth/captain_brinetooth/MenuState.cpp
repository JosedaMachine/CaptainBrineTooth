#include "MenuState.h"
#include "ecs/Entity.h"
#include "game/Game.h"
#include "utils/Vector2D.h"
#include "ecs/Manager.h"
#include "components/Animation.h"
#include "components/AnimBlendGraph.h"
#include "components/Button.h"
#include "PruebaState.h"

MenuState::MenuState(Game* a, b2World* mundo, SoundManager* snd) : GameState(a, mundo,snd)
{
	cam = a->camera;
}

void MenuState::addStateEntityMenu() {
	auto* newP = manager_->addEntity();
	newP->addComponent<Transform>(Vector2D(0, 0), Vector2D(0, 0), cam.w, cam.h, 0.0f);
	auto* anim_controller = newP->addComponent<AnimBlendGraph>();

	anim_controller->addAnimation("waves", &sdlutils().images().at("fondoMenu"), 11, 11, 1, 1, -1);

	soundController->playMainMusic();


	// Titulo
	auto* title = manager_->addEntity();
	title->addComponent<Transform>(Vector2D((cam.w / 3.1) - 50, 80), Vector2D(0, 0), cam.w - (cam.w/9), cam.h - (cam.h / 2), 0.0f);
	

	title->addComponent<Button>(&sdlutils().images().at("titulo"), changeToGame, app, soundController);
	
	// Posicion y altura del titulo para ajustar la posicion del siguiente boton
	Vector2D pos = title->getComponent<Transform>()->getPos();
	int alt = title->getComponent<Transform>()->getH();

	// Boton Iniciar
	auto botonIniciar = manager_->addEntity();
	botonIniciar->addComponent<Transform>(Vector2D(pos.getX() + 75, pos.getY() + alt/1.5), Vector2D(0, 0), cam.w - (cam.w/2.5), cam.h - (cam.h / 1.3), 0.0f);
	botonIniciar->addComponent<Button>(&sdlutils().images().at("jugar"), changeToGame, app, soundController);

	// Ajuste de la posicion y altura auxiliar para el siguiente boton
	pos = botonIniciar->getComponent<Transform>()->getPos();
	alt = botonIniciar->getComponent<Transform>()->getH();

	// Opciones
	auto opciones = manager_->addEntity();
	opciones->addComponent<Transform>(Vector2D(pos.getX(), pos.getY() + alt/1.25), Vector2D(0, 0), cam.w - (cam.w / 2.5), cam.h - (cam.h / 1.3), 0.0f);
	opciones->addComponent<Button>(&sdlutils().images().at("opciones"), changeToGame, app, soundController);

	// Ajuste de la posicion y altura auxiliar para el siguiente boton
	pos = opciones->getComponent<Transform>()->getPos();
	alt = opciones->getComponent<Transform>()->getH();

	// Boton de salir
	auto salir = manager_->addEntity();
	salir->addComponent<Transform>(Vector2D(pos.getX(), pos.getY() + alt / 1.25), Vector2D(0, 0), cam.w - (cam.w / 2.5), cam.h - (cam.h / 1.3), 0.0f);
	salir->addComponent<Button>(&sdlutils().images().at("salir"), changeToGame, app, soundController);
}

void MenuState::update()
{
	for (Entity* b : manager_->getEnteties())
	{
		Button* but = b->getComponent<Button>();
		if (but != nullptr && but->handleEvent())
		{
			break;
		}
	}
}

void MenuState::changeToGame(Game* g, SoundManager* snd)
{
	b2Vec2 gravity(0.0f, 9.8f);
	b2World* world_ = new b2World(gravity);
	snd->stopMusic();
	snd->playSoundEffect("gaviota");
	PruebaState* mainGame = new PruebaState(g, world_, snd);
	g->getGameStateMachine()->pushState(mainGame);
}