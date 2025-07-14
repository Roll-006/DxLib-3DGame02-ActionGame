#pragma once
#include "../Interface/i_state.hpp"
#include "../Object/player.hpp"

class NonWeaponState final : public IState<Player>
{
public:
	NonWeaponState();
	~NonWeaponState() override;

	void Update		(const Player* obj) override;
	void ChangeState(const Player* obj) override;

private:

};
