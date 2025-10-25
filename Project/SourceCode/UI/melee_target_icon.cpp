#include "melee_target_icon.hpp"

#include "../Base/character_base.hpp"
#include "../Command/command_handler.hpp"

MeleeTargetIcon::MeleeTargetIcon(std::shared_ptr<IMeleeHittable>& melee_target, std::shared_ptr<IMeleeHittable>& visible_downed_character) :
	m_melee_target				(melee_target),
	m_visible_downed_character	(visible_downed_character),
	m_button_graphic_resource	(std::make_shared<ButtonGraphicGetter>()),
	m_button_icon_graphic		(nullptr),
	m_melee_cursor_graphic		(std::make_shared<Graphicer>(UIGraphicPath.CURSOR_01)),
	m_down_cursor_graphic		(std::make_shared<Graphicer>(UIGraphicPath.CURSOR_02)),
	m_melee_icon_screen			(std::make_shared<ScreenCreator>(kScreenSize)),
	m_icon_pos					(v3d::GetZeroV()),
	m_icon_size					(0.0f)
{
	m_melee_cursor_graphic->SetCenterPos(kScreenCenterPos + kCursorOffset);
	m_melee_cursor_graphic->SetScale(0.2f);

	m_down_cursor_graphic->SetCenterPos(kScreenCenterPos);
	m_down_cursor_graphic->SetScale(0.2f);
}

MeleeTargetIcon::~MeleeTargetIcon()
{

}

void MeleeTargetIcon::LateUpdate()
{
	CalcResultScreenCenterPos();
	CreateMeleeIconScreen();
	CreateDownIconScreen();
}

void MeleeTargetIcon::Draw() const
{
	if (!m_melee_target && !m_visible_downed_character) { return; }

	DrawBillboard3D(m_icon_pos, 0.5f, 0.5f, m_icon_size, 0.0f, m_melee_icon_screen->GetScreenHandle() ,TRUE);
}

void MeleeTargetIcon::CalcResultScreenCenterPos()
{
	const auto camera_pos	= GetCameraPosition();
	const auto distance		= VSize(m_icon_pos - camera_pos);

	// DrawBillboard3D関数は距離に応じて描画サイズが変更されるため
	// 距離に応じて拡大する
	m_icon_size = kIconSize * distance * 0.01f;
}

void MeleeTargetIcon::CreateMeleeIconScreen()
{
	if (!m_melee_target) { return; }

	const auto	model_handle = std::dynamic_pointer_cast<CharacterBase>(m_melee_target)->GetModeler()->GetModelHandle();
	auto		head_m		 = MV1GetFrameLocalWorldMatrix(model_handle, MV1SearchFrame(model_handle, BonePath.HEAD));

	m_icon_pos = MGetTranslateElem(head_m) + kIconOffset;

	// 入力デバイスおよびキー割り当てに対応した画像を取得
	const auto command = CommandHandler::GetInstance();
	switch (InputChecker::GetInstance()->GetCurrentInputDevice())
	{
	case DeviceKind::kKeyboard:
		m_button_icon_graphic = m_button_graphic_resource->GetWeaponGraphicer(command->GetKeyInputCode(CommandKind::kMelee, CommandSlotKind::kMain));
		m_button_icon_graphic->SetCenterPos(kScreenCenterPos);
		m_button_icon_graphic->SetScale(0.2f);
		break;

	case DeviceKind::kPad:
		m_button_icon_graphic = m_button_graphic_resource->GetWeaponGraphicer(command->GetPadInputCode(CommandKind::kMelee, CommandSlotKind::kStatic1));
		m_button_icon_graphic->SetCenterPos(kScreenCenterPos);
		m_button_icon_graphic->SetScale(0.2f);
		break;
	}

	m_melee_icon_screen		->UseScreen();
	m_button_icon_graphic	->Draw();
	m_melee_cursor_graphic	->Draw();
	m_melee_icon_screen		->UnuseScreen();
}

void MeleeTargetIcon::CreateDownIconScreen()
{
	if (!m_visible_downed_character) { return; }

	const auto	model_handle = std::dynamic_pointer_cast<CharacterBase>(m_visible_downed_character)->GetModeler()->GetModelHandle();
	auto		head_m = MV1GetFrameLocalWorldMatrix(model_handle, MV1SearchFrame(model_handle, BonePath.HEAD));

	m_icon_pos = MGetTranslateElem(head_m) + kIconOffset;

	m_melee_icon_screen		->UseScreen();
	m_down_cursor_graphic	->Draw();
	m_melee_icon_screen		->UnuseScreen();
}
