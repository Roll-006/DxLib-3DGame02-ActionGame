#include "rifle_cartridge_object_pool.hpp"

RifleCartridgeObjectPool::RifleCartridgeObjectPool() : 
	ObjectPoolBase(ObjectPoolName.RIFLE_CARTRIDGE_POOL)
{
	m_pool_size[ObjName.BULLET_556x45]			= kBulletPoolSize;
	m_pool_size[ObjName.SHELL_CASING_556x45]	= kShellCasingPoolSize;
	m_pool_size[ObjName.ROCKET_BOMB]			= kRocketBombPoolSize;

	// 弾丸
	for (int i = 0; i < m_pool_size.at(ObjName.BULLET_556x45); ++i)
	{
		const auto bullet = std::make_shared<Bullet>();

		CreateObj(bullet);

		ObjManager		::GetInstance()->AddObj		   (bullet);
		CollisionManager::GetInstance()->AddCollideObj (bullet);
		PhysicsManager	::GetInstance()->AddPhysicalObj(bullet);
	}

	// 薬莢
	for (int i = 0; i < m_pool_size.at(ObjName.SHELL_CASING_556x45); ++i)
	{
		const auto shell_casing = std::make_shared<ShellCasing>();

		CreateObj(shell_casing);

		ObjManager		::GetInstance()->AddObj			(shell_casing);
		CollisionManager::GetInstance()->AddCollideObj	(shell_casing);
		PhysicsManager	::GetInstance()->AddPhysicalObj	(shell_casing);
	}

	// ロケット弾
	for (int i = 0; i < m_pool_size.at(ObjName.ROCKET_BOMB); ++i)
	{
		const auto rocket_bomb = std::make_shared<RocketBomb>();

		CreateObj(rocket_bomb);

		ObjManager		::GetInstance()->AddObj			(rocket_bomb);
		CollisionManager::GetInstance()->AddCollideObj	(rocket_bomb);
		PhysicsManager	::GetInstance()->AddPhysicalObj	(rocket_bomb);
	}
}

RifleCartridgeObjectPool::~RifleCartridgeObjectPool()
{

}
