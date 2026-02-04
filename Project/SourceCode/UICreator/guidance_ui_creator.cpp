#include "guidance_ui_creator.hpp"

GuidanceUICreator::GuidanceUICreator() :
	m_name					(UICreatorName.GUIDANCE_UI_CREATOR),
	m_priority				(10),
	m_is_active				(true),
	m_mission_ui			(std::make_shared<MissionUI>("mission")),
	m_purpose_ui			(std::make_shared<PurposeUI>("purpose_01")),
	//m_active_guidance_ui	(nullptr),
	m_guidance_ui_activator	(std::make_shared<GuidanceUIActivator>())
{
	// イベント登録
	EventSystem::GetInstance()->Subscribe<ChangeSceneEvent>(this, &GuidanceUICreator::ActivateMissionUI);
}

GuidanceUICreator::~GuidanceUICreator()
{
	// イベントの登録解除
	EventSystem::GetInstance()->Unsubscribe<ChangeSceneEvent>(this, &GuidanceUICreator::ActivateMissionUI);
}

void GuidanceUICreator::Init()
{
	m_is_active = true;

	m_mission_ui->Init();
	m_purpose_ui->Init();
}

void GuidanceUICreator::LateUpdate()
{
	if (!m_is_active) { return; }

	m_mission_ui			->LateUpdate();
	m_purpose_ui			->LateUpdate();
	m_guidance_ui_activator	->LateUpdate();
}

void GuidanceUICreator::OnDraw(const int main_screen_handle)
{
	if (!m_is_active) { return; }

	m_mission_ui->Draw();
	m_purpose_ui->Draw(main_screen_handle);

	// FIXME : 仮
	const auto active_guidance_ui = m_guidance_ui_activator->GetActiveGaidanceUI();
	if (active_guidance_ui)
	{
		active_guidance_ui->Draw(main_screen_handle);
	}

	//if (m_active_guidance_ui)
	//{
	//	m_active_guidance_ui->Draw(main_screen_handle);
	//}
}

void GuidanceUICreator::ActivateMissionUI(const ChangeSceneEvent& event)
{
	switch (event.next_scene_kind)
	{
	case SceneKind::kPlay:
		m_mission_ui->Activate();
		m_purpose_ui->Activate();
		break;

	default:
		break;
	}
}
