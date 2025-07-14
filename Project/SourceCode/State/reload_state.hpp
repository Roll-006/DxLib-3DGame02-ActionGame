#pragma once
#include "../Interface/i_state.hpp"
#include "../Object/player.hpp"

class ReloadState final : public IState<Player>
{
public:
	ReloadState();
	~ReloadState() override;

	void Update		(const Player* obj) override;
	void ChangeState(const Player* obj) override;

private:

};
