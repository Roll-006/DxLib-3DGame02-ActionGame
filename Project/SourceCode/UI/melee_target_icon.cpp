#include "melee_target_icon.hpp"

#include "../Base/character_base.hpp"
#include "../Command/command_handler.hpp"
#include "../VirtualCamera/cinemachine_brain.hpp"

MeleeTargetIcon::MeleeTargetIcon(std::shared_ptr<IMeleeHittable>& melee_target) : 
	m_melee_target				(melee_target),
	m_button_graphic_resource	(std::make_shared<ButtonGraphicGetter>()),
	m_button_icon_graphic		(nullptr),
	m_cursor_graphic			(std::make_shared<Graphicer>(UIGraphicPath.CURSOR_01)),
	m_result_screen				(std::make_shared<ScreenCreator>(kScreenSize)),
	m_icon_pos					(v3d::GetZeroV()),
	m_icon_size					(0.0f),
	m_is_draw_icon				(false)
{
	m_cursor_graphic->SetCenterPos(kScreenCenterPos + kCursorOffset);
	m_cursor_graphic->SetScale(0.5f);
}

MeleeTargetIcon::~MeleeTargetIcon()
{

}

void MeleeTargetIcon::LateUpdate()
{
	m_is_draw_icon = false;

	if (m_melee_target)
	{
		if (m_melee_target->IsStandStun() || m_melee_target->IsCrouchStun())
		{
			m_is_draw_icon = true;

			const auto	model_handle = std::dynamic_pointer_cast<CharacterBase>(m_melee_target)->GetModeler()->GetModelHandle();
			auto		head_m		 = MV1GetFrameLocalWorldMatrix(model_handle, MV1SearchFrame(model_handle, BonePath.HEAD));
			
			m_icon_pos = MGetTranslateElem(head_m);
		}
	}

	// 入力デバイスおよびキー割り当てに対応した画像を取得
	const auto command = CommandHandler::GetInstance();
	switch (InputChecker::GetInstance()->GetCurrentInputDevice())
	{
	case DeviceKind::kKeyboard:
		m_button_icon_graphic = m_button_graphic_resource->GetWeaponGraphicer(command->GetKeyInputCode(CommandKind::kMelee, CommandSlotKind::kMain));
		m_button_icon_graphic->SetCenterPos(kScreenCenterPos);
		m_button_icon_graphic->SetScale(0.5f);
		break;

	case DeviceKind::kPad:
		m_button_icon_graphic = m_button_graphic_resource->GetWeaponGraphicer(command->GetPadInputCode(CommandKind::kMelee, CommandSlotKind::kStatic1));
		m_button_icon_graphic->SetCenterPos(kScreenCenterPos);
		m_button_icon_graphic->SetScale(0.5f);
		break;
	}

	CalcIcconSize();
	CreateResultScreen();
}

void MeleeTargetIcon::Draw() const
{
	if (m_is_draw_icon)
	{
		DrawBillboard3D(m_icon_pos + kOffset, 0.5f, 0.5f, kIconSize, 0.0f, m_result_screen->GetScreenHandle(), TRUE);
	}
}

void MeleeTargetIcon::CalcIcconSize()
{

}

void MeleeTargetIcon::CreateResultScreen()
{
	m_result_screen->UseScreen();

	m_button_icon_graphic->Draw();
	m_cursor_graphic->Draw();

	m_result_screen->UnuseScreen();
}
