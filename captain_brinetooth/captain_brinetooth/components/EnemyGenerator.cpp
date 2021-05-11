#include "EnemyGenerator.h"


EnemyGenerator::EnemyGenerator()
{

}

EnemyGenerator::~EnemyGenerator()
{

}

Entity* EnemyGenerator::generateRandomEnemy(Vector2D pos)
{
	int rnd = sdlutils().rand().teCuoto(0, NUM_ENEMIES);
	switch (rnd)
	{
	case 0:
		return generateElfShark(pos);
		//generateFringeHead(pos);
		break;
	case 1:
		return generateMedusa(pos);
		break;
	case 2:
		return generateFringeHead(pos);
		break;
	case 3:
		return generatePompeyWorm(pos);
		break;
	default:
		break;
	}
}

Entity* EnemyGenerator::generateFringeHead(Vector2D pos)
{
	Config fringeHead{};
	fringeHead.vel = Vector2D(0, 0);
	fringeHead.size = Vector2D(60.0f, 60.0f);
	fringeHead.friction = 0;
	fringeHead.physicType = DYNAMIC;
	fringeHead.fixedRotation = true;
	fringeHead.rotation = 0.0f;
	fringeHead.col = ENEMY;
	fringeHead.colMask = ENEMY_MASK;

	auto* enemy = createBasicEntity(pos, fringeHead.size, fringeHead.rotation, fringeHead.vel);
	BoxCollider* collider = enemy->addComponent<BoxCollider>(fringeHead.physicType, fringeHead.col, fringeHead.colMask, false, 0.7f,
		true, 0.0f, Vector2D(), Vector2D(), 100000);
	AnimBlendGraph* anim_controller = enemy->addComponent<AnimBlendGraph>();
	anim_controller->addAnimation("idle", &sdlutils().images().at("fringehead_idle"), 1, 12, 12, 24, -1);
	anim_controller->addAnimation("attack", &sdlutils().images().at("fringehead_atk"), 1, 13, 13, 24, 0, 0, 12, Vector2D(0.5, 0.7));
	//Proportion?
	anim_controller->keepProportion("idle", Vector2D(enemy->getComponent<Transform>()->getW(), enemy->getComponent<Transform>()->getH()));
	anim_controller->addTransition("idle", "attack", "Shoot", 1, false);
	anim_controller->addTransition("attack", "idle", "Shoot", 0, true);
	anim_controller->setParamValue("Shoot", 0);
	enemy->addComponent<FringeHeadAtack>();
	enemy->addComponent<Enemy_Health>(200, Vector2D(50, 5), build_sdlcolor(255, 0, 0, 200), 50);
	//build_sdlcolor(76, 15, 22, 200)

	return enemy;
}

Entity* EnemyGenerator::generateMedusa(Vector2D pos)
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
	fjh1->addComponent<Enemy_Health>(300, Vector2D(50, 5), build_sdlcolor(255, 0, 0, 255), 50);
	fjh1->addComponent<BoxCollider>(flowerJellyHat.physicType, flowerJellyHat.col, flowerJellyHat.colMask);
	fjh1->addComponent<ContactDamage>();
	fjh1->addComponent<JellyHatBehavior>(fjh1);

	return fjh1;
}

