#include "rifle_cartridge_object_pool.hpp"

RifleCartridgeObjectPool::RifleCartridgeObjectPool() : 
	ObjectPoolBase(ObjectPoolName.RIFLE_CARTRIDGE_POOL)
{
	m_pool_size[ObjName.BULLET]						= kBulletPoolSize;
	m_pool_size[ObjName.SHELL_CASING_556x45]		= kShellCasingPoolSize;
	m_pool_size[ObjName.ROCKET_BOMB]				= kRocketBombPoolSize;

	// 弾丸
	for (size_t i = 0; i < m_pool_size.at(ObjName.BULLET); ++i)
	{
		const auto bullet = std::make_shared<Bullet>();

		CreateObj(bullet);
		bullet->AddToObjManager();
	}

	// 薬莢
	for (size_t i = 0; i < m_pool_size.at(ObjName.SHELL_CASING_556x45); ++i)
	{
		const auto shell_casing = std::make_shared<ShellCasing>(ModelPath.SHELL_CASING_556x45);

		CreateObj(shell_casing);
		shell_casing->AddToObjManager();
	}

	// ロケット弾
	for (size_t i = 0; i < m_pool_size.at(ObjName.ROCKET_BOMB); ++i)
	{
		const auto rocket_bomb = std::make_shared<RocketBomb>();

		CreateObj(rocket_bomb);
		rocket_bomb->AddToObjManager();
	}
}

RifleCartridgeObjectPool::~RifleCartridgeObjectPool()
{
	DestroyObjects(ObjName.BULLET);
	DestroyObjects(ObjName.SHELL_CASING_556x45);
	DestroyObjects(ObjName.ROCKET_BOMB);
}
