#pragma once
#include "../../Base/player_state_base.hpp"

class Animator;

namespace player_state
{
	class FrontKick final : public PlayerStateBase
	{
	public:
		FrontKick(Player& player, player_state::State& state, const std::shared_ptr<Animator>& animator);
		~FrontKick();

		void Update()		override;
		void LateUpdate()	override;
		void Enter()		override;
		void Exit()			override;

		[[nodiscard]] const PlayerStateKind GetNextStateKind() override;

	private:
		static constexpr float kAttackTriggerRadius = 19.0f;

		bool m_has_trigger_created;
		bool m_has_trigger_deleted;
	};
}
