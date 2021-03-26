// This file is part of the course TPV2@UCM - Samir Genaim

#pragma once

#include "../ecs/Component.h"
#include "../sdlutils/Texture.h"
#include "../game/Game.h"
#include "Transform.h"

class Image: public Component {
public:
	Image(Texture *tex) :
			tr_(nullptr), //
			tex_(tex) //
	{
	}
	virtual ~Image() {
	}

	void init() override {
		tr_ = entity_->getComponent<Transform>();
		assert(tr_ != nullptr);
	}

	void render() override {
		//Vector2D actpos;
		//actpos.setX(tr_->getPos().getX() - Game::camera.x);
		//actpos.setY(tr_->getPos().getY() - Game::camera.y);

		//Vector2D actpos(tr_->getPos().getX(), tr_->getPos().getY());
		SDL_Rect dest = build_sdlrect(tr_->getPos(), tr_->getW(), tr_->getH());

		tex_->render(dest, tr_->getRot());
	}

	Texture* getTexture() { return tex_; }

private:
	Transform *tr_;
	Texture *tex_;
};

