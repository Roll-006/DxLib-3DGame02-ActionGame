#pragma once
#include "../Interface/i_state.hpp"

class Player;

class AimingState final : public IState<Player>
{
public:
	AimingState();
	~AimingState() override;

	void Update(const Player* obj) override;
	void Change(const Player* obj) override;

private:

};