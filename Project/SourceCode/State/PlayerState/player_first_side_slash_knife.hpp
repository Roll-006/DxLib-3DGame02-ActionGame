#pragma once
#include "../../Base/player_state_base.hpp"

class Animator;

namespace player_state
{
	class FirstSideSlashKnife final : public PlayerStateBase
	{
	public:
		FirstSideSlashKnife(Player& player, player_state::State& state, const std::shared_ptr<Animator>& animator);
		~FirstSideSlashKnife();

		void Update()		override;
		void LateUpdate()	override;
		void Enter()		override;
		void Exit()			override;

		[[nodiscard]] const PlayerStateKind GetNextStateKind() override;

	private:
		static constexpr float kComboValidTime = 0.7f;

		float m_combo_timer;
		bool  m_has_trigger_created;
		bool  m_has_trigger_deleted;
	};
}
