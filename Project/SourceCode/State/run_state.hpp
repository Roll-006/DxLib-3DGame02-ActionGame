#pragma once
#include "../Interface/i_state.hpp"
#include "../Object/player.hpp"

class RunState final : public IState<Player>
{
public:
	RunState();
	~RunState() override;

	void Update		(const Player* obj) override;
	void ChangeState(const Player* obj) override;

private:

};
