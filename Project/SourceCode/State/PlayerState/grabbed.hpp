#pragma once
#include "../../Base/action_state_base.hpp"

#include "../../Object/player.hpp"
#include "../../Part/player_state_controller.hpp"

namespace player_state
{
	class Grabbed final : public ActionStateBase<Player>
	{
	public:
		Grabbed();
		~Grabbed() override;

		void Update		(std::shared_ptr<Player>& obj) override;
		void LateUpdate	(std::shared_ptr<Player>& obj) override;
		void Enter		(std::shared_ptr<Player>& obj) override;
		void Exit		(std::shared_ptr<Player>& obj) override;

		[[nodiscard]] std::shared_ptr<IState<Player>> ChangeState(std::shared_ptr<Player>& obj) override;
		[[nodiscard]] bool IsStopAllState() const override { return m_is_stop_all_state; }

	private:
		void Escape(std::shared_ptr<Player>& obj);

	private:
		static constexpr float kEscapeStartTime = 1.0f;		// ’Eo‚ğŠJn‚·‚é‚Ü‚Å‚ÌŠÔ
		static constexpr float kEscapeMinTime	= 1.8f;		// ’Eo‚ÌÅ’ZŠÔ
		static constexpr int   kEscapeInputNum	= 16;		// ˜A‘Å’Eo‚Ì“ü—Í‰ñ”

		bool					m_is_stop_all_state;
		float					m_escape_start_timer;
	};
}
