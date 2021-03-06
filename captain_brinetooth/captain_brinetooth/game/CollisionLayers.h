#pragma once

#include "b2_types.h"

//This�Enamespace sets the different physic layers of the game

namespace ColLayers {

	//This are the different collision layers, they have values in powers of 2 in hexadecimal
	//IMPORTANT: the number of Collision Layers must be the same than the Collision Masks
	//Every mask corresponds with one collision layer and only one
	enum collisionLayers {
		DEFAULT = 0x0001,
		PLAYER = 0x0002,
		PLAYER_ATTACK = 0x0004,
		ENEMY = 0x0008,
		ENEMY_ATTACK = 0x0010,
		PLAYER_DETECTION = 0x0020,
		GROUND = 0x0040,
		PLAYER_DASH = 0X0080,
		CEBO_GANCHO = 0X0100,
		PLAYER_JUMP = 0X0200,
	};

	//Collision masks. Decide what each collision layer can collide with
	const uint16 DEFAULT_MASK = DEFAULT | GROUND | PLAYER | PLAYER_ATTACK | ENEMY | ENEMY_ATTACK | PLAYER_DASH | CEBO_GANCHO | PLAYER_JUMP;
	const uint16 PLAYER_MASK = DEFAULT | GROUND | ENEMY | ENEMY_ATTACK | PLAYER_DETECTION ;
	const uint16 PLAYER_ATTACK_MASK = DEFAULT | GROUND | ENEMY;
	const uint16 ENEMY_MASK = DEFAULT | GROUND | PLAYER | PLAYER_ATTACK;
	const uint16 ENEMY_ATTACK_MASK = DEFAULT | GROUND | PLAYER;
	const uint16 PLAYER_JUMP_MASK = DEFAULT | GROUND;
	const uint16 PLAYER_DETECTION_MASK = PLAYER | PLAYER_DASH;
	const uint16 GROUND_MASK = DEFAULT | PLAYER | PLAYER_ATTACK | ENEMY | ENEMY_ATTACK | PLAYER_DASH | PLAYER_JUMP;
	const uint16 PLAYER_DASH_MASK = DEFAULT | GROUND | PLAYER_DETECTION;
	const uint16 CEBO_GANCHO_MASK = DEFAULT;
}