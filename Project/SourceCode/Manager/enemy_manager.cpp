#include "enemy_manager.hpp"

EnemyManager::EnemyManager() :
	m_object_pool(std::make_shared<EnemyObjectPool>())
{
	// イベント登録
	EventSystem::GetInstance()->Subscribe<ReleaseEvent>	(this, &EnemyManager::NotifyAllowAction);
	EventSystem::GetInstance()->Subscribe<GrabEvent>	(this, &EnemyManager::NotifyDisallowActionForcibly);

	// 初期の敵を生成
	JSONLoader json_loader;
	nlohmann::json data;
	if (json_loader.Load("Data/JSON/points.json", data))
	{
		const auto pos		= data.at("position") .get<std::vector<VECTOR>>();
		const auto dir		= data.at("direction").get<std::vector<VECTOR>>();
		const auto count	= min(pos.size(), dir.size());

		for (size_t i = 0; i < 1; ++i)
		{
			const auto enemy = std::static_pointer_cast<EnemyBase>(m_object_pool->GetObj(ObjName.ZOMBIE));

			m_active_enemies.emplace_back(enemy);
			enemy->OnRespawn(pos.at(i), dir.at(i));

			// TODO : 仮
			if (i == 0)
			{
				enemy->CreatePatrolPos(PatrolRouteGiver::PatrolKind::kStop, "0");
			}
			//else if (i == 1)
			//{
			//	enemy->CreatePatrolPos(PatrolRouteGiver::PatrolKind::kLoop, "1");
			//}
		}
	}
}

EnemyManager::~EnemyManager()
{
	// イベントの登録解除
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
		// 離した本人以外の敵の行動をすべて復帰させる
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
		// 掴んだ本人以外の敵の行動はすべて停止させる
		if(event.enemy_handle != enemy->GetEnemyHandle())
		{
			enemy->OnDisallowActionForcibly();
		}
	}
}
