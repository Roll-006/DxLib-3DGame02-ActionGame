#include "enemy_manager.hpp"

EnemyManager::EnemyManager() :
	m_enemy_size		(0),
	m_dead_enemy_count	(0),
	m_object_pool		(std::make_shared<EnemyObjectPool>())
{
	// ƒCƒxƒ“ƒg“o˜^
	EventSystem::GetInstance()->Subscribe<ReleaseEvent>			(this, &EnemyManager::NotifyAllowAction);
	EventSystem::GetInstance()->Subscribe<GrabEvent>			(this, &EnemyManager::NotifyDisallowActionForcibly);
	EventSystem::GetInstance()->Subscribe<DeadAllEnemyEvent>	(this, &EnemyManager::NotifyDisallowActionForcibly);
	EventSystem::GetInstance()->Subscribe<OnTargetDetectedEvent>(this, &EnemyManager::NotifyDetectedTarget);
	EventSystem::GetInstance()->Subscribe<DeadEnemyEvent>		(this, &EnemyManager::CountDeadEnemy);

	// ‰Šú‚Ì“G‚ğ¶¬
	JSONLoader json_loader;
	nlohmann::json data;
	if (json_loader.Load("Data/JSON/init_enemies.json", data))
	{
		// ƒ]ƒ“ƒr
		const auto init_zombie_size = data.at("init_enemies").at("zombie").size();
		m_enemy_size += static_cast<int>(init_zombie_size);
		for (size_t i = 0; i < init_zombie_size; ++i)
		{
			const auto enemy		= std::static_pointer_cast<EnemyBase>(m_object_pool->GetObj(ObjName.ZOMBIE));
			const auto pos			= data.at("init_enemies").at("zombie").at(std::to_string(i)).at("position").get<VECTOR>();
			const auto dir			= data.at("init_enemies").at("zombie").at(std::to_string(i)).at("direction").get<VECTOR>();
			const auto use_patrol	= data.at("init_enemies").at("zombie").at(std::to_string(i)).at("use_patrol");
			const auto route_id		= data.at("init_enemies").at("zombie").at(std::to_string(i)).at("route_id");
			const auto patrol_kind	= data.at("init_enemies").at("zombie").at(std::to_string(i)).at("patrol_kind");

			m_active_enemies.emplace_back(enemy);
			enemy->OnRespawn(pos, dir);
			if (use_patrol)
			{
				enemy->CreatePatrolPos(patrol_kind, route_id);
			}
		}
	}

	const auto pool_holder = ObjectPoolHolder::GetInstance();
	pool_holder->AddObjectPool(m_object_pool);
}

EnemyManager::~EnemyManager()
{
	// ƒCƒxƒ“ƒg‚Ì“o˜^‰ğœ
	EventSystem::GetInstance()->Unsubscribe<ReleaseEvent>			(this, &EnemyManager::NotifyAllowAction);
	EventSystem::GetInstance()->Unsubscribe<GrabEvent>				(this, &EnemyManager::NotifyDisallowActionForcibly);
	EventSystem::GetInstance()->Unsubscribe<DeadAllEnemyEvent>		(this, &EnemyManager::NotifyDisallowActionForcibly);
	EventSystem::GetInstance()->Unsubscribe<OnTargetDetectedEvent>	(this, &EnemyManager::NotifyDetectedTarget);
	EventSystem::GetInstance()->Unsubscribe<DeadEnemyEvent>			(this, &EnemyManager::CountDeadEnemy);

	for (const auto enemy : m_active_enemies)
	{
		m_object_pool->ReturnObj(enemy);
	}

	const auto pool_holder = ObjectPoolHolder::GetInstance();
	pool_holder->RemoveObjectPool(m_object_pool->GetName());
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

	ReturnPool();
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
		// —£‚µ‚½–{lˆÈŠO‚Ì“G‚Ìs“®‚ğ‚·‚×‚Ä•œ‹A‚³‚¹‚é
		if (event.enemy_handle != enemy->GetEnemyID())
		{
			enemy->OnAllowAction();
		}
	}
}

void EnemyManager::NotifyDisallowActionForcibly(const GrabEvent& event)
{
	// ’Í‚ñ‚¾–{lˆÈŠO‚Ì“G‚Ìs“®‚Í‚·‚×‚Ä’â~‚³‚¹‚é
	NotifyDisallowActionForcibly(event.enemy_id);
}

void EnemyManager::NotifyDisallowActionForcibly(const DeadAllEnemyEvent& event)
{
	// €–S‚µ‚½–{lˆÈŠO‚Ì“G‚Ìs“®‚ğ‚·‚×‚Ä’â~‚³‚¹‚é
	//NotifyDisallowActionForcibly(event.enemy_id);
}

void EnemyManager::NotifyDetectedTarget(const OnTargetDetectedEvent& event)
{
	for (const auto& enemy : m_active_enemies)
	{
		if (enemy->IsDetectedTarget()) { continue; }

		const auto enemy_pos	= enemy->GetTransform()->GetPos(CoordinateKind::kWorld);
		const auto distance		= VSize(enemy_pos - event.notify_pos);

		// ‹——£‚ªˆê’è“à‚È‚ç”­Œ©ó‘Ô‚É‚·‚é
		if(distance <= event.notify_distance)
		{
			enemy->OnDetected();
		}
	}
}

void EnemyManager::CountDeadEnemy(const DeadEnemyEvent& event)
{
	++m_dead_enemy_count;
	if (m_dead_enemy_count >= m_enemy_size)
	{
		EventSystem::GetInstance()->Publish(DeadAllEnemyEvent());
	}
}

void EnemyManager::NotifyDisallowActionForcibly(const std::string& origin_enemy_id)
{
	for (const auto& enemy : m_active_enemies)
	{
		// –{lˆÈŠO‚Ì“G‚Ìs“®‚ğ‚·‚×‚Ä’â~‚³‚¹‚é
		if (origin_enemy_id != enemy->GetEnemyID())
		{
			enemy->OnDisallowActionForcibly();
		}
	}
}

void EnemyManager::ReturnPool()
{
	for (auto itr = m_active_enemies.begin(); itr != m_active_enemies.end();)
	{
		if (std::dynamic_pointer_cast<IPoolable>(*itr)->IsReturnPool())
		{
			ObjectPoolHolder::GetInstance()->GetObjectPool(ObjectPoolName.ENEMY_POOL)->ReturnObj(*itr);
			itr = m_active_enemies.erase(itr);
		}
		else
		{
			++itr;
		}
	}
}
