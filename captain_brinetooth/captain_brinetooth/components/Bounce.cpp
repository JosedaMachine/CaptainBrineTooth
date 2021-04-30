#include "Bounce.h"

Bounce::Bounce() :
	tr_(nullptr) {
}

Bounce::~Bounce()
{
}

void Bounce::init() {
	tr_ = entity_->getComponent<Transform>();
	assert(tr_ != nullptr);
}

void Bounce::update() {
	auto& pos = tr_->getPos();
	auto& vel = tr_->getVel();
	auto w = tr_->getW();
	auto h = tr_->getH();

	if (pos.getX() < 0) {
		pos.setX(0.0);
		vel.setX(-vel.getX());
	}
	else if (pos.getX() + w > sdlutils().width()) {
		pos.setX(sdlutils().width() - w);
		vel.setX(-vel.getX());
	}

	if (pos.getY() < 0) {
		pos.setY(0.0);
		vel.setY(-vel.getY());
	}
	else if (pos.getY() + h > sdlutils().height()) {
		pos.setY(sdlutils().height() - h);
		vel.setY(-vel.getY());

	}
}
