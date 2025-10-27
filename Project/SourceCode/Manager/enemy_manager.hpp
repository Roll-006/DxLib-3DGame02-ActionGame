#pragma once
#include <vector>

#include "../Event/event_system.hpp"
#include "../Object/zombie.hpp"
#include "../JSON/json_loader.hpp"

// MEMO : 「責務」
// 掴み中は他エネミーに行動停止命令を送る
// ターゲットの設定

class EnemyManager final
{
public:
	EnemyManager();
	~EnemyManager();

	void Init();
	void Update();
	void LateUpdate();
	void Draw() const;

	void AddToObjManager();
	void RemoveToObjManager();

	void AttachTarget(const std::shared_ptr<CharacterBase>& target_character);
	void DetachTarget();
	
	void NotifyAllowAction			 (const ReleaseEvent&	event);
	void NotifyDisallowActionForcibly(const GrabEvent&		event);

private:
	std::vector<std::shared_ptr<EnemyBase>> m_enemies;
};
