#pragma once
#include "../Interface/i_state.hpp"
#include "../Object/player.hpp"

class MoveState final : public IState<Player>
{
public:
	MoveState();
	~MoveState() override;

	void Update		(const Player* obj) override;
	void ChangeState(const Player* obj) override;

private:

};
