﻿#pragma once

#include "../ecs/Component.h"
#include "../ecs/Entity.h"
#include "Enemy_Health.h"
#include <iostream>


//This component functions as an example of a component that can be used to trigger an effect when said entity collides with something

class WeaponDamageDetection : public Component {
public:
	WeaponDamageDetection(int damageToApply) : damageToApply_(damageToApply) {
	}
	virtual ~WeaponDamageDetection() {
	}

	void init() override {
		entity_->setCollisionMethod(ContactEnemy);
	}

	//Metodo de ejemplo que se llama al colisionar
	//IMPORTANTE: debe de ser estatico
	static void ContactEnemy(b2Contact* contact) {
		//Llamar a otro metodo
		Entity* enemy = (Entity*)contact->GetFixtureA()->GetBody()->GetUserData().pointer;

		if (enemy != nullptr)
			if (enemy->getComponent<Enemy_Health>() != nullptr) {
				Entity* entidad = (Entity*)contact->GetFixtureB()->GetBody()->GetUserData().pointer;

				if (entidad != nullptr) {
					entidad->getComponent<WeaponDamageDetection>()->ApplyDamage(enemy);
				}
			}	
			else //Si el cosa es un enemigo que tiene el componente vida, quitar vida
				std::cout << "OHHH NOOO Perdi� vida\n";
	}

	//Aplica danio al enemigo
	void ApplyDamage(Entity* enemy) {
		enemy->getComponent<Enemy_Health>()->loseLife(damageToApply_);
	}
private:
	int damageToApply_;
};