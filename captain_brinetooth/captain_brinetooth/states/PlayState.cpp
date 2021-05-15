﻿#include "PlayState.h"
#include "PasueState.h"
#include <fstream>

const auto MAP_PATH = "assets/maps/levelTest/levelTest - copia.tmx";

#define _CRTDBG_MAP_ALLOC
#include <crtdbg.h>
#ifdef _DEBUG
#define DEBUG_NEW (_NORMAL_BLOCK, __FILE__, __LINE__)
#define  DEBUG_NEW
#endif

PlayState::PlayState(App* a, std::shared_ptr<b2World> mundo, SoundManager* snd, bool saved) : GameState(a, mundo, snd) {
	//Testing floor
	//auto suelo = manager_->addEntity();
	//suelo->addComponent<Transform>(Vector2D(500, 600), Vector2D(), 500, 20, 0.0f);
	//auto anim = suelo->addComponent<AnimBlendGraph>();
	//anim->addAnimation("idle", &sdlutils().images().at("debug_square"), 1, 1, 1, 24, 0);
	//suelo->addComponent<BoxCollider>();
	save = saved;
}

PlayState::~PlayState() {
}

void PlayState::init() {
	//---BG----
	backgroundLevel = createBasicEntity(Vector2D(960, 640), Vector2D(2560, 1440), 0.0f, Vector2D(0, 0));
	auto* bgParallax = backgroundLevel->addComponent<ParallaxScroll>();

	/*bgParallax->addLayer(&sdlutils().images().at("bg_ice_layer1"), 0.2);
	bgParallax->addLayer(&sdlutils().images().at("bg_ice_layer2"), 0.25);
	bgParallax->addLayer(&sdlutils().images().at("bg_ice_layer3"), 0.35);
	bgParallax->addLayer(&sdlutils().images().at("bg_ice_layer4"), 0.4);
	bgParallax->addLayer(&sdlutils().images().at("bg_ice_layer5"), 0.45);*/

	////SE TIENE QUE CREAR PRIMERO EL NIVEL Y LUEGO EL PLAYER
	createLevel0();

	Config playerConfig{};
	playerConfig.pos = getMngr()->getHandler<Map>()->getComponent<MapProcedural>()->getPlayerPos();
	//playerConfig.pos = Vector2D(0, 0);
	playerConfig.vel = Vector2D(0, 0);
	playerConfig.size = Vector2D(100.0f, 100.0f);
	playerConfig.friction = 0.2f;
	playerConfig.physicType = DYNAMIC;
	playerConfig.fixedRotation = true;
	playerConfig.rotation = 0.0f;
	createPlayer(playerConfig);

	Config swordGiverConfig{};
	swordGiverConfig.pos = Vector2D(sdlutils().width() / 2.5f, sdlutils().height());
	swordGiverConfig.vel = Vector2D();
	swordGiverConfig.size = Vector2D(50, 50);
	swordGiverConfig.friction = 0.2f;
	swordGiverConfig.physicType = STATIC;
	swordGiverConfig.fixedRotation = true;
	swordGiverConfig.rotation = 0.0f;
	swordGiverConfig.spriteId = "espada";
	int swordNumber = 0;
	//createWeaponGiver(swordGiverConfig, swordNumber);

	getMngr()->getHandler<Map>()->getComponent<MapProcedural>()->setPlayer2spawn();

	/*Config hammerGiverConfig{};
	hammerGiverConfig.pos = Vector2D(sdlutils().width() / 2.0f, sdlutils().height());
	hammerGiverConfig.vel = Vector2D();
	hammerGiverConfig.size = Vector2D(50, 50);
	hammerGiverConfig.friction = 0.2f;
	hammerGiverConfig.physicType = STATIC;
	hammerGiverConfig.fixedRotation = true;
	hammerGiverConfig.rotation = 0.0f;
	hammerGiverConfig.spriteId = "martillo";
	int hammerNumber = 1;
	createWeaponGiver(hammerGiverConfig, hammerNumber);*/

	//Config chainsawGiverConfig{};
	//chainsawGiverConfig.pos = Vector2D(sdlutils().width() / 2.5f, sdlutils().height() - 100);
	//chainsawGiverConfig.vel = Vector2D();
	//chainsawGiverConfig.size = Vector2D(50, 50);
	//chainsawGiverConfig.friction = 0.2f;
	//chainsawGiverConfig.physicType = STATIC;
	//chainsawGiverConfig.fixedRotation = true;
	//chainsawGiverConfig.rotation = 0.0f;
	//chainsawGiverConfig.spriteId = "sierra";
	//int chainsawNumber = 2;
	//createWeaponGiver(chainsawGiverConfig, chainsawNumber);

	//Config crabGiverConfig{};
	//crabGiverConfig.pos = Vector2D(sdlutils().width() / 2.0f, sdlutils().height() - 100);
	//crabGiverConfig.vel = Vector2D();
	//crabGiverConfig.size = Vector2D(50, 50);
	//crabGiverConfig.friction = 0.2f;
	//crabGiverConfig.physicType = STATIC;
	//crabGiverConfig.fixedRotation = true;
	//crabGiverConfig.rotation = 0.0f;
	//crabGiverConfig.spriteId = "crab";
	//int crabNumber = 3;
	//createWeaponGiver(crabGiverConfig, crabNumber);

	//Config machineGunGiverConfig{};
	//machineGunGiverConfig.pos = Vector2D(sdlutils().width() / 1.75f, sdlutils().height());
	//machineGunGiverConfig.vel = Vector2D();
	//machineGunGiverConfig.size = Vector2D(50, 50);
	//machineGunGiverConfig.friction = 0.2f;
	//machineGunGiverConfig.physicType = STATIC;
	//machineGunGiverConfig.fixedRotation = true;
	//machineGunGiverConfig.rotation = 0.0f;
	//machineGunGiverConfig.spriteId = "machine_gun";
	//int machineGunNumber = 4;
	//createWeaponGiver(machineGunGiverConfig, machineGunNumber);

	/*Config whipGiverConfig{};
	whipGiverConfig.pos = Vector2D(sdlutils().width() / 1.75f, sdlutils().height() - 100);
	whipGiverConfig.vel = Vector2D();
	whipGiverConfig.size = Vector2D(50, 50);
	whipGiverConfig.friction = 0.2f;
	whipGiverConfig.physicType = STATIC;
	whipGiverConfig.fixedRotation = true;
	whipGiverConfig.rotation = 0.0f;
	whipGiverConfig.spriteId = "anguila";
	int whipNumber = 5;
	createWeaponGiver(whipGiverConfig, whipNumber);*/

	/*Config bellowGiverConfig{};
	bellowGiverConfig.pos = Vector2D(sdlutils().width() / 1.5f, sdlutils().height());
	bellowGiverConfig.vel = Vector2D();
	bellowGiverConfig.size = Vector2D(50, 50);
	bellowGiverConfig.friction = 0.2f;
	bellowGiverConfig.physicType = STATIC;
	bellowGiverConfig.fixedRotation = true;
	bellowGiverConfig.rotation = 0.0f;
	bellowGiverConfig.spriteId = "escupetintas";
	int bellowNumber = 5;
	createWeaponGiver(bellowGiverConfig, bellowNumber);*/

	//Config elfShark{};
	//elfShark.pos = Vector2D(sdlutils().width() * 1.6f, sdlutils().height() * 0.3f);
	//elfShark.vel = Vector2D(0, 0);
	//elfShark.size = Vector2D(180.0f, 180.0f);
	//elfShark.friction = 0.2f;
	//elfShark.physicType = KINEMATIC;
	//elfShark.fixedRotation = true;
	//elfShark.rotation = 0.0f;
	//elfShark.col = ENEMY;
	//elfShark.colMask = ENEMY_MASK;
	//createElfShark(elfShark);

	//Config flowerJellyHat{};
	//flowerJellyHat.pos = Vector2D(sdlutils().width() * 0.8f, sdlutils().height() * 0.8f);
	//flowerJellyHat.vel = Vector2D(0, 0);
	//flowerJellyHat.size = Vector2D(100.0f, 100.0f);
	//flowerJellyHat.friction = 0.2f;
	//flowerJellyHat.physicType = DYNAMIC;
	//flowerJellyHat.fixedRotation = true;
	//flowerJellyHat.rotation = 0.0f;
	//flowerJellyHat.col = ENEMY;
	//flowerJellyHat.colMask = ENEMY_MASK;
	//createFlowerJellyHat(flowerJellyHat);

	/*Config fishlerConfig{};
	fishlerConfig.pos = Vector2D(sdlutils().width(), sdlutils().height() * 1.3f);
	fishlerConfig.vel = Vector2D();
	fishlerConfig.size = Vector2D(100, 200);
	fishlerConfig.friction = 0.2f;
	fishlerConfig.physicType = DYNAMIC;
	fishlerConfig.col = ENEMY;
	fishlerConfig.colMask = ENEMY_MASK;
	fishlerConfig.fixedRotation = true;
	fishlerConfig.rotation = 0.0f;
	fishlerConfig.spriteId = "espada";
	Entity* fishler = manager_->addEntity();
	fishler->addComponent<Transform>(fishlerConfig.pos, fishlerConfig.vel, fishlerConfig.size.getX(), fishlerConfig.size.getY(), fishlerConfig.rotation);
	fishler->addComponent<BoxCollider>(fishlerConfig.physicType, fishlerConfig.col, fishlerConfig.colMask, false, fishlerConfig.friction);
	AnimBlendGraph* fishler_anim_controller = fishler->addComponent<AnimBlendGraph>();
	fishler_anim_controller->addAnimation("sinMoverse", &sdlutils().images().at(fishlerConfig.spriteId), 1, 1, 1, 23, 1);
	fishler->addComponent<Enemy_Health>(5000, Vector2D(300, 20), build_sdlcolor(255, 0, 0, 255), 50);
	fishler->addComponent<FishlerController>();
	fishler->addComponent<ContactDamage>();*/
}

