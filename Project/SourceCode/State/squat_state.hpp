#pragma once
#include "../Interface/i_state.hpp"

class Player;

class SquatState final : public IState<Player>
{
public:
	SquatState();
	~SquatState() override;

	void Update(const Player* obj) override;
	void Change(const Player* obj) override;

private:

};