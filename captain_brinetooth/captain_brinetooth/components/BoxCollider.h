#pragma once
#include <iostream>

#include "box2d.h"
#include <cmath>
#include "../utils/Vector2D.h"
#include "../ecs/Component.h"
#include "../ecs/Entity.h"
#include "Transform.h"
#include "../sdlutils/SDLUtils.h"
#include "../game/App.h"

//1 pixel(X) 0.0002645833 m
//then n pixels = n * 0.0002645833;

enum TYPE { STATIC, DYNAMIC, KINEMATIC };

class BoxCollider : public Component {
public:
	BoxCollider(int typeAux = TYPE::STATIC, const uint16& collisionLayer = 0x0001, const uint16& collisionMask = 0xFFFF, bool isTriggerAux = false, float friction = 0.7f, bool fixedRotation = true, float rotation = 0.0f)
	{
		type = typeAux;
		isTrigger = isTriggerAux;
		friction_ = friction;
		tr_ = nullptr;
		colLay_ = collisionLayer;
		colMask_ = collisionMask;
		fixedRotation_ = fixedRotation;
		rotation_ = rotation;
	}

	virtual ~BoxCollider() {
		body->SetEnabled(false);
	}

	void init() override {
		tr_ = entity_->getComponent<Transform>();
		assert(tr_ != nullptr);
		pos = tr_->getPos();

		size = Vector2D(tr_->getW() / sdlutils().getPPM() , tr_->getH() / sdlutils().getPPM());
		world = entity_->getWorld();

		b2BodyDef bodyDef;
		switch (type) {
		case TYPE::STATIC:
			bodyDef.type = b2_staticBody;
			break;

		case TYPE::DYNAMIC:
			bodyDef.type = b2_dynamicBody;
			break;

		case TYPE::KINEMATIC:
			bodyDef.type = b2_kinematicBody;
			break;

		default:
			bodyDef.type = b2_staticBody;
			break;
		}
		
		bodyDef.position.Set(pos.getX() / sdlutils().getPPM(), pos.getY() / sdlutils().getPPM());
		bodyDef.angle = rotation_;

		//Stores the entity in the body for future reference in collisions
		bodyDef.userData.pointer = reinterpret_cast<uintptr_t>(entity_);

		//Make the body
		body = world->CreateBody(&bodyDef);

		b2PolygonShape boxShape;
		boxShape.SetAsBox(size.getX() / 2.0f, size.getY() / 2.0f);

		b2FixtureDef fixtureDef;
		//Que el cubo real tenga la misma forma que la definicion
		fixtureDef.shape = &boxShape;
		//No puede ser la densidad 0 para un objeto dinamico
		fixtureDef.density = 1.0f;
		fixtureDef.friction = friction_;
		fixtureDef.isSensor = isTrigger;

		body->SetFixedRotation(fixedRotation_);
		
		fixtureDef.filter.categoryBits = colLay_; // tag para determinar capa de colision
		fixtureDef.filter.maskBits = colMask_; // con que capas de colision se hace pues eso, colision

		fixture = body->CreateFixture(&fixtureDef);
	}

	void render() override {
		if (sdlutils().getDebug()){
			SDL_SetRenderDrawColor(sdlutils().renderer(), 0, 255, 0, 255);
			float x = round((body->GetPosition().x * sdlutils().getPPM()) - tr_->getW() / 2.0f) - App::camera.x;
			float y = round((body->GetPosition().y * sdlutils().getPPM()) - tr_->getH() / 2.0f) - App::camera.y;
			SDL_Rect dest = build_sdlrect(x, y, tr_->getW(), tr_->getH());

			SDL_RenderDrawRect(sdlutils().renderer(), &dest);
		}
	}

	void setSpeed(Vector2D speed) {
		b2Vec2 vel = body->GetLinearVelocity();
		vel.x = speed.getX(); vel.y = speed.getY();
		body->SetLinearVelocity(vel);

		
	}

	/// <summary>
	/// Aplica una fuerza gradual en la direccion indicada
	/// </summary>
	/// <param name="dir">Vector2D de dirección</param>
	/// <param name="force">Fuerza aplicada</param>
	void applyForce(Vector2D dir, float force = 1.0f){
		dir.normalize();
		body->ApplyForce(b2Vec2(dir.getX() * force, dir.getY() * force), body->GetWorldCenter(), true);
	}

	/// <summary>
	/// Aplica una fuerza inmediata en la direccion indicada
	/// </summary>
	/// <param name="dir">Vector2D de dirección</param>
	/// <param name="force">Fuerza aplicada</param>
	void applyLinearForce(Vector2D dir, float force = 1.0f){
		dir.normalize();
		body->ApplyLinearImpulse(b2Vec2(dir.getX()* force, dir.getY()* force) , body->GetWorldCenter(), true);
	}

	void update() override {
		actRenderPos();
		tr_->setRot((body->GetAngle() * (180.0f)) / M_PI);

		/*�Custom method to detect collisions and delete a body
		b2ContactEdge* b;

		b = body->GetContactList();

		if (b != nullptr && entra == false) {
			std::cout << "Collided";
			world->DestroyBody(b->contact->
			A()->GetBody());
			entra = 1;
		}*/

	}

	inline b2Body* getBody() const {
		return body;
	}

	inline b2Fixture* getFixture() const {
		return fixture;
	}

	/// <summary>
	/// Actualiza las coordenadas físicas de un cuerpo en base a las actuales
	/// </summary>
	/// <param name="x">in pixels</param>
	/// <param name="y">in pixels</param>
	inline void actPhyscialPos(int x, int y) {
		int x_ = x / sdlutils().getPPM();
		int y_ = y / sdlutils().getPPM();

		b2Vec2 toMove(x_, y_);
		body->SetTransform(body->GetPosition() + toMove, body->GetAngle());

		actRenderPos();
	}

	/// <summary>
	/// Cambia las coordenadas físicas de un cuerpo y su rotación
	/// </summary>
	/// <param name="x">in pixels</param>
	/// <param name="y">in pixels</param>
	/// <param name="rotation">in degrees</param>
	inline void setPhysicalTransform(int x, int y, float degrees) {
		int x_ = x / sdlutils().getPPM();
		int y_ = y / sdlutils().getPPM();
		float newRot = (degrees * M_PI) / (180.0f);

		b2Vec2 toMove(x_, y_);
		body->SetTransform(toMove, newRot);
		
		actRenderPos();
	}
	inline Vector2D getPhysicalPos()
	{
		b2Vec2 physpos= body->GetPosition();
		Vector2D position;
		position.set(Vector2D(physpos.x, pos.getY));
		return position;
	}
	inline void actRenderPos(){
		tr_->getPos().set(round((body->GetPosition().x * sdlutils().getPPM()) - tr_->getW() / 2.0f), round((body->GetPosition().y * sdlutils().getPPM()) - tr_->getH() / 2.0f));
	}


private:
	Transform* tr_;
	Vector2D pos, size;
	int type;
	bool isTrigger;
	float rotation_;
	float friction_;
	uint16 colLay_;
	uint16 colMask_;

	//bool entra = 0;
	bool fixedRotation_;
	std::shared_ptr<b2World> world = nullptr;
	b2Body* body = nullptr;
	b2Fixture* fixture = nullptr;

};