void PlayState::update() {
	//Queremos que, en el estado que hayan objetos físicos, se actualicen las físicas
	manager_->getWorld()->Step(1.0f / 60.0f, 6, 2);
	if (ih().keyDownEvent()) {
		if (ih().isKeyDown(SDL_SCANCODE_ESCAPE)) {
			app->setloadSavedGame(true);
			manager_->getSoundMngr()->playPauseMusic();
			StateMachine* sM = app->getStateMachine();
			//Pusheamos n nuevo estado de pausa pero antes guardando las vidas y el inventario del player 
			createSaveDataandSTate();
		}
	}
	GameState::update();
}

/// <summary>
/// Crea el tile nivel 0 con f�sicas
/// </summary>
void PlayState::createLevel0() {
	auto* nivel = manager_->addEntity();
	nivel->addComponent<EnemyGenerator>();
	Level0* levelTile = nivel->addComponent<Level0>(MAP_PATH, manager_->getWorld());
	map = nivel->addComponent<MapProcedural>(3, 0, app);
	getMngr()->setHandler<Map>(nivel);
	camLimits = levelTile->getMaxCoordenates();
}

void PlayState::createPlayer(const Config& playerConfig) {
	auto* player = createBasicEntity(playerConfig.pos, playerConfig.size, playerConfig.rotation, playerConfig.vel);

#pragma region Animations
	//Plantilla de uso de ANIMATION CONTROLLER
	auto* anim_controller = player->addComponent<AnimBlendGraph>();
#pragma region idle, run & jump
	//-idle, run & jump---------------------------------------------------------------------------------------------------
	//Animations
	anim_controller->addAnimation("idle", &sdlutils().images().at("player_idle"), 4, 6, 24, 24, -1);
	anim_controller->addAnimation("run", &sdlutils().images().at("player_run"), 4, 5, 20, 24, -1);
	anim_controller->addAnimation("jump", &sdlutils().images().at("player_jump"), 4, 6, 24, 24, 0);
	anim_controller->addAnimation("dash_ground", &sdlutils().images().at("player_dash_ground"), 3, 4, 12, 60, 0, 0, 11, Vector2D(0.5, 0.2));
	anim_controller->addAnimation("dash_air", &sdlutils().images().at("player_dash_air"), 3, 4, 12, 60, 0, 0, 11, Vector2D(0.5, 0.5));
	anim_controller->addAnimation("death", &sdlutils().images().at("player_death"), 6, 8, 48, 20, 0, 0, 47, Vector2D(0.5, 0.7));
	//Proportion?
	anim_controller->keepProportion("idle", Vector2D(player->getComponent<Transform>()->getW(), player->getComponent<Transform>()->getH()));
	//Transitions 
	anim_controller->addTransition("idle", "run", "Speed", 1, false);
	anim_controller->addTransition("run", "idle", "Speed", 0, false);
	anim_controller->addTransition("run", "jump", "NotOnFloor", 1, false);	//Anim fuente, anim destino, parametro, valor de parametro, esperar a que termine la animacion
	anim_controller->addTransition("jump", "run", "NotOnFloor", 0, false);
	anim_controller->addTransition("idle", "jump", "NotOnFloor", 1, false);
	anim_controller->addTransition("jump", "idle", "NotOnFloor", 0, true);
#pragma endregion
#pragma region dash
	//Transitions
	anim_controller->addTransition("jump", "dash_air", "Dash_Air", 1, false);
	anim_controller->addTransition("dash_air", "jump", "Dash_Air", 0, true);
	anim_controller->addTransition("run", "dash_ground", "Dash_Ground", 1, false);
	anim_controller->addTransition("dash_ground", "run", "Dash_Ground", 0, true);
	anim_controller->addTransition("idle", "dash_ground", "Dash_Ground", 1, false);
	anim_controller->addTransition("dash_ground", "idle", "Dash_Ground", 0, true);
	//death
	anim_controller->addTransition("death", "idle", "Dead", 23, false);
	anim_controller->addTransition("idle", "death", "Dead", 1, false);
	anim_controller->addTransition("run", "death", "Dead", 1, false);
	anim_controller->addTransition("jump", "death", "Dead", 1, false);
	anim_controller->addTransition("dash_ground", "death", "Dead", 1, false);
	anim_controller->addTransition("dash_air", "death", "Dead", 1, false);
#pragma endregion
#pragma region parametros
	//--Parametros--
	anim_controller->setParamValue("NotOnFloor", 0);	//AVISO: Si no existe el parametro, no hara nada
	anim_controller->setParamValue("Speed", 0);
	anim_controller->setParamValue("Dash_Air", 0);
	anim_controller->setParamValue("Dash_Ground", 0);
	anim_controller->setParamValue("Dead", 0);
#pragma endregion
#pragma region Weapons
	//-WEAPONS------------------------------------------------------------------------------------------------------
#pragma region Chainsaw
	//---CHAINSAW---------------------------------------------------------------------------------------------------
	anim_controller->addAnimation("chainsaw_attack1", &sdlutils().images().at("chainsaw_combo"), 6, 8, 48, 15, 0, 1, 8, Vector2D(0.75, 0.72));
	anim_controller->addAnimation("chainsaw_attack2", &sdlutils().images().at("chainsaw_combo"), 6, 8, 48, 15, 0, 9, 18, Vector2D(0.75, 0.72));
	anim_controller->addAnimation("chainsaw_attack3", &sdlutils().images().at("chainsaw_combo"), 6, 8, 48, 28, 0, 19, 27, Vector2D(0.75, 0.72));
	anim_controller->addAnimation("chainsaw_attack4", &sdlutils().images().at("chainsaw_combo"), 6, 8, 48, 24, -1, 28, 47, Vector2D(0.75, 0.72));

	anim_controller->addTransition("run", "chainsaw_attack1", "chainsaw_att", 1, false);
	anim_controller->addTransition("idle", "chainsaw_attack1", "chainsaw_att", 1, false);
	anim_controller->addTransition("jump", "chainsaw_attack1", "chainsaw_att", 1, false);
	anim_controller->addTransition("chainsaw_attack1", "run", "chainsaw_att", 0, true);
	anim_controller->addTransition("chainsaw_attack1", "chainsaw_attack2", "chainsaw_att", 2, true);
	anim_controller->addTransition("run", "chainsaw_attack2", "chainsaw_att", 2, false);
	anim_controller->addTransition("idle", "chainsaw_attack2", "chainsaw_att", 2, false);
	anim_controller->addTransition("jump", "chainsaw_attack2", "chainsaw_att", 2, false);
	anim_controller->addTransition("chainsaw_attack2", "run", "chainsaw_att", 0, true);
	anim_controller->addTransition("chainsaw_attack2", "chainsaw_attack3", "chainsaw_att", 3, true);
	anim_controller->addTransition("run", "chainsaw_attack3", "chainsaw_att", 3, false);
	anim_controller->addTransition("idle", "chainsaw_attack3", "chainsaw_att", 3, false);
	anim_controller->addTransition("jump", "chainsaw_attack3", "chainsaw_att", 3, false);
	anim_controller->addTransition("chainsaw_attack3", "chainsaw_attack4", "chainsaw_att", 4, true);
	anim_controller->addTransition("chainsaw_attack4", "run", "chainsaw_att", 0, false);
	anim_controller->addTransition("chainsaw_attack4", "chainsaw_attack1", "chainsaw_att", 1, false);

	anim_controller->addTransition("chainsaw_attack1", "dash_air", "Dash_Air", 1, false);
	anim_controller->addTransition("chainsaw_attack1", "dash_ground", "Dash_Ground", 1, false);
	anim_controller->addTransition("dash_air", "chainsaw_attack1", "chainsaw_att", 1, true);
	anim_controller->addTransition("dash_ground", "chainsaw_attack1", "chainsaw_att", 1, true);
	anim_controller->addTransition("chainsaw_attack2", "dash_air", "Dash_Air", 1, false);
	anim_controller->addTransition("chainsaw_attack2", "dash_ground", "Dash_Ground", 1, false);
	anim_controller->addTransition("dash_air", "chainsaw_attack2", "chainsaw_att", 2, true);
	anim_controller->addTransition("dash_ground", "chainsaw_attack2", "chainsaw_att", 2, true);
	anim_controller->addTransition("chainsaw_attack3", "dash_air", "Dash_Air", 1, false);
	anim_controller->addTransition("chainsaw_attack3", "dash_ground", "Dash_Ground", 1, false);
	anim_controller->addTransition("dash_air", "chainsaw_attack3", "chainsaw_att", 3, true);
	anim_controller->addTransition("dash_ground", "chainsaw_attack3", "chainsaw_att", 3, true);
	anim_controller->addTransition("chainsaw_attack4", "dash_air", "Dash_Air", 1, false);
	anim_controller->addTransition("chainsaw_attack4", "dash_ground", "Dash_Ground", 1, false);
	//death
	anim_controller->addTransition("chainsaw_attack1", "death", "Dead", 1, false);
	anim_controller->addTransition("chainsaw_attack2", "death", "Dead", 1, false);
	anim_controller->addTransition("chainsaw_attack3", "death", "Dead", 1, false);
	anim_controller->addTransition("chainsaw_attack4", "death", "Dead", 1, false);

	anim_controller->setParamValue("chainsaw_att", 0);
	//--------------------------------------------------------------------------------------------------------------
#pragma endregion

#pragma region Sword
	//---SWORD------------------------------------------------------------------------------------------------------
	anim_controller->addAnimation("sword_attack1", &sdlutils().images().at("sword_combo"), 6, 8, 48, 48, 0, 1, 9, Vector2D(0.71, 0.69));
	anim_controller->addAnimation("sword_attack2", &sdlutils().images().at("sword_combo"), 6, 8, 48, 48, 0, 12, 22, Vector2D(0.71, 0.69));
	anim_controller->addAnimation("sword_attack3", &sdlutils().images().at("sword_combo"), 6, 8, 48, 48, 0, 23, 35, Vector2D(0.71, 0.69));
	anim_controller->addAnimation("sword_attack4", &sdlutils().images().at("sword_combo"), 6, 8, 48, 48, -1, 36, 47, Vector2D(0.71, 0.69));

	anim_controller->addTransition("run", "sword_attack1", "sword_att", 1, false);
	anim_controller->addTransition("idle", "sword_attack1", "sword_att", 1, false);
	anim_controller->addTransition("jump", "sword_attack1", "sword_att", 1, false);
	anim_controller->addTransition("sword_attack1", "run", "sword_att", 0, true);
	anim_controller->addTransition("sword_attack1", "sword_attack2", "sword_att", 2, true);
	anim_controller->addTransition("run", "sword_attack2", "sword_att", 2, false);
	anim_controller->addTransition("idle", "sword_attack2", "sword_att", 2, false);
	anim_controller->addTransition("jump", "sword_attack2", "sword_att", 2, false);
	anim_controller->addTransition("sword_attack2", "run", "sword_att", 0, true);
	anim_controller->addTransition("sword_attack2", "sword_attack3", "sword_att", 3, true);
	anim_controller->addTransition("run", "sword_attack3", "sword_att", 3, false);
	anim_controller->addTransition("idle", "sword_attack3", "sword_att", 3, false);
	anim_controller->addTransition("jump", "sword_attack3", "sword_att", 3, false);
	anim_controller->addTransition("sword_attack3", "sword_attack4", "sword_att", 4, true);
	anim_controller->addTransition("sword_attack4", "run", "sword_att", 0, false);
	anim_controller->addTransition("sword_attack4", "sword_attack1", "sword_att", 1, false);

	anim_controller->addTransition("sword_attack1", "dash_air", "Dash_Air", 1, false);
	anim_controller->addTransition("sword_attack1", "dash_ground", "Dash_Ground", 1, false);
	anim_controller->addTransition("dash_air", "sword_attack1", "sword_att", 1, true);
	anim_controller->addTransition("dash_ground", "sword_attack1", "sword_att", 1, true);
	anim_controller->addTransition("sword_attack2", "dash_air", "Dash_Air", 1, false);
	anim_controller->addTransition("sword_attack2", "dash_ground", "Dash_Ground", 1, false);
	anim_controller->addTransition("dash_air", "sword_attack2", "sword_att", 2, true);
	anim_controller->addTransition("dash_ground", "sword_attack2", "sword_att", 2, true);
	anim_controller->addTransition("sword_attack3", "dash_air", "Dash_Air", 1, false);
	anim_controller->addTransition("sword_attack3", "dash_ground", "Dash_Ground", 1, false);
	anim_controller->addTransition("dash_air", "sword_attack3", "sword_att", 3, true);
	anim_controller->addTransition("dash_ground", "sword_attack3", "sword_att", 3, true);
	anim_controller->addTransition("sword_attack4", "dash_air", "Dash_Air", 1, false);
	anim_controller->addTransition("sword_attack4", "dash_ground", "Dash_Ground", 1, false);
	//death
	anim_controller->addTransition("sword_attack1", "death", "Dead", 1, false);
	anim_controller->addTransition("sword_attack2", "death", "Dead", 1, false);
	anim_controller->addTransition("sword_attack4", "death", "Dead", 1, false);
	anim_controller->addTransition("sword_attack4", "death", "Dead", 1, false);

	anim_controller->setParamValue("sword_att", 0);
	//--------------------------------------------------------------------------------------------------------------
#pragma endregion

#pragma region Hammer
//---HAMMER---------------------------------------------------------------------------------------------------------
	anim_controller->addAnimation("hammer_attack1", &sdlutils().images().at("hammer_combo"), 5, 7, 31, 60, 0, 1, 16, Vector2D(0.75, 0.69));
	anim_controller->addAnimation("hammer_attack2", &sdlutils().images().at("hammer_combo"), 5, 7, 31, 48, 0, 17, 30, Vector2D(0.75, 0.69));

	anim_controller->addTransition("run", "hammer_attack1", "hammer_att", 1, false);
	anim_controller->addTransition("idle", "hammer_attack1", "hammer_att", 1, false);
	anim_controller->addTransition("jump", "hammer_attack1", "hammer_att", 1, false);
	anim_controller->addTransition("hammer_attack1", "run", "hammer_att", 0, false);
	anim_controller->addTransition("hammer_attack1", "hammer_attack2", "hammer_att", 2, true);
	anim_controller->addTransition("run", "hammer_attack2", "hammer_att", 2, false);
	anim_controller->addTransition("idle", "hammer_attack2", "hammer_att", 2, false);
	anim_controller->addTransition("jump", "hammer_attack2", "hammer_att", 2, false);
	anim_controller->addTransition("hammer_attack2", "run", "hammer_att", 0, false);
	anim_controller->addTransition("hammer_attack2", "hammer_attack1", "hammer_att", 1, false);

	anim_controller->addTransition("hammer_attack1", "dash_air", "Dash_Air", 1, false);
	anim_controller->addTransition("hammer_attack1", "dash_ground", "Dash_Ground", 1, false);
	anim_controller->addTransition("dash_air", "hammer_attack1", "hammer_att", 1, true);
	anim_controller->addTransition("dash_ground", "hammer_attack1", "hammer_att", 1, true);
	anim_controller->addTransition("hammer_attack2", "dash_air", "Dash_Air", 1, false);
	anim_controller->addTransition("hammer_attack2", "dash_ground", "Dash_Ground", 1, false);
	anim_controller->addTransition("dash_air", "hammer_attack2", "hammer_att", 2, true);
	anim_controller->addTransition("dash_ground", "hammer_attack2", "hammer_att", 2, true);
	//death
	anim_controller->addTransition("hammer_attack1", "death", "Dead", 1, false);
	anim_controller->addTransition("hammer_attack2", "death", "Dead", 1, false);

	anim_controller->setParamValue("hammer_att", 0);
	//	//--------------------------------------------------------------------------------------------------------------
#pragma endregion
//
//#pragma region Crab
////---CRAB---------------------------------------------------------------------------------------------------------
	anim_controller->addAnimation("crab_attack1", &sdlutils().images().at("crab_combo"), 6, 7, 39, 35, 0, 1, 8, Vector2D(0.71, 0.5));
	anim_controller->addAnimation("crab_attack2", &sdlutils().images().at("crab_combo"), 6, 7, 39, 48, 0, 8, 25, Vector2D(0.71, 0.5));
	anim_controller->addAnimation("crab_attack3", &sdlutils().images().at("crab_combo"), 6, 7, 39, 48, 0, 27, 38, Vector2D(0.71, 0.5));

	anim_controller->addTransition("run", "crab_attack1", "crab_att", 1, false);
	anim_controller->addTransition("idle", "crab_attack1", "crab_att", 1, false);
	anim_controller->addTransition("jump", "crab_attack1", "crab_att", 1, false);
	anim_controller->addTransition("crab_attack1", "run", "crab_att", 0, true);
	anim_controller->addTransition("crab_attack1", "crab_attack2", "crab_att", 2, true);
	anim_controller->addTransition("run", "crab_attack2", "crab_att", 2, false);
	anim_controller->addTransition("idle", "crab_attack2", "crab_att", 2, false);
	anim_controller->addTransition("jump", "crab_attack2", "crab_att", 2, false);
	anim_controller->addTransition("crab_attack2", "run", "crab_att", 0, false);
	anim_controller->addTransition("crab_attack2", "crab_attack3", "crab_att", 3, false);
	anim_controller->addTransition("run", "crab_attack3", "crab_att", 3, false);
	anim_controller->addTransition("idle", "crab_attack3", "crab_att", 3, false);
	anim_controller->addTransition("jump", "crab_attack3", "crab_att", 3, false);
	anim_controller->addTransition("crab_attack3", "run", "crab_att", 0, false);
	anim_controller->addTransition("crab_attack3", "crab_attack1", "crab_att", 1, true);

	anim_controller->addTransition("crab_attack1", "dash_air", "Dash_Air", 1, false);
	anim_controller->addTransition("crab_attack1", "dash_ground", "Dash_Ground", 1, false);
	anim_controller->addTransition("dash_air", "crab_attack1", "crab_att", 1, true);
	anim_controller->addTransition("dash_ground", "crab_attack1", "crab_att", 1, true);
	anim_controller->addTransition("crab_attack2", "dash_air", "Dash_Air", 1, false);
	anim_controller->addTransition("crab_attack2", "dash_ground", "Dash_Ground", 1, false);
	anim_controller->addTransition("dash_air", "crab_attack2", "crab_att", 2, true);
	anim_controller->addTransition("dash_ground", "crab_attack2", "crab_att", 2, true);
	anim_controller->addTransition("crab_attack3", "dash_air", "Dash_Air", 1, false);
	anim_controller->addTransition("crab_attack3", "dash_ground", "Dash_Ground", 1, false);
	anim_controller->addTransition("dash_air", "crab_attack3", "crab_att", 3, true);
	anim_controller->addTransition("dash_ground", "crab_attack3", "crab_att", 3, true);
	//death
	anim_controller->addTransition("crab_attack1", "death", "Dead", 1, false);
	anim_controller->addTransition("crab_attack2", "death", "Dead", 1, false);
	anim_controller->addTransition("crab_attack3", "death", "Dead", 1, false);

	anim_controller->setParamValue("crab_att", 0);

	//FISTS

	anim_controller->addAnimation("fist_attack1", &sdlutils().images().at("fists_combo"), 6, 7, 39, 35, 0, 1, 8, Vector2D(0.71, 0.5));
	anim_controller->addAnimation("fist_attack2", &sdlutils().images().at("fists_combo"), 6, 7, 39, 48, 0, 8, 25, Vector2D(0.71, 0.5));
	anim_controller->addAnimation("fist_attack3", &sdlutils().images().at("fists_combo"), 6, 7, 39, 48, 0, 27, 38, Vector2D(0.71, 0.5));

	anim_controller->addTransition("run", "fist_attack1", "fist_att", 1, false);
	anim_controller->addTransition("idle", "fist_attack1", "fist_att", 1, false);
	anim_controller->addTransition("jump", "fist_attack1", "fist_att", 1, false);
	anim_controller->addTransition("fist_attack1", "run", "fist_att", 0, true);
	anim_controller->addTransition("fist_attack1", "fist_attack2", "fist_att", 2, true);
	anim_controller->addTransition("run", "fist_attack2", "fist_att", 2, false);
	anim_controller->addTransition("idle", "fist_attack2", "fist_att", 2, false);
	anim_controller->addTransition("jump", "fist_attack2", "fist_att", 2, false);
	anim_controller->addTransition("fist_attack2", "run", "fist_att", 0, false);
	anim_controller->addTransition("fist_attack2", "fist_attack3", "fist_att", 3, false);
	anim_controller->addTransition("run", "fist_attack3", "fist_att", 3, false);
	anim_controller->addTransition("idle", "fist_attack3", "fist_att", 3, false);
	anim_controller->addTransition("jump", "fist_attack3", "fist_att", 3, false);
	anim_controller->addTransition("fist_attack3", "run", "fist_att", 0, false);
	anim_controller->addTransition("fist_attack3", "fist_attack1", "fist_att", 1, true);

	anim_controller->addTransition("fist_attack1", "dash_air", "Dash_Air", 1, false);
	anim_controller->addTransition("fist_attack1", "dash_ground", "Dash_Ground", 1, false);
	anim_controller->addTransition("dash_air", "fist_attack1", "fist_att", 1, true);
	anim_controller->addTransition("dash_ground", "fist_attack1", "fist_att", 1, true);
	anim_controller->addTransition("fist_attack2", "dash_air", "Dash_Air", 1, false);
	anim_controller->addTransition("fist_attack2", "dash_ground", "Dash_Ground", 1, false);
	anim_controller->addTransition("dash_air", "fist_attack2", "fist_att", 2, true);
	anim_controller->addTransition("dash_ground", "fist_attack2", "fist_att", 2, true);
	anim_controller->addTransition("fist_attack3", "dash_air", "Dash_Air", 1, false);
	anim_controller->addTransition("fist_attack3", "dash_ground", "Dash_Ground", 1, false);
	anim_controller->addTransition("dash_air", "fist_attack3", "fist_att", 3, true);
	anim_controller->addTransition("dash_ground", "fist_attack3", "fist_att", 3, true);
	//death
	anim_controller->addTransition("fist_attack1", "death", "Dead", 1, false);
	anim_controller->addTransition("fist_attack2", "death", "Dead", 1, false);
	anim_controller->addTransition("fist_attack3", "death", "Dead", 1, false);

	anim_controller->setParamValue("fist_att", 0);

	//	//--------------------------------------------------------------------------------------------------------------
	//#pragma endregion
	//
	//#pragma region MachineGun
	anim_controller->addAnimation("machine_gun1", &sdlutils().images().at("machineGun_combo"), 5, 6, 29, 48, -1, 1, 7, Vector2D(0.65, 0.5));
	anim_controller->addAnimation("machine_gun2", &sdlutils().images().at("machineGun_combo"), 5, 6, 29, 48, 0, 8, 27, Vector2D(0.65, 0.5));

	anim_controller->addTransition("run", "machine_gun1", "machineGun_att", 1, false);
	anim_controller->addTransition("idle", "machine_gun1", "machineGun_att", 1, false);
	anim_controller->addTransition("jump", "machine_gun1", "machineGun_att", 1, false);
	anim_controller->addTransition("machine_gun1", "run", "machineGun_att", 0, false);
	anim_controller->addTransition("machine_gun1", "machine_gun2", "machineGun_att", 2, false);
	anim_controller->addTransition("machine_gun2", "run", "machineGun_att", 0, false);
	anim_controller->addTransition("machine_gun2", "idle", "machineGun_att", 0, false);
	anim_controller->addTransition("machine_gun2", "machine_gun1", "machineGun_att", 1, false);

	anim_controller->addTransition("machine_gun1", "dash_air", "Dash_Air", 1, false);
	anim_controller->addTransition("machine_gun1", "dash_ground", "Dash_Ground", 1, false);
	anim_controller->addTransition("dash_air", "machine_gun1", "machineGun_att", 1, true);
	anim_controller->addTransition("dash_ground", "machine_gun1", "machineGun_att", 1, true);
	anim_controller->addTransition("machine_gun2", "dash_air", "Dash_Air", 1, false);
	anim_controller->addTransition("machine_gun2", "dash_ground", "Dash_Ground", 1, false);
	//death
	anim_controller->addTransition("machine_gun1", "death", "Dead", 1, false);
	anim_controller->addTransition("machine_gun2", "death", "Dead", 1, false);

	anim_controller->setParamValue("machineGun_att", 0);

	//Bellow

	anim_controller->addAnimation("bellow", &sdlutils().images().at("squid_combo"), 3, 4, 12, 48, 1, Vector2D(0.67, 0.5));

	anim_controller->addTransition("run", "bellow", "bellow_att", 1, false);
	anim_controller->addTransition("idle", "bellow", "bellow_att", 1, false);
	anim_controller->addTransition("jump", "bellow", "bellow_att", 1, false);
	anim_controller->addTransition("bellow", "run", "bellow_att", 0, false);
	anim_controller->addTransition("bellow", "idle", "bellow_att", 0, false);

	anim_controller->addTransition("bellow", "dash_air", "Dash_Air", 1, false);
	anim_controller->addTransition("bellow", "dash_ground", "Dash_Ground", 1, false);
	anim_controller->addTransition("dash_air", "bellow", "bellow_att", 1, true);
	anim_controller->addTransition("dash_ground", "bellow", "bellow_att", 1, true);
	//death
	anim_controller->addTransition("bellow", "death", "Dead", 1, false);

	anim_controller->setParamValue("bellow_att", 0);
	//--------------------------------------------------------------------------------------------------------------
#pragma endregion
	//--------------------------------------------------------------------------------------------------------------
#pragma endregion
#pragma endregion

	player->addComponent<BoxCollider>(playerConfig.physicType, PLAYER, PLAYER_MASK, false,
		playerConfig.friction, playerConfig.fixedRotation, playerConfig.rotation, Vector2D(playerConfig.size.getX() * 0.6, playerConfig.size.getY() * 0.95));
	player->addComponent<TriggerCollider>("Feet", PLAYER_JUMP, PLAYER_JUMP_MASK, Vector2D(0, 0.25), Vector2D(60.0f, 15.0f));
	player->addComponent<Player_Health>(&sdlutils().images().at("fullvida"), &sdlutils().images().at("mediavida"), &sdlutils().images().at("vacio"), 300.0f, app);
	player->addComponent<Armas_HUD>(app);

	/*if(playerConfig.physicType != KINEMATIC)*/ player->addComponent<PlayerController>();
	//else player->addComponent<KeyBoardCtrl>(map);

	//Por testing basura
	player->addComponent<CameraFollow>(Vector2D(200.0f, -80.0f), 0.1f, false, false, manager_->getHandler<Map>()->getComponent<Level0>()->getMaxCoordenates()); //Vector2D offset y porcentaje de la velocidad de la camara, mas bajo mas lento sigue
	player->addComponent<Inventory>();

	player->addComponent<LoseLife>();

	if (save)
	{
		ifstream readtxt;
		readtxt.open("savedGame.dat");
		float life;
		int weapon1;
		int weapon2;
		string info;
		readtxt >> info;

		if (info == "Vida:")readtxt >> life;
		readtxt >> info;
		if (info == "Arma0:")readtxt >> weapon1;
		readtxt >> info;
		if (info == "Arma1:")readtxt >> weapon2;

		player->getComponent<Player_Health>()->setLife(life);


		if (weapon2 < 100 && weapon1 < 100) 
		{
			if (weapon2 < 100)player->getComponent<Inventory>()->addWeapon(weapon1);
			if (weapon1 < 100)player->getComponent<Inventory>()->addWeapon(weapon2);

		}

		if (weapon2 < 100)player->getComponent<Inventory>()->addWeapon(weapon2);
		if (weapon1 < 100)player->getComponent<Inventory>()->addWeapon(weapon1);

	}
	//Seteamos al Player como MainHandler
	manager_->setHandler<Player>(player);

	//Carga de archivo
}
void PlayState::createSaveDataandSTate()
{

	StateMachine* sM = app->getStateMachine();
	//Savings players Data 
	infoPartida infoPlayer;
	Entity* playerrefAux = app->getStateMachine()->currentState()->getMngr()->getHandler<Player>();
	infoPlayer.playerLife = playerrefAux->getComponent<Player_Health>()->getLife();
	infoPlayer.weapon1 = playerrefAux->getComponent<Inventory>()->getWeapon(0);
	infoPlayer.weapon2 = playerrefAux->getComponent<Inventory>()->getWeapon(1);
	//Si no hay armas las ponemos a 0 como decision de diseño en el elemento de guardado 
	if (playerrefAux->getComponent<Inventory>()->emptyInventory())
	{
		infoPlayer.weapon1 = 100;
		infoPlayer.weapon2 = 100;
	}
	else    //Si tiene un arma ponemos la otra a 100( como si fuese null) si no leemos las dos puesto que tendria dos armas
	{
		if(playerrefAux->getComponent<Inventory>()->hasOneWeapon())infoPlayer.weapon2 = 100;
		//si no tiene el inventario lleno , con lo cual se le pasa direcatente la lectura de las armas 


	}
	sM->pushState(new PauseState(this, app, sM->currentState()->getMngr()->getWorld(), sM->currentState()->getMngr()->getSoundMngr(), infoPlayer));





}
Entity* PlayState::getBackgroundLevel(){
	return backgroundLevel;
}
void PlayState::createWeaponGiver(const Config& weaponGiverConfig, const int& weaponType) {
	auto* weaponGiver = createBasicEntity(weaponGiverConfig.pos, weaponGiverConfig.size, weaponGiverConfig.rotation, weaponGiverConfig.vel);

	auto* anim_controller = weaponGiver->addComponent<AnimBlendGraph>();
	anim_controller->addAnimation("idle", &sdlutils().images().at(weaponGiverConfig.spriteId), 1, 1, 1, 24, 1);
	weaponGiver->addComponent<BoxCollider>(weaponGiverConfig.physicType, PLAYER_DETECTION, PLAYER_DETECTION_MASK, true);
	weaponGiver->addComponent<GetAWeaponOnCollision>(weaponType);
}

