#include "enemy_manager.hpp"

EnemyManager::EnemyManager()
{
	m_enemies.emplace_back(std::make_shared<Zombie>());
}

EnemyManager::~EnemyManager()
{

}

void EnemyManager::Init()
{
	for (const auto& enemy : m_enemies)
	{
		enemy->Init();
	}
}

void EnemyManager::Update()
{
	for (const auto& enemy : m_enemies)
	{
		enemy->Update();
	}
}

void EnemyManager::LateUpdate()
{
	for (const auto& enemy : m_enemies)
	{
		enemy->LateUpdate();
	}
}

void EnemyManager::DrawToShadowMap() const
{
	for (const auto& enemy : m_enemies)
	{
		enemy->DrawToShadowMap();
	}
}

void EnemyManager::Draw() const
{
	for (const auto& enemy : m_enemies)
	{
		enemy->Draw();
	}
}

void EnemyManager::AddToObjManager()
{
	for (const auto& enemy : m_enemies)
	{
		enemy->AddToObjManager();
	}
}

void EnemyManager::RemoveToObjManager()
{
	for (const auto& enemy : m_enemies)
	{
		enemy->RemoveToObjManager();
	}
}

void EnemyManager::AttachTarget(const std::shared_ptr<CharacterBase> target_character)
{
	for (const auto& enemy : m_enemies)
	{
		enemy->AttachTarget(target_character);
	}
}

void EnemyManager::DetachTarget()
{
	for (const auto& enemy : m_enemies)
	{
		enemy->DetachTarget();
	}
}

void EnemyManager::NotifyStopActionForcibly()
{

}
