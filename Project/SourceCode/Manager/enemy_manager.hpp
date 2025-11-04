#pragma once
#include <vector>

#include "../Event/event_system.hpp"
#include "../ObjectPool/enemy_object_pool.hpp"
#include "../JSON/json_loader.hpp"

class EnemyManager final
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
	
private:
	void NotifyAllowAction			 (const ReleaseEvent&			event);
	void NotifyDisallowActionForcibly(const GrabEvent&				event);
	void NotifyDisallowActionForcibly(const DeadBossEvent&			event);
	void NotifyDetectedTarget		 (const OnTargetDetectedEvent&	event);

	void NotifyDisallowActionForcibly(const std::string& origin_enemy_id);

	void ReturnPool();

private:
	std::shared_ptr<EnemyObjectPool>		m_object_pool;
	std::vector<std::shared_ptr<EnemyBase>> m_active_enemies;
};
