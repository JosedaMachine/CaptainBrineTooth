// This file is part of the course TPV2@UCM - Samir Genaim

#include "Manager.h"

#include <algorithm>

Manager::Manager(App* a, std::shared_ptr<b2World> mundo, SoundManager* snd_, bool* pop) {
	app = a;
	snd = snd_;
	world_ = mundo;
	popped = pop;
}

Manager::~Manager() {
	for (auto e : entities_) {
		delete e;
	}
}

void Manager::refresh() {
	// remove dead entities from the list of entities
	entities_.erase( //
		std::remove_if( //
			entities_.begin(), //
			entities_.end(), //
			[](const Entity* e) { //
				if (e->isActive()) {
					return false;
				}
				else {
					delete e;
					return true;
				}
			}), //
		entities_.end());
}


void Manager::update() {
	auto n = entities_.size();
	for (auto i = 0u; i < n && !(*popped); i++)
		entities_[i]->update();
}

void Manager::render() {
	auto n = entities_.size();
	for (auto i = 0u; i < n; i++)
		entities_[i]->render();
}
