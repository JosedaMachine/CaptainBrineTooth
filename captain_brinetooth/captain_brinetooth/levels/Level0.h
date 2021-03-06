#pragma once

#include "SDL.h"
#include <string>
#include <vector>
#include <map>
#include "box2d.h"
#include "../sdlutils/Texture.h"
#include "../sdlutils/SDLUtils.h"
#include "../ecs/Component.h"

#include "tmxlite/Map.hpp"
#include "tmxlite/Layer.hpp"
#include "tmxlite/TileLayer.hpp"
#include "tmxlite/Object.hpp"
#include "..//components/EnemyGenerator.h"

//#include "../components/BoxCollider.h"

using namespace std;

typedef int gid;

// Informacion sobre un tile individual
struct tile {
	Texture* sheet_;
	int x_, y_,	 //Coordenadas en el mundo
		tx_, ty_,	 //Coordenadas en la sprite sheet
		width_, height_;

	tile(Texture* tset, int x = 0, int y = 0, int tx = 0, int ty = 0, int w = 0, int h = 0);
	void draw();
};

struct object {
	float oX_, oY_, oWidth_, oHeight_;
	object(float oX, float oY, float oWidth, float oHeight);
};


class Level0 : public Component{
public:
	Level0(const string& name, std::shared_ptr<b2World> b2World);
	virtual ~Level0();
	virtual void init();
	void render() override;
	Vector2D getFilsCols() { return Vector2D(fils_, cols_); }
	Vector2D getTileSize() { return Vector2D(tile_height_, tile_width_); }
	void load(const string& path);

	void setPlayerPos();

	std::array<bool, 4> returnRoomCons() { return Cons; }

	/// <summary>
	/// Get the positions of the trigger at the center of the body
	/// </summary>
	/// <returns></returns>
	vector<tmx::Vector2f> getConPos() { return connectionPos; }


	vector<tmx::Vector2f> getConSize() { return connectionSize; }

	std::vector<vector<tmx::Vector2f>> getVerticesList(){
		return points;
	}

	void clearTileset();

	tmx::Vector2f getPlayerPos() { return playerPos; }
	
	vector<char> getConNames() {return connectionsNames; }

	tmx::Vector2f getEnd() { return endPos; }

	tmx::Vector2f getStorePos() { return storePos; }

	bool finalRoom() { return finalR; }

	void traveled() { finalR = false; }

	vector<tmx::Vector2f> getPescaPoints() { return pescaPos; }
	//Creo que es para que sea L value. Se lo tengo que preguntar a Samir
	Vector2D& getMaxCoordenates() { return (maxCoordenate); }

	void addProjectile(Entity* p) { projectiles.push_back(p); }
	
private:
	void spawnEnemies();

	void clearLevelVectors();

	string name_;
	int fils_, cols_,		// filas y columnas de 
		tile_width_, tile_height_;
	std::shared_ptr<b2World> b2World_ = nullptr;


	std::array<bool, 4> Cons;
	//Posici�n base del player
	tmx::Vector2f playerPos;

	map<gid, Texture*> tilesets_;	// tilesets usados para dibujar el Tiled Map
	vector<tile*> tiles_;			// tiles a dibujar en pantalla


	//Proyectiles
	vector<Entity*> projectiles;
	//Puntos para las colisiones del mapa
	vector<vector<tmx::Vector2f>> points;
	//Enemigos
	vector<tmx::Vector2f> enemiePos;
	vector<Entity*> enemigos;
	//Conexiones
	vector<tmx::Vector2f> connectionPos;
	vector<char> connectionsNames;
	vector<tmx::Vector2f> connectionSize;
	vector<tmx::Vector2f> pescaPos;

	tmx::Vector2f endPos;
	tmx::Vector2f bossPos;
	tmx::Vector2f storePos;

	bool finalR = false;

	Vector2D maxCoordenate = Vector2D(0,0);
	EnemyGenerator* generator;
};