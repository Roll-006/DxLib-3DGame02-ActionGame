#pragma once
#include "../../Base/weapon_action_state_base.hpp"

#include "../../Object/player.hpp"
#include "../../Part/player_state_controller.hpp"

namespace player_state
{
	class SecondSideSlashKnife final : public WeaponActionStateBase<Player>
	{
	public:
		SecondSideSlashKnife();
		~SecondSideSlashKnife() override;

		void Update		(Player* obj) override;
		void LateUpdate	(Player* obj) override;
		void Enter		(Player* obj) override;
		void Exit		(Player* obj) override;
		[[nodiscard]] std::shared_ptr<IState<Player>> ChangeState(const Player* obj) override;

	private:
		static constexpr float kComboValidTime = 0.7f;	// ˆê’iŠK–Ú‚ÌUŒ‚‚ª—LŒø‚É‚È‚éŠÔ

		float m_combo_timer;
	};
}
