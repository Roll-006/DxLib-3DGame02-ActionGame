#pragma once
#include "../../Interface/i_state.hpp"
#include "../../Object/player.hpp"

namespace player_state
{
	class GunEquipped final : public IState<Player>
	{
	public:
		GunEquipped();
		~GunEquipped() override;

		void Update(const Player* obj) override;
		void Enter(const Player* obj) override;
		[[nodiscard]] IState<Player>* ChangeState(const Player* obj) override;

	private:

	};
}
