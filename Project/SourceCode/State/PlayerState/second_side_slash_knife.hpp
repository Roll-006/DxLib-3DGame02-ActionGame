#pragma once
#include "../../Base/weapon_action_state_base.hpp"

#include "../../Object/player.hpp"
#include "../../Part/player_state_controller.hpp"

namespace player_state
{
	class SecondSideSlashKnife final : public WeaponActionStateBase
	{
	public:
		SecondSideSlashKnife(Player& player);
		~SecondSideSlashKnife() override;

		void Update() override;
		void LateUpdate() override;
		void Enter() override;
		void Exit() override;

		[[nodiscard]] int GetNextStateKind() override;
		[[nodiscard]] bool IsStopAllState() const override { return m_is_stop_all_state; }

	private:
		static constexpr float kComboValidTime = 0.7f;	// 一段階目の攻撃が有効になる時間

		float m_combo_timer;
		bool  m_is_stop_all_state;
		bool  m_has_trigger_created;
		bool  m_has_trigger_deleted;
		Player& m_player;
	};
}
