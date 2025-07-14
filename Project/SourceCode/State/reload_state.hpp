#pragma once
#include "../Interface/i_state.hpp"

class Player;

class ReloadState final : public IState<Player>
{
public:
	ReloadState();
	~ReloadState() override;

	void Update(const Player* obj) override;
	void Change(const Player* obj) override;

private:

};