/// <summary>
/// Crea una caja roja que posee f�sicas y controlador de teclado
/// </summary>
/// <param name="pos">Posici�n</param>
/// <param name="vel">Velocidad (por defecto es cero)</param>
/// <param name="height">Altura en pixeles</param>
/// <param name="width">Anchura en pixeles</param>
/// <param name="rotation">Rotacion (por defecto es cero)</param>
/// <param name="physicType">Determina el tipo f�sico del objeto (STATIC, DYNAMIC, KINEMATIC)</param>
void PlayState::createBoxTest(const Config& entityConfig) {
	auto* box = createBasicEntity(entityConfig.pos, entityConfig.size, entityConfig.rotation, entityConfig.vel);

	auto* anim_controller = box->addComponent<AnimBlendGraph>();

	box->addComponent<BoxCollider>(entityConfig.physicType, entityConfig.col, entityConfig.colMask, entityConfig.isTrigger, entityConfig.friction, entityConfig.fixedRotation, entityConfig.rotation);
	//box->addComponent<CameraFollow>(box->getComponent<Transform>());

	if (entityConfig.physicType == 1 || entityConfig.physicType == 2)
		box->addComponent<KeyBoardCtrl>(map);
}

void PlayState::createElfShark(const Config& entityConfig) {
#pragma region ElfShark
	auto* elf1 = createBasicEntity(entityConfig.pos, entityConfig.size, entityConfig.rotation, entityConfig.vel);
	//auto* elf1 = manager_->addEntity();
	//Transform* t = elf1->addComponent<Transform>(Vector2D(sdlutils().width() * 1.6f, sdlutils().height() * 0.3f), Vector2D(0, 0), 180.0f, 180.0f, 0.0f);
	//elf1->addComponent<BoxCollider>(KINEMATIC, ENEMY, ENEMY_MASK);
	elf1->addComponent<BoxCollider>(entityConfig.physicType, entityConfig.col, entityConfig.colMask);
	AnimBlendGraph* elf1_anim_controller = elf1->addComponent<AnimBlendGraph>();
	elf1_anim_controller->addAnimation("idle", &sdlutils().images().at("Elf_Shark"), 1, 3, 1, 1, -1);
	elf1_anim_controller->addAnimation("attack", &sdlutils().images().at("Elf_Shark"), 1, 3, 3, 8, 0);
	elf1_anim_controller->addTransition("idle", "attack", "Attack", 1, false);
	elf1_anim_controller->addTransition("attack", "idle", "Attack", 0, true);
	auto* trigger_elf1 = elf1->addComponent<EnemyTrigger>(Vector2D(1000.0f, 600.0f));
	trigger_elf1->addTriggerComponent<ElfSharkAttack>(elf1);
	elf1->addComponent<Enemy_Health>(300, Vector2D(300, 20), build_sdlcolor(255, 0, 0, 255), 50);
#pragma endregion
}

void PlayState::createFlowerJellyHat(const Config& entityConfig) {
	auto* fjh1 = createBasicEntity(entityConfig.pos, entityConfig.size, entityConfig.rotation, entityConfig.vel);
	AnimBlendGraph* fjh1_anim_controller = fjh1->addComponent<AnimBlendGraph>();
	fjh1_anim_controller->addAnimation("idle", &sdlutils().images().at("Medusa"), 7, 6, 38, 8, -1);
	fjh1->addComponent<Enemy_Health>(300, Vector2D(300, 20), build_sdlcolor(255, 0, 0, 255), 50);
	fjh1->addComponent<BoxCollider>(entityConfig.physicType, entityConfig.col, entityConfig.colMask);
	fjh1->addComponent<ContactDamage>();
	fjh1->addComponent<JellyHatBehavior>(fjh1);
}



