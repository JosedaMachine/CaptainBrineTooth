#include "ContactDamage.h"
#include "../ecs/Entity.h"
#include "../utils/Vector2D.h"
#include "../ecs/Manager.h"
#include "Player_Health.h"

void ContactDamage::init()
{
	entity_->setCollisionMethod(callDamage);
}
void ContactDamage::callDamage(b2Contact* contact)
{
	Entity* bullet = (Entity*)contact->GetFixtureA()->GetBody()->GetUserData().pointer;
	if (bullet != nullptr) {
		if (bullet->getComponent<ContactDamage>() != nullptr)
			bullet->getComponent<ContactDamage>()->makeDamage();
		else {
			bullet = (Entity*)contact->GetFixtureB()->GetBody()->GetUserData().pointer;
			if (bullet != nullptr)
				bullet->getComponent<ContactDamage>()->makeDamage();
		}
	}
}
void ContactDamage::makeDamage()
{
	Entity* p = entity_->getMngr()->getHandler<Player>();

	Player_Health* pH = p->getComponent<Player_Health>(); //El jugador pierde una vida;

	pH->loseLife();
}
