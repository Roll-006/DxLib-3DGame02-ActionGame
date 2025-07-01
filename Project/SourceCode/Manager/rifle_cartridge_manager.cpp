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
	
	// TODO : 後に見やすく変更
	// 生存していない弾丸は削除
	const auto remove_bullet = std::find_if(m_rifle_cartridge[ObjName.BULLET].begin(), m_rifle_cartridge[ObjName.BULLET].end(), [](const std::shared_ptr<ObjBase> obj)
	{
		return !std::dynamic_pointer_cast<Bullet>(obj)->IsAlive();
	});
	if (remove_bullet != m_rifle_cartridge[ObjName.BULLET].end())
	{
		ObjectPoolManager::GetInstance()->GetObjectPool(ObjectPoolName.BULLET_POOL)->ReturnObj(*remove_bullet);
		m_rifle_cartridge[ObjName.BULLET].erase(remove_bullet);
	}

	// TODO : 後に見やすく変更
	// 生存していない薬莢は削除
	const auto remove_shell_casing = std::find_if(m_rifle_cartridge[ObjName.SHELL_CASING].begin(), m_rifle_cartridge[ObjName.SHELL_CASING].end(), [](const std::shared_ptr<ObjBase> obj)
	{
		return !std::dynamic_pointer_cast<ShellCasing>(obj)->IsAlive();
	});
	if (remove_shell_casing != m_rifle_cartridge[ObjName.SHELL_CASING].end())
	{
		ObjectPoolManager::GetInstance()->GetObjectPool(ObjectPoolName.BULLET_POOL)->ReturnObj(*remove_shell_casing);
		m_rifle_cartridge[ObjName.SHELL_CASING].erase(remove_shell_casing);
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

	const auto pool = ObjectPoolManager::GetInstance()->GetObjectPool(ObjectPoolName.BULLET_POOL);
	DrawFormatString(0,  80, 0xffffff, "bullet_pool_size          : %d", pool->GetPoolSize(ObjName.BULLET));
	DrawFormatString(0, 100, 0xffffff, "shell_casing_pool_size    : %d", pool->GetPoolSize(ObjName.SHELL_CASING));
	DrawFormatString(0, 120, 0xffffff, "bullet_manager_size       : %d", m_rifle_cartridge.at(ObjName.BULLET).size());
	DrawFormatString(0, 140, 0xffffff, "shell_casing_manager_size : %d", m_rifle_cartridge.at(ObjName.SHELL_CASING).size());


	std::queue<VECTOR> temp = m_hit_pos;
	while (!temp.empty())
	{
		DrawSphere3D(temp.front(), 5, 16, 0xff0000, 0xff0000, TRUE);
		temp.pop();
	}
}

void RifleCartridgeManager::Shot(GunBase& gun)
{
	const auto object_pool = ObjectPoolManager::GetInstance()->GetObjectPool(ObjectPoolName.BULLET_POOL);

	// プールから弾丸を取得し、有効であれば発射
	const auto bullet = std::dynamic_pointer_cast<Bullet>(object_pool->GetObj(ObjName.BULLET));
	if (bullet != nullptr)
	{
		bullet->OnShot(gun);
		AddBullet(bullet);
		
	}

	// プールから薬莢を取得し、有効であれば排出
	const auto shell_casing = std::dynamic_pointer_cast<ShellCasing>(object_pool->GetObj(ObjName.SHELL_CASING));
	if (shell_casing != nullptr)
	{
		shell_casing->Eject(gun);
		AddShellCasing(shell_casing);
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
		ObjectPoolManager::GetInstance()->GetObjectPool(ObjectPoolName.BULLET_POOL)->ReturnObj(*remove_bullet);
		m_rifle_cartridge[ObjName.BULLET].erase(remove_bullet);
	}
}

void RifleCartridgeManager::AddBullet(const std::shared_ptr<Bullet> bullet)
{
	if (std::find(m_rifle_cartridge[ObjName.BULLET].begin(), m_rifle_cartridge[ObjName.BULLET].end(), bullet) == m_rifle_cartridge[ObjName.BULLET].end())
	{
		m_rifle_cartridge[ObjName.BULLET].emplace_back(bullet);
	}
}

void RifleCartridgeManager::AddShellCasing(const std::shared_ptr<ShellCasing> shell_casing)
{
	if (std::find(m_rifle_cartridge[ObjName.SHELL_CASING].begin(), m_rifle_cartridge[ObjName.SHELL_CASING].end(), shell_casing) == m_rifle_cartridge[ObjName.SHELL_CASING].end())
	{
		m_rifle_cartridge[ObjName.SHELL_CASING].emplace_back(shell_casing);
	}
}
