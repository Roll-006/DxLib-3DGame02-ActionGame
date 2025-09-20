#pragma once
#include <vector>
#include "../Interface/i_enemy.hpp"

// MEMO : 「責務」
// 掴み中は他エネミーに行動停止命令を送る
// 

class EnemyManager final
{
public:
	EnemyManager();
	~EnemyManager();

private:
	std::vector<IEnemy> m_enemies;
};
