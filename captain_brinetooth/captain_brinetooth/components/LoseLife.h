#pragma once

#include "../ecs/Component.h"
#include "../ecs/Entity.h"
#include "../components/Player_Health.h"
#include <iostream>


//This component functions as an example of a component that can be used to trigger an effect when said entity collides with something

class LoseLife : public Component {
public:
	LoseLife() {
	}
	virtual ~LoseLife() {
	}

	void init() override {
		//Asigna el metodo introducido en la entidad para que se pueda utilizar mediante el metodo PlayCollisionMethod
		//entity_->setCollisionMethod(LoseLifeMethod(this));
	}

	//Metodo de ejemplo que se llama al colisionar
	//IMPORTANTE: debe de ser estatico
	static void LoseLifeMethod(b2Contact* contact) {
		//Llamar a otro metodo
		Entity* cosaB = (Entity*)contact->GetFixtureB()->GetBody()->GetUserData().pointer;

		if (cosaB != nullptr)
			if (cosaB->getComponent<Player_Health>() != nullptr)
				cosaB->getComponent<Player_Health>()->loseLife();
			else //Si el cosa es un enemigo que tiene el componente vida, quitar vida

		std::cout << "OHHH NOOO Perdi� vida\n";
	}
};