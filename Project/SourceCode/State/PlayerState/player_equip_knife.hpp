#pragma once
#include "../../Base/player_state_base.hpp"

class Animator;

namespace player_state
{
	class EquipKnife final : public PlayerStateBase
	{
	public:
		EquipKnife(Player& player, player_state::State& state, const std::shared_ptr<Animator>& animator);
		~EquipKnife();

		void Update()		override;
		void LateUpdate()	override;
		void Enter()		override;
		void Exit()			override;

		[[nodiscard]] const PlayerStateKind GetNextStateKind() override;

	private:
		static constexpr float kReleaseKnifeForciblyTime = 5.0f;

		float m_elapsed_time;
	};
}
