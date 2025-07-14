#pragma once
#include "../Interface/i_state.hpp"
#include "../Object/player.hpp"

class KnifeEquippedState final : public IState<Player>
{
public:
	KnifeEquippedState();
	~KnifeEquippedState() override;

	void Update(const Player* obj) override;
	void Enter (const Player* obj) override;
	[[nodiscard]] IState<Player>* ChangeState(const Player* obj) override;

private:

};
