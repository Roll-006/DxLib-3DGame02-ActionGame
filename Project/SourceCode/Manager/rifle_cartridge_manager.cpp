#include "rifle_cartridge_manager.hpp"

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
	
	// プールへ変換
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

void RifleCartridgeManager::DrawToShadowMap() const
{
	for (const auto& rifle_cartridge : m_rifle_cartridge)
	{
		for (const auto& part : rifle_cartridge.second)
		{
			part->DrawToShadowMap();
		}
	}
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

	//const auto pool = ObjectPoolHolder::GetInstance()->GetObjectPool(ObjectPoolName.RIFLE_CARTRIDGE_POOL);
	//DrawFormatString(0,  80, 0xffffff, "bullet_pool_size          : %d", pool->GetPoolSize(ObjName.BULLET));
	//DrawFormatString(0, 100, 0xffffff, "shell_casing_pool_size    : %d", pool->GetPoolSize(ObjName.SHELL_CASING));
	//DrawFormatString(0, 120, 0xffffff, "bullet_manager_size       : %d", m_rifle_cartridge.at(ObjName.BULLET).size());
	//DrawFormatString(0, 140, 0xffffff, "shell_casing_manager_size : %d", m_rifle_cartridge.at(ObjName.SHELL_CASING).size());

	std::queue<VECTOR> temp = m_hit_pos;
	while (!temp.empty())
	{
		DrawSphere3D(temp.front(), 5, 16, 0xff0000, 0xff0000, TRUE);
		temp.pop();
	}
}

void RifleCartridgeManager::Shot(GunBase& gun)
{
	const auto object_pool = ObjectPoolHolder::GetInstance()->GetObjectPool(ObjectPoolName.RIFLE_CARTRIDGE_POOL);

	// プールから弾丸を取得し、有効であれば発射
	const auto bullet = std::static_pointer_cast<Bullet>(object_pool->GetObj(ObjName.BULLET));
	if (bullet != nullptr)
	{
		bullet->OnShot(gun);
		AddRifleCartridge(bullet);	
	}

	// プールから薬莢を取得し、有効であれば排出
	const auto shell_casing = std::static_pointer_cast<ShellCasing>(object_pool->GetObj(ObjName.SHELL_CASING));
	if (shell_casing != nullptr)
	{
		shell_casing->Eject(gun);
		AddRifleCartridge(shell_casing);
	}
}

void RifleCartridgeManager::DeleteBullet(const int obj_handle)
{
	// 指定の弾丸を削除
	const auto remove_bullet = std::find_if(m_rifle_cartridge[ObjName.BULLET].begin(), m_rifle_cartridge[ObjName.BULLET].end(), [=](const std::shared_ptr<ObjBase> obj)
	{
		return obj->GetObjHandle() == obj_handle;
	});

	if(remove_bullet != m_rifle_cartridge[ObjName.BULLET].end())
	{
		ObjectPoolHolder::GetInstance()->GetObjectPool(ObjectPoolName.RIFLE_CARTRIDGE_POOL)->ReturnObj(*remove_bullet);
		m_rifle_cartridge[ObjName.BULLET].erase(remove_bullet);
	}
}

void RifleCartridgeManager::AddRifleCartridge(const std::shared_ptr<ObjBase> rifle_cartridge)
{
	if (std::find(m_rifle_cartridge[rifle_cartridge->GetName()].begin(), m_rifle_cartridge[rifle_cartridge->GetName()].end(), rifle_cartridge) == m_rifle_cartridge[rifle_cartridge->GetName()].end())
	{
		m_rifle_cartridge[rifle_cartridge->GetName()].emplace_back(rifle_cartridge);
	}
}
