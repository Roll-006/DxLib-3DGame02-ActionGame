#include "melee_target_icon.hpp"
#include "../Base/character_base.hpp"
#include "../Command/command_handler.hpp"

MeleeTargetIcon::MeleeTargetIcon(std::shared_ptr<IMeleeHittable>& melee_target) : 
	m_melee_target	(melee_target),
	m_button_graphic(std::make_shared<ButtonGraphicGetter>()),
	m_icon_graphic	(nullptr),
	m_icon_pos		(v3d::GetZeroV()),
	m_is_draw_icon	(false)
{

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
		m_icon_graphic = m_button_graphic->GetWeaponGraphicer(command->GetKeyInputCode(CommandKind::kMelee, CommandSlotKind::kMain));
		break;

	case DeviceKind::kPad:
		m_icon_graphic = m_button_graphic->GetWeaponGraphicer(command->GetPadInputCode(CommandKind::kMelee, CommandSlotKind::kStatic1));
		break;
	}
}

void MeleeTargetIcon::Draw() const
{
	if (m_is_draw_icon)
	{
		DrawBillboard3D(m_icon_pos + kOffset, 0.5f, 0.5f, 10.0f, 0.0f, m_icon_graphic->GetGraphicHandle(), TRUE);
	}
}
