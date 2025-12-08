#include "guidance_ui_creator.hpp"

GuidanceUICreator::GuidanceUICreator() :
	m_name		(UICreatorName.GUIDANCE_UI_CREATOR),
	m_priority	(10),
	m_is_active	(true)
{
	// ƒCƒxƒ“ƒg“o˜^
	//EventSystem::GetInstance()->Subscribe<DeadPlayerEvent>(this, &PlayerUICreator::Deactivate);
}

GuidanceUICreator::~GuidanceUICreator()
{
	// ƒCƒxƒ“ƒg‚Ì“o˜^‰ðœ
	//EventSystem::GetInstance()->Unsubscribe<DeadPlayerEvent>(this, &PlayerUICreator::Deactivate);
}

void GuidanceUICreator::Init()
{
	m_is_active = true;
}

void GuidanceUICreator::LateUpdate()
{
	if (!m_is_active) { return; }
}

void GuidanceUICreator::OnDraw(const int main_screen_handle)
{
	if (!m_is_active) { return; }
}
