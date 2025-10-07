#pragma once
#include "../../Base/weapon_action_state_base.hpp"

#include "../../Object/player.hpp"
#include "../../Part/player_state_controller.hpp"

namespace player_state
{
	class SpinningSlashKnife final : public WeaponActionStateBase<Player>
	{
	public:
		SpinningSlashKnife();
		~SpinningSlashKnife() override;

		void Update		(std::shared_ptr<Player>& obj) override;
		void LateUpdate	(std::shared_ptr<Player>& obj) override;
		void Enter		(std::shared_ptr<Player>& obj) override;
		void Exit		(std::shared_ptr<Player>& obj) override;

		[[nodiscard]] std::shared_ptr<IState<Player>> ChangeState(std::shared_ptr<Player>& obj) override;
		[[nodiscard]] bool IsStopAllState() const override { return m_is_stop_all_state; }

	private:
		static constexpr float kComboValidTime = 1.0f;	// “ñ’iŠK–Ú‚ÌUŒ‚‚ª—LŒø‚É‚È‚éŠÔ

		float m_combo_timer;
		bool  m_is_stop_all_state;
	};
}
