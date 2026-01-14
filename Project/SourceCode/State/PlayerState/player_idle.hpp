#pragma once
#include "../../Base/player_state_base.hpp"

class Animator;

namespace player_state
{
	class Idle final : public PlayerStateBase
	{
	public:
		Idle(Player& player, player_state::State& state, const std::shared_ptr<Animator>& animator);
		~Idle();

		void Update()		override;
		void LateUpdate()	override;
		void Enter()		override;
		void Exit()			override;

		[[nodiscard]] const PlayerStateKind GetNextStateKind() override;

	private:
		float m_non_move_time;
	};
}
