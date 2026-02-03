#pragma once
#include "../Interface/i_ui_creator.hpp"
#include "../UI/mission_ui.hpp"
#include "../UI/purpose_ui.hpp"
#include "../Event/event_system.hpp"
#include "../UI/guidance_ui_holder.hpp"

class GuidanceUICreator final : public IUICreator
{
public:
	GuidanceUICreator();
	~GuidanceUICreator() override;

	void Init()			override;
	void LateUpdate()	override;
	void OnDraw(const int main_screen_handle) override;

	void Activate() 	override { m_is_active = true; }
	void Deactivate()	override { m_is_active = false; }

	[[nodiscard]] const std::string GetName()		const override { return m_name; }
	[[nodiscard]] const int			GetPriority()	const override { return m_priority; }
	[[nodiscard]] const bool		IsActive()		const override { return m_is_active; }

private:
	void ActivateMissionUI(const ChangeSceneEvent& event);

private:
	std::string	m_name;
	int			m_priority;
	bool		m_is_active;

	std::shared_ptr<MissionUI>			m_mission_ui;
	std::shared_ptr<PurposeUI>			m_purpose_ui;
	std::shared_ptr<GuidanceUIHolder>	m_guidance_ui_holder;
};
