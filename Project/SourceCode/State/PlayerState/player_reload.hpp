#pragma once
#include "../../Base/player_state_base.hpp"

class Animator;

namespace player_state
{
	class Reload final : public PlayerStateBase
	{
	public:
		Reload(Player& player, player_state::State& state, const std::shared_ptr<Animator>& animator);
		~Reload();

		void Update()		override;
		void LateUpdate()	override;
		void Enter()		override;
		void Exit()			override;

		[[nodiscard]] const PlayerStateKind GetNextStateKind() override;

	private:
		bool m_is_release_ammo_box;
		bool m_is_set_ammo_box;
		bool m_is_cocking;
	};
}
