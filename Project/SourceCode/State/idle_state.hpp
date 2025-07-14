#pragma once
#include "../Interface/i_state.hpp"

class Player;

class IdleState final : public IState<Player>
{
public:
	IdleState();
	~IdleState() override;

	void Update(const Player* obj) override;
	void Change(const Player* obj) override;

private:

};