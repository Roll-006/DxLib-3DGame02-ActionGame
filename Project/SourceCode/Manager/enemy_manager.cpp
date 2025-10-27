#include "enemy_manager.hpp"

EnemyManager::EnemyManager()
{
	// イベント登録
	EventSystem::GetInstance()->Subscribe<ReleaseEvent>	(this, &EnemyManager::NotifyAllowAction);
	EventSystem::GetInstance()->Subscribe<GrabEvent>	(this, &EnemyManager::NotifyDisallowActionForcibly);

	// 初期位置・向きを設定
	// TODO : 仮で3体配置。のちに変更。
	JSONLoader json_loader;
	nlohmann::json data;
	if (json_loader.Load("Data/JSON/enemy_data.json", data))
	{
		for (int i = 0; i < 3; ++i)
		{
			const auto enemy	= data ["enemies"][std::to_string(i + 1)];
			const auto pos		= enemy["position"];
			const auto look_dir	= enemy["direction"];
			m_enemies.emplace_back(std::make_shared<Zombie>(pos, look_dir));
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

void EnemyManager::AttachTarget(const std::shared_ptr<CharacterBase>& target_character)
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

void EnemyManager::NotifyAllowAction(const ReleaseEvent& event)
{
	for (const auto& enemy : m_enemies)
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
	for (const auto& enemy : m_enemies)
	{
		// 掴んだ本人以外の敵の行動はすべて停止させる
		if(event.enemy_handle != enemy->GetEnemyHandle())
		{
			enemy->OnDisallowActionForcibly();
		}
	}
}
