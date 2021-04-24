#include "EnemyGenerator.h"


EnemyGenerator::EnemyGenerator()
{

}

EnemyGenerator::~EnemyGenerator()
{

}

void EnemyGenerator::generateRandomEnemy(Vector2D pos)
{
	int rnd = sdlutils().rand().teCuoto(0, NUM_ENEMIES);
	switch (rnd)
	{
	case 0:
		generateElfShark(pos);
		break;
	case 1:
		generateMedusa(pos);
		break;
	default:
		break;
	}
}

void EnemyGenerator::generateFringeHead(Vector2D pos)
{

}

void EnemyGenerator::generateMedusa(Vector2D pos)
{
	Config flowerJellyHat{};
	flowerJellyHat.pos = pos;
	flowerJellyHat.vel = Vector2D(0, 0);
	flowerJellyHat.size = Vector2D(100.0f, 100.0f);
	flowerJellyHat.friction = 0.2f;
	flowerJellyHat.physicType = DYNAMIC;
	flowerJellyHat.fixedRotation = true;
	flowerJellyHat.rotation = 0.0f;
	flowerJellyHat.col = ENEMY;
	flowerJellyHat.colMask = ENEMY_MASK;

	auto* fjh1 = createBasicEntity(flowerJellyHat.pos, flowerJellyHat.size, flowerJellyHat.rotation, flowerJellyHat.vel);
	AnimBlendGraph* fjh1_anim_controller = fjh1->addComponent<AnimBlendGraph>();
	fjh1_anim_controller->addAnimation("idle", &sdlutils().images().at("Medusa"), 7, 6, 38, 8, -1);
	fjh1->addComponent<Enemy_Health>(300, Vector2D(300, 20), build_sdlcolor(255, 0, 0, 255), 50);
	fjh1->addComponent<BoxCollider>(flowerJellyHat.physicType, flowerJellyHat.col, flowerJellyHat.colMask);
	fjh1->addComponent<JellyHatBehavior>(fjh1);
	
}

void EnemyGenerator::generateElfShark(Vector2D pos)
{
	Config elfShark{};
	elfShark.pos = pos;
	elfShark.vel = Vector2D(0, 0);
	elfShark.size = Vector2D(180.0f, 180.0f);
	elfShark.friction = 0.2f;
	elfShark.physicType = KINEMATIC;
	elfShark.fixedRotation = true;
	elfShark.rotation = 0.0f;
	elfShark.col = ColLayers::collisionLayers::ENEMY;
	elfShark.colMask = ColLayers::ENEMY_MASK;



	auto* elf1 = createBasicEntity(elfShark.pos, elfShark.size, elfShark.rotation, elfShark.vel);
	//auto* elf1 = manager_->addEntity();
	//Transform* t = elf1->addComponent<Transform>(Vector2D(sdlutils().width() * 1.6f, sdlutils().height() * 0.3f), Vector2D(0, 0), 180.0f, 180.0f, 0.0f);
	//elf1->addComponent<BoxCollider>(KINEMATIC, ENEMY, ENEMY_MASK);
	elf1->addComponent<BoxCollider>(elfShark.physicType, elfShark.col, elfShark.colMask);
	AnimBlendGraph* elf1_anim_controller = elf1->addComponent<AnimBlendGraph>();
	elf1_anim_controller->addAnimation("idle", &sdlutils().images().at("Elf_Shark"), 1, 3, 1, 1, -1);
	elf1_anim_controller->addAnimation("attack", &sdlutils().images().at("Elf_Shark"), 1, 3, 3, 8, 0);
	elf1_anim_controller->addTransition("idle", "attack", "Attack", 1, false);
	elf1_anim_controller->addTransition("attack", "idle", "Attack", 0, true);
	auto* trigger_elf1 = elf1->addComponent<EnemyTrigger>(Vector2D(1000.0f, 600.0f));
	trigger_elf1->addTriggerComponent<ElfSharkAttack>(elf1);
	elf1->addComponent<Enemy_Health>(300, Vector2D(300, 20), build_sdlcolor(255, 0, 0, 255), 50);
}

Entity* EnemyGenerator::createBasicEntity(const Vector2D& pos, const Vector2D& size, const float& rotation, const Vector2D& vel)
{
	auto* e = entity_->getMngr()->addEntity(); //entity_->manager_->addEntity();
	e->addComponent<Transform>(pos, vel, size.getX(), size.getY(), rotation);
	return e;
}
