#include "rifle_cartridge_manager.hpp"
#include "../Base/gun_base.hpp"

RifleCartridgeManager::RifleCartridgeManager()
{

}

RifleCartridgeManager::~RifleCartridgeManager()
{

}

void RifleCartridgeManager::Update()
{
	for (const auto& rifle_cartridge : m_rifle_cartridge)
	{
		for (const auto& part : rifle_cartridge.second)
		{
			part->Update();
		}
	}
}

void RifleCartridgeManager::LateUpdate()
{
	for (const auto& rifle_cartridge : m_rifle_cartridge)
	{
		for (const auto& part : rifle_cartridge.second)
		{
			part->LateUpdate();
		}
	}
	
	ReturnPool();
}

void RifleCartridgeManager::Draw() const
{
	for (const auto& rifle_cartridge : m_rifle_cartridge)
	{
		for (const auto& part : rifle_cartridge.second)
		{
			part->Draw();
		}
	}
}

void RifleCartridgeManager::SearchValidRifleCartidge(GunBase& gun)
{
	const auto pool = ObjectPoolHolder::GetInstance()->GetObjectPool(ObjectPoolName.RIFLE_CARTRIDGE_POOL);
	std::shared_ptr<ObjBase>		bullet_obj		= nullptr;
	std::shared_ptr<IBullet>		bullet			= nullptr;
	std::shared_ptr<ShellCasing>	shell_casing	= nullptr;

	// プールから弾丸・薬莢を取得
	switch (gun.GetGunKind())
	{
	case GunKind::kAssaultRifle:
		bullet_obj		= pool->GetObj(ObjName.BULLET);
		bullet			= std::static_pointer_cast<Bullet>(bullet_obj);
		shell_casing	= std::static_pointer_cast<ShellCasing>(pool->GetObj(ObjName.SHELL_CASING_556x45));
		break;

	case GunKind::kRocketLauncher:
		bullet_obj		= pool->GetObj(ObjName.ROCKET_BOMB);
		bullet			= std::static_pointer_cast<RocketBomb>(bullet_obj);
		break;

	default:
		break;
	}

	// 有効であれば発射
	if (bullet != nullptr)
	{
		bullet->OnShot(gun);
		AddRifleCartridge(bullet_obj);
	}

	// 有効であれば排出
	if (shell_casing != nullptr)
	{
		shell_casing->Eject(gun);
		AddRifleCartridge(shell_casing);
	}
}

void RifleCartridgeManager::DeleteBullet(const std::shared_ptr<ObjBase>& bullet)
{
	// 指定の弾丸を削除
	const auto remove_bullet = std::find_if(m_rifle_cartridge[bullet->GetName()].begin(), m_rifle_cartridge[bullet->GetName()].end(), [=](const std::shared_ptr<ObjBase> obj)
	{
		return obj->GetObjHandle() == bullet->GetObjHandle();
	});

	if(remove_bullet != m_rifle_cartridge[bullet->GetName()].end())
	{
		ObjectPoolHolder::GetInstance()->GetObjectPool(ObjectPoolName.RIFLE_CARTRIDGE_POOL)->ReturnObj(*remove_bullet);
		m_rifle_cartridge[bullet->GetName()].erase(remove_bullet);
	}
}

void RifleCartridgeManager::AddRifleCartridge(const std::shared_ptr<ObjBase>& rifle_cartridge)
{
	if (std::find(m_rifle_cartridge[rifle_cartridge->GetName()].begin(), m_rifle_cartridge[rifle_cartridge->GetName()].end(), rifle_cartridge) == m_rifle_cartridge[rifle_cartridge->GetName()].end())
	{
		m_rifle_cartridge[rifle_cartridge->GetName()].emplace_back(rifle_cartridge);
	}
}

void RifleCartridgeManager::ReturnPool()
{
	for (auto& [obj_name, objects] : m_rifle_cartridge)
	{
		auto& vec = objects;
		for (auto itr = vec.begin(); itr != vec.end();)
		{
			if (std::dynamic_pointer_cast<IPoolable>(*itr)->IsReturnPool())
			{
				ObjectPoolHolder::GetInstance()->GetObjectPool(ObjectPoolName.RIFLE_CARTRIDGE_POOL)->ReturnObj(*itr);
				itr = vec.erase(itr);
			}
			else
			{
				++itr;
			}
		}
	}
}
