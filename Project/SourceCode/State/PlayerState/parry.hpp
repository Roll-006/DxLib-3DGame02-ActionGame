#pragma once
#include "../../Interface/i_weapon_action_state.hpp"
#include "../../Object/player.hpp"

namespace player_state
{
	class Parry final : public IWeaponActionState<Player>
	{
	public:
		Parry();
		~Parry() override;

		void Update(const Player* obj) override;
		void Enter (const Player* obj) override;
		[[nodiscard]] IState<Player>* ChangeState(const Player* obj) override;

	private:

	};
}
