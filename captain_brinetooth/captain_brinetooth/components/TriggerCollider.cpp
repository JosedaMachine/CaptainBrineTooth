#include "TriggerCollider.h"

#define _CRTDBG_MAP_ALLOC
#include<iostream>
#include <crtdbg.h>
#ifdef _DEBUG
#define DEBUG_NEW new(_NORMAL_BLOCK, __FILE__, __LINE__)
#define new DEBUG_NEW
#endif

void TriggerCollider::init()
{
	entitytr_ = entity_->getComponent<Transform>();
	assert(entitytr_ != nullptr);
	//Inicializamos las componentes del trigger
	trigger = entity_->getMngr()->addEntity(); //TRIGGER DEL ENEMIGO (rango de deteccion)
	Vector2D triggerpos = Vector2D(entitytr_->getPos().getX() + entitytr_->getW() * offsetTrigger_.getX(), entitytr_->getPos().getY() + entitytr_->getH() * offsetTrigger_.getY());
	trigger->addComponent<Transform>(triggerpos, Vector2D(0, 0), triggersize_.getX(), triggersize_.getY(), 0.0f);
	trigger->addComponent<BoxCollider>(DYNAMIC, colLayer_, colMask_, true);
}

void TriggerCollider::update()
{
	auto& pos = entity_->getComponent<BoxCollider>()->getBody()->GetTransform().p;
	trigger->getComponent<BoxCollider>()->getBody()->SetTransform(b2Vec2(pos.x + offsetTrigger_.getX(),pos.y + offsetTrigger_.getY()), 0.0f);
}

Entity* TriggerCollider::getParent()
{
	return entity_;
}

Entity* TriggerCollider::getTriggerEntity()
{
	return trigger;
}

const std::string& TriggerCollider::getID()
{
	return id_;
}
