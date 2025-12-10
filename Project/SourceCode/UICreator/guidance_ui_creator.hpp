#pragma once
#include "../Interface/i_ui_creator.hpp"
#include "../UI/mission_ui.hpp"
#include "../Event/event_system.hpp"

class GuidanceUICreator final : public IUICreator
{
public:
	GuidanceUICreator();
	~GuidanceUICreator() override;

	void Init()			override;
	void LateUpdate()	override;
	void OnDraw(const int main_screen_handle) override;

	/// @brief アクティブ化する
	void Activate() 	override { m_is_active = true; }
	/// @brief 非アクティブ化する(削除せずに機能を停止)
	void Deactivate()	override { m_is_active = false; }

	[[nodiscard]] std::string GetName()		const override { return m_name; }
	[[nodiscard]] int		  GetPriority() const override { return m_priority; }
	[[nodiscard]] bool		  IsActive()	const override { return m_is_active; }

private:
	void ActivateMissionUI(const ChangeSceneEvent& event);

private:
	std::string	m_name;
	int			m_priority;
	bool		m_is_active;

	std::shared_ptr<MissionUI> m_mission_ui;
};
