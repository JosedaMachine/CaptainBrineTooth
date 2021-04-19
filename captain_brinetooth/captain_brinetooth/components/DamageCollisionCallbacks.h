#include "box2d.h"
#include "iostream"
#include "../sdlutils/SDLUtils.h"

class DamageCollisionCallbacks : public b2ContactListener
{
    void BeginContact(b2Contact* contact) {
        //Debugg
        if(sdlutils().getConsoleDebug()) std::cout << "TOUCHED WITH BEGIN CONTACT\n";

        //Codigo de ejemplo para obtener la entidad de los cuerpos que colisionan
        Entity* cosaA = (Entity*)contact->GetFixtureA()->GetBody()->GetUserData().pointer;

        //Llamamos a su metodo de colision, el cual debe de asinarse mediante un componente en la entidad
        //(Ver LoseLife para un ejemplo de componente de colision)
        if (cosaA != nullptr)
            cosaA->playCollisionMethod(contact);

        Entity* cosaB = (Entity*)contact->GetFixtureB()->GetBody()->GetUserData().pointer;
        if (cosaB != nullptr)
            cosaB->playCollisionMethod(contact);
    }

    void EndContact(b2Contact* contact) {
        //Debugg
        if (sdlutils().getDebug()) std::cout << "STOPPED TOUCHING\n";

        //Codigo de ejemplo para obtener la entidad de los cuerpos que colisionan
        Entity* cosaA = (Entity*)contact->GetFixtureA()->GetBody()->GetUserData().pointer;

        //Llamamos a su metodo de colision, el cual debe de asinarse mediante un componente en la entidad
        //(Ver LoseLife para un ejemplo de componente de colision)
        if(cosaA != nullptr)
            cosaA->playEndCollisionMethod(contact);

        Entity* cosaB = (Entity*)contact->GetFixtureB()->GetBody()->GetUserData().pointer;
        if(cosaB != nullptr)
            cosaB->playEndCollisionMethod(contact);
    }
};