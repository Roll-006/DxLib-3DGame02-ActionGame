#pragma once
#include <vector>

#include "../Base/singleton_base.hpp"
#include "../Event/event_system.hpp"
#include "../ObjectPool/enemy_object_pool.hpp"
#include "../JSON/json_loader.hpp"

class EnemyManager final : public SingletonBase<EnemyManager>
{
public:
	EnemyManager();
	~EnemyManager();

	void Init();
	void Update();
	void LateUpdate();
	void Draw() const;

	void AttachTarget(const std::shared_ptr<CharacterBase>& target_character);
	void DetachTarget();

	//[[nodiscard]] const int CanDetected() const { return m_detected_enemy_count <= 3; }
	
private:
	void NotifyAllowAction			 (const ReleaseEvent&			event);
	void NotifyDisallowActionForcibly(const GrabEvent&				event);
	//void NotifyDisallowActionForcibly(const DeadAllEnemyEvent&		event);
	void NotifyDetectedTarget		 (const OnTargetDetectedEvent&	event);
	void CountDeadEnemy				 (const DeadEnemyEvent&			event);

	void NotifyDisallowActionForcibly(const std::string& origin_enemy_id);

	void ReturnPool();

private:
	int m_enemy_size;
	int m_dead_enemy_count;
	int m_detected_enemy_count;

	std::shared_ptr<EnemyObjectPool>		m_object_pool;
	std::vector<std::shared_ptr<EnemyBase>> m_active_enemies;

	friend SingleInstanceBase<EnemyManager>;
};
