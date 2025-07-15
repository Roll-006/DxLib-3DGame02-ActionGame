#pragma once
#include "../../Interface/i_weapon_action_state.hpp"
#include "../../Object/player.hpp"

namespace player_state
{
	class StabKnife final : public IWeaponActionState<Player>
	{
	public:
		StabKnife();
		~StabKnife() override;

		void Update(const Player* obj) override;
		void Enter (const Player* obj) override;
		[[nodiscard]] std::shared_ptr<IState<Player>> ChangeState(const Player* obj) override;

	private:

	};
}
