#pragma once
#include "../../Base/weapon_action_state_base.hpp"

#include "../../Object/player.hpp"
#include "../../Part/player_state_controller.hpp"

namespace player_state
{
	class FirstSideSlashKnife final : public WeaponActionStateBase<Player>
	{
	public:
		FirstSideSlashKnife();
		~FirstSideSlashKnife() override;

		void Update		(std::shared_ptr<Player> obj) override;
		void LateUpdate	(std::shared_ptr<Player> obj) override;
		void Enter		(std::shared_ptr<Player> obj) override;
		void Exit		(std::shared_ptr<Player> obj) override;

		[[nodiscard]] std::shared_ptr<IState<Player>> ChangeState(std::shared_ptr<Player> obj) override;
		[[nodiscard]] bool IsAllStop() const override { return m_is_all_stop; }

	private:
		static constexpr float kComboValidTime = 0.7f;	// “ñ’iŠK–Ú‚ÌUŒ‚‚ª—LŒø‚É‚È‚éŠÔ

		float m_combo_timer;
		bool  m_is_all_stop;
	};
}
