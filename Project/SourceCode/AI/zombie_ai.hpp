#pragma once
#include "../Interface/i_enemy_ai.hpp"

#include "../Part/zombie_state_controller.hpp"
#include "../Object/zombie.hpp"
#include "../Object/player.hpp"

class ZombieAI final : public IEnemyAI<Zombie>
{
public:
	ZombieAI(const std::shared_ptr<Player> player);
	~ZombieAI() override;

	void Update		(const std::shared_ptr<Zombie> enemy) override;
	void LateUpdate	(const std::shared_ptr<Zombie> enemy) override;

	[[nodiscard]] std::shared_ptr<EnemyStateControllerBase<Zombie>> GetStateController() const { return m_state; }

private:
	std::shared_ptr<EnemyStateControllerBase<Zombie>>	m_state;
	std::shared_ptr<Player>								m_player;
};
