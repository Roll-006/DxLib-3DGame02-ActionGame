#pragma once

#include "../AI/EnemyAI/wait.hpp"
#include "../AI/EnemyAI/patrol.hpp"
#include "../AI/EnemyAI/investigate.hpp"
#include "../AI/EnemyAI/search.hpp"
#include "../AI/EnemyAI/alert.hpp"
#include "../AI/EnemyAI/track.hpp"
#include "../AI/EnemyAI/battle.hpp"

class ZombieAIController
{
public:
	ZombieAIController();
	~ZombieAIController();

	void Update();

private:

};
