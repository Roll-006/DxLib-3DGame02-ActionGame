#include "guidance_ui_creator.hpp"

GuidanceUICreator::GuidanceUICreator() :
	m_name		(UICreatorName.GUIDANCE_UI_CREATOR),
	m_priority	(10),
	m_is_active	(true),
	m_mission_ui(std::make_shared<MissionUI>())
{
	// ƒCƒxƒ“ƒg“o˜^
	//EventSystem::GetInstance()->Subscribe<ChangeSceneEvent>(this, &GuidanceUICreator::ActivateMissionUI);
}

GuidanceUICreator::~GuidanceUICreator()
{
	// ƒCƒxƒ“ƒg‚Ì“o˜^‰ðœ
	//EventSystem::GetInstance()->Unsubscribe<ChangeSceneEvent>(this, &GuidanceUICreator::ActivateMissionUI);
}

void GuidanceUICreator::Init()
{
	m_is_active = true;

	m_mission_ui->Init();
}

void GuidanceUICreator::LateUpdate()
{
	if (!m_is_active) { return; }

	m_mission_ui->LateUpdate();
}

void GuidanceUICreator::OnDraw(const int main_screen_handle)
{
	if (!m_is_active) { return; }

	m_mission_ui->Draw();
}

void GuidanceUICreator::ActivateMissionUI(const ChangeSceneEvent& event)
{
	switch (event.next_scene_kind)
	{
	case SceneKind::kPlay:
		m_mission_ui->Activate();
		break;

	default:
		break;
	}
}
