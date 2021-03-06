// This file is part of the course TPV2@UCM - Samir Genaim

#pragma once

#include <initializer_list>
#include <vector>
#include <list>

#include "ecs.h"
#include "Entity.h"
#include "box2d.h"
#include "../ecs/SoundManager.h"
#include "../game/App.h"

class App;
class Manager {
public:
	Manager(App* a, std::shared_ptr<b2World> mundo, SoundManager* snd, bool* pop);
	virtual ~Manager();

	// entities
	Entity* addEntity() {
		Entity *e = new Entity(this, world_);
		if (e != nullptr) {
			e->resetGroups();
			entities_.emplace_back(e);
		}
		return e;
	}

	// handlers
	template<typename T>
	inline void setHandler(Entity *e) {
		hdlrs_[ecs::hdlrIdx<T>] = e;
	}

	template<typename T>
	inline Entity* getHandler() {
		return hdlrs_[ecs::hdlrIdx<T>];
	}

	inline const std::vector<Entity*>& getEnteties() {
		return entities_;
	}

	inline void desactiveEnteties(){
		for (auto e : entities_) e->setActive(false);
	}

	inline std::shared_ptr<b2World> getWorld() {
		return world_;
	}

	inline SoundManager* getSoundMngr()
	{
		return snd;
	}
	inline App* getApp()
	{
		return app;
	}

	void update();
	void render();
	void refresh();

private:

	bool* popped;
	std::shared_ptr<b2World> world_;
	std::vector<Entity*> entities_;
	std::array<Entity*, ecs::maxHdlr> hdlrs_;
	SoundManager* snd;
	App* app;
};

