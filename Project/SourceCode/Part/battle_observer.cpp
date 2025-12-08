#include "battle_observer.hpp"

BattleObserver::BattleObserver() :
	m_battle_enemies_count	(0),
	m_is_in_battle			(false)
{
	// イベント登録
	EventSystem::GetInstance()->Subscribe<OnTargetDetectedEvent>	(this, &BattleObserver::AddInBattleCount);
	EventSystem::GetInstance()->Subscribe<DeadEnemyEvent>			(this, &BattleObserver::RemoveInBattleCount);
}

BattleObserver::~BattleObserver()
{
	// イベント登録
	EventSystem::GetInstance()->Unsubscribe<OnTargetDetectedEvent>	(this, &BattleObserver::AddInBattleCount);
	EventSystem::GetInstance()->Unsubscribe<DeadEnemyEvent>			(this, &BattleObserver::RemoveInBattleCount);
}

void BattleObserver::AddInBattleCount(const OnTargetDetectedEvent& event)
{
	++m_battle_enemies_count;

	// 最初の発見者であった場合は戦闘開始
	if (m_battle_enemies_count <= 1)
	{
		m_is_in_battle = true;
		EventSystem::GetInstance()->Publish(StartBattleEvent());
	}
}

void BattleObserver::RemoveInBattleCount(const DeadEnemyEvent& event)
{
	const auto prev_count = m_battle_enemies_count;

	--m_battle_enemies_count;

	// 以前まで戦闘中の敵がいて現在一体もいない場合は戦闘終了
	if (prev_count >= 1 && m_battle_enemies_count <= 0)
	{
		m_is_in_battle = false;
		EventSystem::GetInstance()->Publish(EndBattleEvent());
	}
}
