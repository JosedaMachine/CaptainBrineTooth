#include "Crab.h"

#include <iostream>
#include "../game/CollisionLayers.h"
#include "AnimBlendGraph.h"

using namespace ColLayers;

void Crab::init() {
	tr_ = entity_->getComponent<Transform>();
	assert(tr_ != nullptr);
	anim_ = entity_->getComponent<AnimBlendGraph>();
}

void Crab::update() {
	if (ih().keyDownEvent()) {
		if (ih().isKeyDown(SDL_SCANCODE_E)) {

			//Player not attacking or in combo
			if (CURRENT_STATUS == STATUS::Iddle && stoppedAttackingTime + timeBeforeNextAttackStarts < sdlutils().currRealTime()) {
				std::cout << "Crab attack\n";

				//Set player as sawing
				CURRENT_STATUS = STATUS::OnAnimationLock;
				CURRENT_ATTACK = ATTACKS::Attack1;

				//Activate attack animation + sawing on attack
				if (anim_->searchParamValue("crab_att") != -1)
					anim_->setParamValue("crab_att", 1);

				//
				creaTrigger(100);

				//Time control variables
				stoppedSawTime = sdlutils().currRealTime();
			}
			else if (CURRENT_STATUS == STATUS::OnCombo) {
				switch (CURRENT_ATTACK)
				{
				case ATTACKS::Attack1:
					std::cout << "Attack 2 Initiated\n";
					CURRENT_STATUS = STATUS::Sawing;
					CURRENT_ATTACK = ATTACKS::Attack2;

					if (anim_->searchParamValue("crab_att") != -1)
						anim_->setParamValue("crab_att", 2);

					sawActivationTime = sdlutils().currRealTime();
					break;
					
				case ATTACKS::Attack2:
					std::cout << "Attack 3 Initiated\n";
					CURRENT_STATUS = STATUS::OnAnimationLock;
					CURRENT_ATTACK = ATTACKS::Attack3;

					if (anim_->searchParamValue("crab_att") != -1)
						anim_->setParamValue("crab_att", 3);

					//
					creaTrigger(100);

					stoppedSawTime = sdlutils().currRealTime();
					break;
				case ATTACKS::Attack3:
					std::cout << "Crab attack\n";

					//Set player as sawing
					CURRENT_STATUS = STATUS::OnAnimationLock;
					CURRENT_ATTACK = ATTACKS::Attack1;

					//Activate attack animation + sawing on attack
					if (anim_->searchParamValue("crab_att") != -1)
						anim_->setParamValue("crab_att", 1);

					//
					creaTrigger(100);

					//Time control variables
					stoppedSawTime = sdlutils().currRealTime();
					break;
				default:
					break;
				}
			}
		}
	}

	//Check out of input cases
	if (CURRENT_STATUS == STATUS::Sawing && sawActivationTime + maxHoldTime < sdlutils().currRealTime()) {
		//Deactivate chainsaw
		std::cout << "STOPPED STABBING\n";
		CURRENT_STATUS = STATUS::OnAnimationLock;

		if (anim_->searchParamValue("crab_att") != -1)
			anim_->setParamValue("crab_att", 0);

		stoppedSawTime = sdlutils().currRealTime();
	}
	else if (CURRENT_STATUS == STATUS::OnAnimationLock && stoppedSawTime + animationLockTime < sdlutils().currRealTime()) {
		//Deactivate animation lock
		std::cout << "STOPPED ANIMATION\n";
		CURRENT_STATUS = STATUS::OnCombo;

		if (trigger != nullptr) {
			trigger->setActive(false);
			trigger = nullptr;
		}

		if (anim_->searchParamValue("crab_att") != -1)
			anim_->setParamValue("crab_att", 0);

		comboActivationTime = sdlutils().currRealTime();
	}
	else if (CURRENT_STATUS == STATUS::OnCombo && comboActivationTime + maxComboPanningTime < sdlutils().currRealTime()) {
		//Deactivate combo availability
		std::cout << "STOPPED COMBO\n";
		CURRENT_STATUS = STATUS::Iddle;
		CURRENT_ATTACK = ATTACKS::NotAttacking;

		stoppedAttackingTime = sdlutils().currRealTime();
	}

	//Updating the trigger's position
	if (trigger != nullptr) {
		if (entity_->getComponent<PlayerController>()->getFlip()) trigger->getComponent<BoxCollider>()->getBody()->SetTransform(b2Vec2((tr_->getPos().getX() + (-triggerOffSetX + entity_->getComponent<Transform>()->getW())) / sdlutils().getPPM(),
			(tr_->getPos().getY() + triggerOffSetY) / sdlutils().getPPM()), 0.0f);
		else trigger->getComponent<BoxCollider>()->getBody()->SetTransform(b2Vec2((tr_->getPos().getX() + triggerOffSetX) / sdlutils().getPPM(),
			(tr_->getPos().getY() + triggerOffSetY) / sdlutils().getPPM()), 0.0f);
	}

	if (currentlyStabbing && stabActivationTime + stabTriggerTime < sdlutils().currRealTime()) {
		if (trigger != nullptr) {
			trigger->setActive(false);
			trigger = nullptr;
		}
		currentlyStabbing = false;
	}

	//Comprobamos si hay que spawnear una estocada
	if (CURRENT_STATUS == Sawing && stabActivationTime + timeBetweenStabs < sdlutils().currRealTime()) {
		std::cout << "Crab Punch\n";

		//
		creaTrigger(100);

		stabActivationTime = sdlutils().currRealTime();
		currentlyStabbing = true;
	}
}

void Crab::creaTrigger(int damage) {
	//Activate attack trigger
	trigger = entity_->getMngr()->addEntity();
	if (entity_->getComponent<PlayerController>()->getFlip()) trigger->addComponent<Transform>(tr_->getPos() + Vector2D(-triggerOffSetX + entity_->getComponent<Transform>()->getW(), triggerOffSetY),
		Vector2D(0, 0), triggerWidth, triggerHeight, 0.0f);
	else trigger->addComponent<Transform>(tr_->getPos() + Vector2D(triggerOffSetX, triggerOffSetY),
		Vector2D(0, 0), triggerWidth, triggerHeight, 0.0f);
	/*anim_controller = trigger->addComponent<AnimBlendGraph>();
	anim_controller->addAnimation("iddle", &sdlutils().images().at("fondo"), 1, 1, 1, 1, 1);*/
	trigger->addComponent<BoxCollider>(TYPE::KINEMATIC, PLAYER_ATTACK, PLAYER_ATTACK_MASK, true);
	trigger->addComponent<WeaponDamageDetection>(damage);
}