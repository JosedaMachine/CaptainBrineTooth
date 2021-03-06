#include "Hammer.h"

#include <iostream>
#include "../game/CollisionLayers.h"

using namespace ColLayers;

void Hammer::init() {
	tr_ = entity_->getComponent<Transform>();
	assert(tr_ != nullptr);
	anim_ = entity_->getComponent<AnimBlendGraph>();
}

void Hammer::update() {
	if (entity_->hasComponent<Player_Health>()) {
		if (!entity_->getComponent<PlayerController>()->isPlayerDashing() && !entity_->getComponent<Player_Health>()->getPlayerIsDying()) {
			if (ih().mouseButtonEvent() || ih().keyUpEvent()) {
				if (ih().getMouseButtonState(InputHandler::MOUSEBUTTON::LEFT) || ih().isKeyDown(SDL_CONTROLLER_BUTTON_X)) {

					//Player not attacking or in combo
					if (CURRENT_STATUS == STATUS::Iddle) {
						std::cout << "Martillazo! el primero\n";

						//Set player as sawing
						CURRENT_STATUS = STATUS::OnAnimationLock;
						CURRENT_ATTACK = ATTACKS::Attack1;

						entity_->getMngr()->getSoundMngr()->playSoundEffect("hammer_lateral", 0);
						//Activate attack animation + sawing on attack
						if (anim_->getParamIndex("hammer_att") != -1)
							anim_->setParamValue("hammer_att", 1);

						isAttacking = true;
						creaTrigger(65);

						//Time control variables
						stoppedSawTime = sdlutils().currRealTime();
					}
					//Player already attacked once
					else if (CURRENT_STATUS == STATUS::OnCombo && CURRENT_ATTACK == ATTACKS::Attack1) {
						std::cout << "Martillazo! el segundo\n";

						CURRENT_STATUS = STATUS::OnAnimationLock;
						CURRENT_ATTACK = ATTACKS::Attack2;
						entity_->getMngr()->getSoundMngr()->playSoundEffect("hammer_down", 0);


						if (anim_->getParamIndex("hammer_att") != -1)
							anim_->setParamValue("hammer_att", 2);

						isAttacking = true;
						creaTrigger(70);

						stoppedSawTime = sdlutils().currRealTime();
					}
					else if (CURRENT_STATUS == STATUS::OnCombo && CURRENT_ATTACK == ATTACKS::Attack2) {
						std::cout << "Martillazo! el primero\n";

						//Set player as sawing
						CURRENT_STATUS = STATUS::OnAnimationLock;
						CURRENT_ATTACK = ATTACKS::Attack1;
						entity_->getMngr()->getSoundMngr()->playSoundEffect("hammer_lateral", 0);

						//Activate attack animation + sawing on attack
						if (anim_->getParamIndex("hammer_att") != -1)
							anim_->setParamValue("hammer_att", 1);

						isAttacking = true;
						creaTrigger(65);

						//Time control variables
						stoppedSawTime = sdlutils().currRealTime();
					}
				}
			}
			//Check out of input cases
			if (CURRENT_STATUS == STATUS::OnAnimationLock && stoppedSawTime + animationLockTime < sdlutils().currRealTime()) {
				//Deactivate animation lock
				std::cout << "STOPPED ANIMATION\n";
				CURRENT_STATUS = STATUS::OnCombo;
				comboActivationTime = sdlutils().currRealTime();

				if (trigger != nullptr) {
					trigger->setActive(false);
					trigger = nullptr;
				}
			}
			else if (CURRENT_STATUS == STATUS::OnCombo && comboActivationTime + maxComboPanningTime < sdlutils().currRealTime()) {
				//Deactivate combo availability
				std::cout << "STOPPED COMBO\n";
				CURRENT_STATUS = STATUS::Iddle;
				CURRENT_ATTACK = ATTACKS::NotAttacking;

				isAttacking = false;
				anim_->setParamValue("hammer_att", 0);
			}

			/*if (anim_->isComplete() && anim_->getParamIndex("hammer_att") != ATTACKS::NotAttacking && anim_->getParamIndex("hammer_att") == CURRENT_ATTACK) {
				anim_->setParamValue("hammer_att", 0);
			}*/

			//Updating the trigger's position
			if (trigger != nullptr) {
				if (anim_->isFlipX()) trigger->getComponent<BoxCollider>()->getBody()->SetTransform(b2Vec2((tr_->getPos().getX() + (-triggerOffSetX)) / sdlutils().getPPM(),
					(tr_->getPos().getY() + triggerOffSetY) / sdlutils().getPPM()), 0.0f);
				else trigger->getComponent<BoxCollider>()->getBody()->SetTransform(b2Vec2((tr_->getPos().getX() + triggerOffSetX) / sdlutils().getPPM(),
					(tr_->getPos().getY() + triggerOffSetY) / sdlutils().getPPM()), 0.0f);
			}
		}
		else {
			if (CURRENT_STATUS != STATUS::Iddle) {
				isAttacking = false;
				if (trigger != nullptr) {
					trigger->setActive(false);
					trigger = nullptr;
				}
				CURRENT_STATUS = STATUS::Iddle;
				CURRENT_ATTACK = ATTACKS::NotAttacking;
			}
		}
	}
	//else de los weapon **NECESARIO PARA EL TUTORIAL**
	#pragma region Tutorial
	else {
		if (!entity_->getComponent<PlayerController>()->isPlayerDashing()) {
			if (ih().mouseButtonEvent() || ih().keyUpEvent()) {
				if (ih().getMouseButtonState(InputHandler::MOUSEBUTTON::LEFT) || ih().isKeyDown(SDL_CONTROLLER_BUTTON_X)) {

					//Player not attacking or in combo
					if (CURRENT_STATUS == STATUS::Iddle) {
						std::cout << "Martillazo! el primero\n";

						//Set player as sawing
						CURRENT_STATUS = STATUS::OnAnimationLock;
						CURRENT_ATTACK = ATTACKS::Attack1;

						entity_->getMngr()->getSoundMngr()->playSoundEffect("hammer_lateral", 0);
						//Activate attack animation + sawing on attack
						if (anim_->getParamIndex("hammer_att") != -1)
							anim_->setParamValue("hammer_att", 1);

						isAttacking = true;
						creaTrigger(65);

						//Time control variables
						stoppedSawTime = sdlutils().currRealTime();
					}
					//Player already attacked once
					else if (CURRENT_STATUS == STATUS::OnCombo && CURRENT_ATTACK == ATTACKS::Attack1) {
						std::cout << "Martillazo! el segundo\n";

						CURRENT_STATUS = STATUS::OnAnimationLock;
						CURRENT_ATTACK = ATTACKS::Attack2;
						entity_->getMngr()->getSoundMngr()->playSoundEffect("hammer_down", 0);


						if (anim_->getParamIndex("hammer_att") != -1)
							anim_->setParamValue("hammer_att", 2);

						isAttacking = true;
						creaTrigger(70);

						stoppedSawTime = sdlutils().currRealTime();
					}
					else if (CURRENT_STATUS == STATUS::OnCombo && CURRENT_ATTACK == ATTACKS::Attack2) {
						std::cout << "Martillazo! el primero\n";

						//Set player as sawing
						CURRENT_STATUS = STATUS::OnAnimationLock;
						CURRENT_ATTACK = ATTACKS::Attack1;
						entity_->getMngr()->getSoundMngr()->playSoundEffect("hammer_lateral", 0);

						//Activate attack animation + sawing on attack
						if (anim_->getParamIndex("hammer_att") != -1)
							anim_->setParamValue("hammer_att", 1);

						isAttacking = true;
						creaTrigger(65);

						//Time control variables
						stoppedSawTime = sdlutils().currRealTime();
					}
				}
			}
			//Check out of input cases
			if (CURRENT_STATUS == STATUS::OnAnimationLock && stoppedSawTime + animationLockTime < sdlutils().currRealTime()) {
				//Deactivate animation lock
				std::cout << "STOPPED ANIMATION\n";
				CURRENT_STATUS = STATUS::OnCombo;
				comboActivationTime = sdlutils().currRealTime();

				if (trigger != nullptr) {
					trigger->setActive(false);
					trigger = nullptr;
				}
			}
			else if (CURRENT_STATUS == STATUS::OnCombo && comboActivationTime + maxComboPanningTime < sdlutils().currRealTime()) {
				//Deactivate combo availability
				std::cout << "STOPPED COMBO\n";
				CURRENT_STATUS = STATUS::Iddle;
				CURRENT_ATTACK = ATTACKS::NotAttacking;

				isAttacking = false;
				anim_->setParamValue("hammer_att", 0);
			}

			/*if (anim_->isComplete() && anim_->getParamIndex("hammer_att") != ATTACKS::NotAttacking && anim_->getParamIndex("hammer_att") == CURRENT_ATTACK) {
				anim_->setParamValue("hammer_att", 0);
			}*/

			//Updating the trigger's position
			if (trigger != nullptr) {
				if (anim_->isFlipX()) trigger->getComponent<BoxCollider>()->getBody()->SetTransform(b2Vec2((tr_->getPos().getX() + (-triggerOffSetX)) / sdlutils().getPPM(),
					(tr_->getPos().getY() + triggerOffSetY) / sdlutils().getPPM()), 0.0f);
				else trigger->getComponent<BoxCollider>()->getBody()->SetTransform(b2Vec2((tr_->getPos().getX() + triggerOffSetX) / sdlutils().getPPM(),
					(tr_->getPos().getY() + triggerOffSetY) / sdlutils().getPPM()), 0.0f);
			}
		}
		else {
			if (CURRENT_STATUS != STATUS::Iddle) {
				isAttacking = false;
				if (trigger != nullptr) {
					trigger->setActive(false);
					trigger = nullptr;
				}
				CURRENT_STATUS = STATUS::Iddle;
				CURRENT_ATTACK = ATTACKS::NotAttacking;
			}
		}
	}
#pragma endregion
}

void Hammer::creaTrigger(int damage) {
	trigger = entity_->getMngr()->addEntity();
	if (anim_->isFlipX()) trigger->addComponent<Transform>(tr_->getPos() + Vector2D(-triggerOffSetX, triggerOffSetY),
		Vector2D(0, 0), triggerWidth * entity_->getComponent<Transform>()->getW() / 100.0f, triggerHeight, 0.0f);
	else trigger->addComponent<Transform>(tr_->getPos() + Vector2D(triggerOffSetX, triggerOffSetY),
		Vector2D(0, 0), triggerWidth * entity_->getComponent<Transform>()->getW() / 100.0f, triggerHeight, 0.0f);
	trigger->addComponent<BoxCollider>(TYPE::KINEMATIC, PLAYER_ATTACK, PLAYER_ATTACK_MASK, true);
	trigger->addComponent<WeaponDamageDetection>(damage);
}