Entity* EnemyGenerator::generateElfShark(Vector2D pos)
{
	Config elfShark{};
	elfShark.vel = Vector2D(0, 0);
	elfShark.size = Vector2D(240.0f, 200.0f);
	elfShark.friction = 0.2f;
	elfShark.physicType = DYNAMIC;
	elfShark.fixedRotation = true;
	elfShark.rotation = 0.0f;
	elfShark.col = ENEMY;
	elfShark.colMask = ENEMY_MASK;


	auto* elf = createBasicEntity(pos, elfShark.size, elfShark.rotation, elfShark.vel);
	elf->addComponent<BoxCollider>(elfShark.physicType, elfShark.col, elfShark.colMask, elfShark.isTrigger,
		elfShark.friction, elfShark.fixedRotation, elfShark.rotation, Vector2D(elfShark.size.getX() * 0.5, elfShark.size.getY() * 0.4), 
		Vector2D(elfShark.pos.getX() * 1.5, elfShark.pos.getY() * 0.8));
	AnimBlendGraph* elf_anim_controller = elf->addComponent<AnimBlendGraph>();
	elf_anim_controller->addAnimation("idle", &sdlutils().images().at("elfshark_idle"), 1, 2, 2, 12, -1, 0, 1, Vector2D(0.66, 0.8));
	elf_anim_controller->addAnimation("move", &sdlutils().images().at("elfshark_move"), 1, 2, 2, 12, -1, 0, 1, Vector2D(0.66, 0.8));
	elf_anim_controller->addAnimation("attack_ini", &sdlutils().images().at("elfshark_attack"), 1, 19, 19, 48, 0, 0, 10, Vector2D(0.5, 0.8));
	elf_anim_controller->addAnimation("attack_end", &sdlutils().images().at("elfshark_attack"), 1, 19, 19, 24, 0, 11, 18, Vector2D(0.5, 0.8));
	//Proportion?
	elf_anim_controller->keepProportion("idle", Vector2D(elf->getComponent<Transform>()->getW(), elf->getComponent<Transform>()->getH()));
	elf_anim_controller->addTransition("idle", "move", "Speed", 1, false);
	elf_anim_controller->addTransition("move", "idle", "Speed", 0, false);
	elf_anim_controller->addTransition("idle", "attack_ini", "Attack", 1, false);
	elf_anim_controller->addTransition("attack_end", "idle", "Attack", 0, true);
	elf_anim_controller->addTransition("move", "attack_ini", "Attack", 1, false);
	elf_anim_controller->addTransition("attack_end", "move", "Attack", 0, true);
	elf_anim_controller->addTransition("attack_ini", "move", "Attack", 0, false);
	elf_anim_controller->addTransition("attack_ini", "idle", "Attack", 0, false);
	elf_anim_controller->addTransition("attack_ini", "attack_end", "Attack", 2, true);
	elf_anim_controller->setParamValue("Speed", 0);
	elf_anim_controller->setParamValue("Attack", 0);
	auto* trigger_elf = elf->addComponent<EnemyTrigger>(Vector2D(900.0f, 400.0f));
	trigger_elf->addTriggerComponent<ElfSharkAttack>(elf);
	elf->addComponent<Enemy_Health>(300, Vector2D(50, 5), build_sdlcolor(255, 0, 0, 255), 50);

	return elf;
}

Entity* EnemyGenerator::generatePompeyWorm(Vector2D pos)
{
	Config pompeyWorm{};
	pompeyWorm.pos = Vector2D(700, sdlutils().height() * 2.0f - 200);
	pompeyWorm.vel = Vector2D(0, 0);
	pompeyWorm.size = Vector2D(100.0f, 60.0f);
	pompeyWorm.friction = 100;
	pompeyWorm.physicType = DYNAMIC;
	pompeyWorm.fixedRotation = true;
	pompeyWorm.rotation = 0.0f;
	pompeyWorm.col = ENEMY;
	pompeyWorm.colMask = ENEMY_MASK;



	auto* gusano = createBasicEntity(pos, pompeyWorm.size, pompeyWorm.rotation, pompeyWorm.vel);
	gusano->addComponent<BoxCollider>(pompeyWorm.physicType, pompeyWorm.col, pompeyWorm.colMask);
	AnimBlendGraph* gusano_anim_controller = gusano->addComponent<AnimBlendGraph>();
	gusano_anim_controller->addAnimation("idle", &sdlutils().images().at("pompey_worm_idle"), 1, 2, 2, 12, -1);
	gusano_anim_controller->addAnimation("move", &sdlutils().images().at("pompey_worm_move"), 1, 2, 2, 12, -1);
	gusano_anim_controller->addAnimation("attack", &sdlutils().images().at("pompey_worm_attack"), 2, 3, 6, 12, 0, 0, 4);
	//Proportion?
	gusano_anim_controller->keepProportion("idle", Vector2D(gusano->getComponent<Transform>()->getW(), gusano->getComponent<Transform>()->getH()));
	gusano_anim_controller->addTransition("idle", "move", "Speed", 1, false);
	gusano_anim_controller->addTransition("move", "idle", "Speed", 0, false);
	gusano_anim_controller->addTransition("idle", "attack", "Attack", 1, false);
	gusano_anim_controller->addTransition("attack", "idle", "Attack", 0, true);
	gusano_anim_controller->addTransition("move", "attack", "Attack", 1, false);
	gusano_anim_controller->addTransition("attack", "move", "Attack", 0, true);
	gusano_anim_controller->setParamValue("Speed", 0);
	gusano_anim_controller->setParamValue("Attack", 0);
	auto* trigger_gusano = gusano->addComponent<EnemyTrigger>(Vector2D(800.0f, 500.0f));
	trigger_gusano->addTriggerComponent<PompeyWormAttack>(gusano);
	gusano->addComponent<Enemy_Health>(300, Vector2D(50, 5), build_sdlcolor(255, 0, 0, 255), 50);

	return gusano;
}

Entity* EnemyGenerator::createBasicEntity(const Vector2D& pos, const Vector2D& size, const float& rotation, const Vector2D& vel)
{
	auto* e = entity_->getMngr()->addEntity(); //entity_->manager_->addEntity();
	e->addComponent<Transform>(pos, vel, size.getX(), size.getY(), rotation);
	return e;
}

