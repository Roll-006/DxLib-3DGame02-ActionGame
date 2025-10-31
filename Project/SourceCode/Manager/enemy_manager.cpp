#include "enemy_manager.hpp"

EnemyManager::EnemyManager() :
	m_object_pool(std::make_shared<EnemyObjectPool>())
{
	// イベント登録
	EventSystem::GetInstance()->Subscribe<ReleaseEvent>			(this, &EnemyManager::NotifyAllowAction);
	EventSystem::GetInstance()->Subscribe<GrabEvent>			(this, &EnemyManager::NotifyDisallowActionForcibly);
	EventSystem::GetInstance()->Subscribe<OnTargetDetectedEvent>(this, &EnemyManager::NotifyDetectedTarget);

	// 初期の敵を生成
	JSONLoader json_loader;
	nlohmann::json data;
	if (json_loader.Load("Data/JSON/init_enemies.json", data))
	{
		const auto init_enemies_size = data.at("init_enemies").size();

		for (size_t i = 0; i < 4; ++i)
		{
			const auto enemy		= std::static_pointer_cast<EnemyBase>(m_object_pool->GetObj(ObjName.ZOMBIE));
			const auto pos			= data.at("init_enemies").at(std::to_string(i)).at("position") .get<VECTOR>();
			const auto dir			= data.at("init_enemies").at(std::to_string(i)).at("direction").get<VECTOR>();
			const auto use_patrol	= data.at("init_enemies").at(std::to_string(i)).at("use_patrol");
			const auto route_id		= data.at("init_enemies").at(std::to_string(i)).at("route_id");
			const auto patrol_kind	= data.at("init_enemies").at(std::to_string(i)).at("patrol_kind");
			
			m_active_enemies.emplace_back(enemy);
			enemy->OnRespawn(pos, dir);
			if (use_patrol)
			{
				enemy->CreatePatrolPos(patrol_kind, route_id);
			}
		}
	}
}

EnemyManager::~EnemyManager()
{
	// イベントの登録解除
	EventSystem::GetInstance()->Unsubscribe<ReleaseEvent>			(this, &EnemyManager::NotifyAllowAction);
	EventSystem::GetInstance()->Unsubscribe<GrabEvent>				(this, &EnemyManager::NotifyDisallowActionForcibly);
	EventSystem::GetInstance()->Unsubscribe<OnTargetDetectedEvent>	(this, &EnemyManager::NotifyDetectedTarget);
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

void EnemyManager::NotifyDetectedTarget(const OnTargetDetectedEvent& event)
{
	for (const auto& enemy : m_active_enemies)
	{
		if (enemy->IsTargetInSight()) { continue; }

		const auto enemy_pos	= enemy->GetTransform()->GetPos(CoordinateKind::kWorld);
		const auto distance		= VSize(enemy_pos - event.notify_pos);

		// 距離が一定内なら発見状態にする
		if(distance <= event.notify_distance)
		{
			enemy->OnDetected();
		}
	}
}
