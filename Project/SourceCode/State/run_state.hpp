#pragma once
#include "../Interface/i_state.hpp"

class Player;

class RunState final : public IState<Player>
{
public:
	RunState();
	~RunState() override;

	void Update(const Player* obj) override;
	void Change(const Player* obj) override;

private:

};