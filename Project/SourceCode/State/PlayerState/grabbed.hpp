#pragma once
#include "../../Base/action_state_base.hpp"

#include "../../Object/player.hpp"
#include "../../Part/player_state_controller.hpp"
#include "../../Part/subject.hpp"

#include "../../VirtualCamera/grab_virtual_camera_controller.hpp"

namespace player_state
{
	class Grabbed final : public ActionStateBase<Player>
	{
	public:
		Grabbed();
		~Grabbed() override;

		void Update		(std::shared_ptr<Player> obj) override;
		void LateUpdate	(std::shared_ptr<Player> obj) override;
		void Enter		(std::shared_ptr<Player> obj) override;
		void Exit		(std::shared_ptr<Player> obj) override;

		[[nodiscard]] std::shared_ptr<IState<Player>> ChangeState(std::shared_ptr<Player> obj) override;

	private:
		std::shared_ptr<Subject<Grabbed>>				m_subject;
		std::shared_ptr<GrabVirtualCameraController>	m_grab_camera_controller;
	};
}
