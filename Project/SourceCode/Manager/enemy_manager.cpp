#include "enemy_manager.hpp"

EnemyManager::EnemyManager() :
	m_enemy_size			(0),
	m_dead_enemy_count		(0),
	m_detected_enemy_count	(0),
	m_object_pool			(std::make_shared<EnemyObjectPool>())
{
	// イベント登録
	EventSystem::GetInstance()->Subscribe<ReleaseEvent>			(this, &EnemyManager::NotifyAllowAction);
	EventSystem::GetInstance()->Subscribe<GrabEvent>			(this, &EnemyManager::NotifyDisallowActionForcibly);
	//EventSystem::GetInstance()->Subscribe<DeadAllEnemyEvent>	(this, &EnemyManager::NotifyDisallowActionForcibly);
	EventSystem::GetInstance()->Subscribe<OnTargetDetectedEvent>(this, &EnemyManager::NotifyDetectedTarget);
	EventSystem::GetInstance()->Subscribe<DeadEnemyEvent>		(this, &EnemyManager::CountDeadEnemy);

	// 初期の敵を生成
	nlohmann::json j_data;
	if (json_loader::Load("Data/JSON/init_enemies.json", j_data))
	{
		// ゾンビ
		const auto init_zombie_size = j_data.at("init_enemies").at("zombie").size();
		m_enemy_size += static_cast<int>(init_zombie_size);
		for (size_t i = 0; i < init_zombie_size; ++i)
		{
			const auto enemy		= std::static_pointer_cast<EnemyBase>(m_object_pool->GetObj(ObjName.ZOMBIE));
			const auto pos			= j_data.at("init_enemies").at("zombie").at(std::to_string(i)).at("position").get<VECTOR>();
			const auto dir			= j_data.at("init_enemies").at("zombie").at(std::to_string(i)).at("direction").get<VECTOR>();
			const auto use_patrol	= j_data.at("init_enemies").at("zombie").at(std::to_string(i)).at("use_patrol");
			const auto route_id		= j_data.at("init_enemies").at("zombie").at(std::to_string(i)).at("route_id");
			const auto patrol_kind	= j_data.at("init_enemies").at("zombie").at(std::to_string(i)).at("patrol_kind");

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
	// イベントの登録解除
	EventSystem::GetInstance()->Unsubscribe<ReleaseEvent>			(this, &EnemyManager::NotifyAllowAction);
	EventSystem::GetInstance()->Unsubscribe<GrabEvent>				(this, &EnemyManager::NotifyDisallowActionForcibly);
	//EventSystem::GetInstance()->Unsubscribe<DeadAllEnemyEvent>		(this, &EnemyManager::NotifyDisallowActionForcibly);
	EventSystem::GetInstance()->Unsubscribe<OnTargetDetectedEvent>	(this, &EnemyManager::NotifyDetectedTarget);
	EventSystem::GetInstance()->Unsubscribe<DeadEnemyEvent>			(this, &EnemyManager::CountDeadEnemy);

	DetachTarget();

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
	for (const auto& owner_enemy : m_active_enemies)
	{
		owner_enemy->InitMoveOffset();

		for (const auto& target_enemy : m_active_enemies)
		{
			if (owner_enemy == target_enemy)		{ continue; }
			if (!owner_enemy ->IsDisperseEnemy())	{ continue; }
			if (!target_enemy->IsDisperseEnemy())	{ continue; }

			owner_enemy->CorrectMoveVelocity(target_enemy->GetTransform()->GetPos(CoordinateKind::kWorld));
		}

		owner_enemy->Update();
	}

	//printfDx("%d\n", m_detected_enemy_count);
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
		// 離した本人以外の敵の行動をすべて復帰させる
		if (event.enemy_handle != enemy->GetEnemyID())
		{
			enemy->OnAllowAction();
		}
	}
}

void EnemyManager::NotifyDisallowActionForcibly(const GrabEvent& event)
{
	// 掴んだ本人以外の敵の行動はすべて停止させる
	NotifyDisallowActionForcibly(event.enemy_id);
}

void EnemyManager::NotifyDetectedTarget(const OnTargetDetectedEvent& event)
{
	++m_detected_enemy_count;

	for (const auto& enemy : m_active_enemies)
	{
		if (enemy->IsDetectedTarget())	{ continue; }
		//if (!CanDetected())				{ return; }

		const auto enemy_pos	= enemy->GetTransform()->GetPos(CoordinateKind::kWorld);
		const auto distance		= VSize(enemy_pos - event.notify_pos);

		// 距離が一定内なら発見状態にする
		if(distance <= event.notify_distance)
		{
			enemy->OnDetected();
			++m_detected_enemy_count;
		}
	}
}

void EnemyManager::CountDeadEnemy(const DeadEnemyEvent& event)
{
	++m_dead_enemy_count;
	--m_detected_enemy_count;
	if (m_dead_enemy_count >= m_enemy_size)
	{
		EventSystem::GetInstance()->Publish(DeadAllEnemyEvent());
	}
}

void EnemyManager::NotifyDisallowActionForcibly(const std::string& origin_enemy_id)
{
	for (const auto& enemy : m_active_enemies)
	{
		// 本人以外の敵の行動をすべて停止させる
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
