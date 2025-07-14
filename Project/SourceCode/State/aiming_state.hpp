#pragma once
#include "../Interface/i_state.hpp"
#include "../Object/player.hpp"

class AimingState final : public IState<Player>
{
public:
	AimingState();
	~AimingState() override;

	void Update(const Player* obj) override;
	void Enter (const Player* obj) override;
	[[nodiscard]] IState<Player>* ChangeState(const Player* obj) override;

private:

};
