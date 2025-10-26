#include "stealth_kill_target_icon.hpp"

#include "../Base/character_base.hpp"
#include "../Command/command_handler.hpp"

StealthKillTargetIcon::StealthKillTargetIcon(std::shared_ptr<IStealthKillable>& stealth_kill_target) :
	m_stealth_kill_target		(stealth_kill_target),
	m_button_graphic_resource	(std::make_shared<ButtonGraphicGetter>()),
	m_button_icon_graphic		(nullptr),
	m_knife_graphic				(std::make_shared<Graphicer>(UIGraphicPath.KNIFE)),
	m_stealth_kill_icon_screen	(std::make_shared<ScreenCreator>(kScreenSize)),
	m_icon_pos					(v3d::GetZeroV()),
	m_icon_size					(0.0f)
{
	m_knife_graphic->SetCenterPos(kScreenCenterPos - kGraphicOffset);
	m_knife_graphic->SetScale(0.12f);
}

StealthKillTargetIcon::~StealthKillTargetIcon()
{

}

void StealthKillTargetIcon::LateUpdate()
{
	CalcResultScreenCenterPos();
	CreateStealthKillIconScreen();
}

void StealthKillTargetIcon::Draw(const int main_screen_handle) const
{
	if (m_stealth_kill_target)
	{
		DrawBillboard3D(m_icon_pos, 0.5f, 0.5f, m_icon_size, 0.0f, m_stealth_kill_icon_screen->GetScreenHandle(), TRUE);
	}
}

void StealthKillTargetIcon::CalcResultScreenCenterPos()
{
	const auto camera_pos	= GetCameraPosition();
	const auto distance		= VSize(m_icon_pos - camera_pos);

	// DrawBillboard3D関数は距離に応じて描画サイズが変更されるため
	// 距離に応じて拡大する
	m_icon_size = kIconSize * distance * 0.01f;
}

void StealthKillTargetIcon::CreateStealthKillIconScreen()
{
	if (!m_stealth_kill_target) { return; }

	const auto	model_handle = std::dynamic_pointer_cast<CharacterBase>(m_stealth_kill_target)->GetModeler()->GetModelHandle();
	auto		spine2_m		 = MV1GetFrameLocalWorldMatrix(model_handle, MV1SearchFrame(model_handle, BonePath.SPINE_2));

	m_icon_pos = MGetTranslateElem(spine2_m) + kIconOffset;

	// 入力デバイスおよびキー割り当てに対応した画像を取得
	const auto command = CommandHandler::GetInstance();
	switch (InputChecker::GetInstance()->GetCurrentInputDevice())
	{
	case DeviceKind::kKeyboard:
		m_button_icon_graphic = m_button_graphic_resource->GetWeaponGraphicer(command->GetKeyInputCode(CommandKind::kAttack, CommandSlotKind::kMain));
		m_button_icon_graphic->SetCenterPos(kScreenCenterPos + kGraphicOffset);
		m_button_icon_graphic->SetScale(0.18f);
		break;

	case DeviceKind::kPad:
		m_button_icon_graphic = m_button_graphic_resource->GetWeaponGraphicer(command->GetPadInputCode(CommandKind::kAttack, CommandSlotKind::kStatic1));
		m_button_icon_graphic->SetCenterPos(kScreenCenterPos + kGraphicOffset);
		m_button_icon_graphic->SetScale(0.13f);
		break;
	}

	m_stealth_kill_icon_screen->UseScreen();
	m_button_icon_graphic->Draw();
	m_knife_graphic		 ->Draw();
	m_stealth_kill_icon_screen->UnuseScreen();
}
