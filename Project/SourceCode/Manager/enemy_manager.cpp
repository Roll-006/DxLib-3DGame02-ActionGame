#include "enemy_manager.hpp"

EnemyManager::EnemyManager() :
	m_object_pool(std::make_shared<EnemyObjectPool>())
{
	// ƒCƒxƒ“ƒg“o˜^
	EventSystem::GetInstance()->Subscribe<ReleaseEvent>	(this, &EnemyManager::NotifyAllowAction);
	EventSystem::GetInstance()->Subscribe<GrabEvent>	(this, &EnemyManager::NotifyDisallowActionForcibly);

	JSONLoader json_loader;
	nlohmann::json data;
	if (json_loader.Load("Data/JSON/points.json", data))
	{
		const auto pos		= data.at("position") .get<std::vector<VECTOR>>();
		const auto dir		= data.at("direction").get<std::vector<VECTOR>>();
		const auto count	= min(pos.size(), dir.size());

		for (size_t i = 0; i < 3; ++i)
		{
			const auto enemy = std::static_pointer_cast<EnemyBase>(m_object_pool->GetObj(ObjName.ZOMBIE));

			m_active_enemies.emplace_back(enemy);
			enemy->OnRespawn(pos.at(i), dir.at(i));
		}
	}
}

EnemyManager::~EnemyManager()
{
	// ƒCƒxƒ“ƒg‚Ì“o˜^‰ðœ
	EventSystem::GetInstance()->Unsubscribe<ReleaseEvent>	(this, &EnemyManager::NotifyAllowAction);
	EventSystem::GetInstance()->Unsubscribe<GrabEvent>		(this, &EnemyManager::NotifyDisallowActionForcibly);
}

void EnemyManager::Init()
{
	for (const auto& enemy : m_active_enemies)
	{
		enemy->Init();
	}
}

void EnemyManager::Update()
{
	for (const auto& enemy : m_active_enemies)
	{
		enemy->Update();
	}
}

void EnemyManager::LateUpdate()
{
	for (const auto& enemy : m_active_enemies)
	{
		enemy->LateUpdate();
	}
}

void EnemyManager::Draw() const
{
	for (const auto& enemy : m_active_enemies)
	{
		enemy->Draw();
	}
}

void EnemyManager::AddToObjManager()
{
	//for (const auto& enemy : m_active_enemies)
	//{
	//	enemy->AddToObjManager();
	//}
}

void EnemyManager::RemoveToObjManager()
{
	//for (const auto& enemy : m_active_enemies)
	//{
	//	enemy->RemoveToObjManager();
	//}
}

void EnemyManager::AttachTarget(const std::shared_ptr<CharacterBase>& target_character)
{
	for (const auto& enemy : m_active_enemies)
	{
		enemy->AttachTarget(target_character);
	}
}

void EnemyManager::DetachTarget()
{
	for (const auto& enemy : m_active_enemies)
	{
		enemy->DetachTarget();
	}
}

void EnemyManager::NotifyAllowAction(const ReleaseEvent& event)
{
	for (const auto& enemy : m_active_enemies)
	{
		// —£‚µ‚½–{lˆÈŠO‚Ì“G‚Ìs“®‚ð‚·‚×‚Ä•œ‹A‚³‚¹‚é
		if (event.enemy_handle != enemy->GetEnemyHandle())
		{
			enemy->OnAllowAction();
		}
	}
}

void EnemyManager::NotifyDisallowActionForcibly(const GrabEvent& event)
{
	for (const auto& enemy : m_active_enemies)
	{
		// ’Í‚ñ‚¾–{lˆÈŠO‚Ì“G‚Ìs“®‚Í‚·‚×‚Ä’âŽ~‚³‚¹‚é
		if(event.enemy_handle != enemy->GetEnemyHandle())
		{
			enemy->OnDisallowActionForcibly();
		}
	}
}
