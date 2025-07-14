#pragma once
#include "../Interface/i_state.hpp"
#include "../Object/player.hpp"

class ShotState final : public IState<Player>
{
public:
	ShotState();
	~ShotState() override;

	void Update(const Player* obj) override;
	void Enter (const Player* obj) override;
	[[nodiscard]] IState<Player>* ChangeState(const Player* obj) override;

private:

};
