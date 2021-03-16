// This file is part of the course TPV2@UCM - Samir Genaim

#include "Game.h"

#include "../ecs/ecs.h"
#include "../ecs/Entity.h"
#include "../ecs/Manager.h"
#include "../components/Image.h"
#include "../components/Rotate.h"
#include "../components/Bounce.h"
#include "../components/Armas_HUD.h"
#include "../components/Transform.h"
#include "../components/Rectangle.h"
#include "../components/FramedImage.h"
#include "../components/BoxCollider.h"
#include "../components/KeyBoardCtrl.h"
#include "../components/Player_Health.h"
#include "..//components/enemyMovement.h"
#include "../sdlutils/InputHandler.h"
#include "../sdlutils/SDLUtils.h"
#include "../utils/Vector2D.h"

Game::Game() {
	mngr_.reset(new Manager());
	world_ = mngr_->getWorld();
}

Game::~Game() {
}

void Game::init() {
	SDLUtils::init("Captain BrineTooth", 800, 600, "assets/config/base.resources.json");



	auto* bg = mngr_->addEntity();
	bg->addComponent<Transform>(Vector2D(0, 0), Vector2D(), sdlutils().width(), sdlutils().height(), 0.0f);
	bg->addComponent<FramedImage>(&sdlutils().images().at("fondo"), 11, 11, 0.1f, 2);

	//Test ground
	auto* ground = mngr_->addEntity();
	ground->addComponent<Transform>(Vector2D(-150, 425), Vector2D(), sdlutils().width() * 10, 10.0f, 0.0f);
	ground->addComponent<Image>(&sdlutils().images().at("Square"));
	ground->addComponent<BoxCollider>(0.0f, false);


	auto* HUD = mngr_->addEntity();
	HUD->addComponent<Player_Health>(&sdlutils().images().at("vida"));
	HUD->addComponent<Armas_HUD>(&sdlutils().images().at("sierra"), &sdlutils().images().at("espada"));

	/*
		auto* enemy1 = mngr_->addEntity();
		enemy1->addComponent<Transform>(
		Vector2D(sdlutils().width() / 3.0f - 50.0, sdlutils().height() / 2.0f + 60.0f),
		Vector2D(), 50.0f, 50.0f, 0.0f);
		enemy1->addComponent<FramedImage>(&sdlutils().images().at("Medusa"), 7, 6, 200.0f, 4);
		enemy1->addComponent<BoxCollider>(0.0f, true);
		enemy1->addComponent<EnemyMovement>(Vector2D(1, 0));


		auto* player = mngr_->addEntity();
	player->addComponent<Transform>(Vector2D(sdlutils().width() / 2.0f + 250.0, sdlutils().height() / 2.0f), Vector2D(), 100.0f, 100.0f, 0.0f);
	player->addComponent<FramedImage>(&sdlutils().images().at("Player"), 8, 5, 100.0f, 2);

	*/



	auto* physBody = mngr_->addEntity();
	physBody->addComponent<Transform>(Vector2D(sdlutils().width() / 3.0f - 50.0, 205),Vector2D(), 50.0f, 50.0f, 0.0f);
	physBody->addComponent<Image>(&sdlutils().images().at("Square"));
	physBody->addComponent<BoxCollider>(0.0f, true);

	
	b2RevoluteJointDef* joint=new b2RevoluteJointDef();
	joint->bodyA = physBody->getComponent<BoxCollider>()->getBody();
	joint->bodyB= ground->getComponent<BoxCollider>()->getBody();
	joint->collideConnected = true;
	world_->CreateJoint(joint);
	
}

void Game::start() {

	// a boolean to exit the loop
	bool exit = false;
	SDL_Event event;

	while (!exit) {
		Uint32 startTime = sdlutils().currRealTime();

		ih().clearState();
		while (SDL_PollEvent(&event))
			ih().update(event);

		if (ih().isKeyDown(SDL_SCANCODE_ESCAPE)) {
			exit = true;
			continue;
		}


		world_->Step(1.0f / 60.0f, 6, 2);

		mngr_->update();
		mngr_->refresh();

		sdlutils().clearRenderer();
		mngr_->render();
		sdlutils().presentRenderer();

		Uint32 frameTime = sdlutils().currRealTime() - startTime;

		if (frameTime < 20)
			SDL_Delay(20 - frameTime);
	}

}


