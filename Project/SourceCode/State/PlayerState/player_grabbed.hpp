#pragma once
#include "../../Base/player_state_base.hpp"

class Animator;

namespace player_state
{
	class Grabbed final : public PlayerStateBase
	{
	public:
		Grabbed(Player& player, player_state::State& state, const std::shared_ptr<Animator>& animator);
		~Grabbed();

		void Update()		override;
		void LateUpdate()	override;
		void Enter()		override;
		void Exit()			override;

		[[nodiscard]] const PlayerStateKind GetNextStateKind() override;
	};
}
