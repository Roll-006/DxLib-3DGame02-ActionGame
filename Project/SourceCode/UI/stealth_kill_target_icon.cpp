#include "stealth_kill_target_icon.hpp"

#include "../Base/character_base.hpp"
#include "../Command/command_handler.hpp"

StealthKillTargetIcon::StealthKillTargetIcon(std::shared_ptr<IStealthKillable>& stealth_kill_target) :
	m_stealth_kill_target		(stealth_kill_target),
	m_button_graphic_resource	(std::make_shared<ButtonGraphicGetter>()),
	m_button_icon_graphic		(nullptr),
	m_knife_graphic				(std::make_shared<Graphicer>(UIGraphicPath.KNIFE)),
	m_stealth_kill_icon_screen	(std::make_shared<ScreenCreator>(kScreenSize)),
	m_mask_graphic				(std::make_shared<Graphicer>(UIGraphicPath.STEALTH_KILL_CIRCLE_BLUR)),
	m_mask_screen				(std::make_shared<ScreenCreator>(Window::kScreenSize)),
	m_mask_creator				(std::make_shared<MaskCreator>()),
	m_icon_pos					(v3d::GetZeroV()),
	m_icon_size					(0.0f)
{
	m_knife_graphic->SetCenterPos(kScreenCenterPos - kGraphicOffset);
	m_knife_graphic->SetScale(0.12f);

	m_mask_graphic->SetScale(1.4f);
}

StealthKillTargetIcon::~StealthKillTargetIcon()
{

}

void StealthKillTargetIcon::LateUpdate()
{
	CreateStealthKillIconScreen();
	CalcResultScreenCenterPos();
}

void StealthKillTargetIcon::Draw(const int main_screen_handle) const
{
	if (m_stealth_kill_target)
	{
		m_mask_screen->UseScreen();
		m_mask_graphic->Draw();
		m_mask_screen->UnuseScreen();

		m_mask_creator->CreateMask();
		m_mask_creator->UseMask(m_mask_screen->GetScreenHandle(), true);
		DrawGraph(0, 0, main_screen_handle, TRUE);
		SetDrawBlendMode(DX_BLENDMODE_ALPHA, 150);
		DrawBox(0, 0, m_mask_screen->GetScreenSize().x, m_mask_screen->GetScreenSize().y, 0x000000, TRUE);
		SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
		m_mask_creator->UnuseMask();
		m_mask_creator->DeleteMask();

		m_stealth_kill_icon_screen->Draw();
	}
}

void StealthKillTargetIcon::CalcResultScreenCenterPos()
{
	const auto camera_pos	= GetCameraPosition();
	const auto distance		= VSize(m_icon_pos - camera_pos);

	// DrawBillboard3D関数は距離に応じて描画サイズが変更されるため
	// 距離に応じて拡大する
	m_icon_size = kIconSize * distance * 0.01f;

	// 3D座標をスクリーン上に変換し適用
	const auto screen_pos_v3d = ConvWorldPosToScreenPos(m_icon_pos);
	const auto screen_pos_v2d = Vector2D<int>(screen_pos_v3d.x, screen_pos_v3d.y);
	m_stealth_kill_icon_screen->GetGraphicer()->SetCenterPos(screen_pos_v2d);

	m_mask_graphic->SetCenterPos(screen_pos_v2d - kGraphicOffset);
}

void StealthKillTargetIcon::CreateStealthKillIconScreen()
{
	if (!m_stealth_kill_target) { return; }

	const auto	model_handle = std::dynamic_pointer_cast<const CharacterBase>(m_stealth_kill_target)->GetModeler()->GetModelHandle();
	auto		spine2_m		 = MV1GetFrameLocalWorldMatrix(model_handle, MV1SearchFrame(model_handle, FramePath.SPINE_2));

	m_icon_pos = matrix::GetPos(spine2_m) + kIconOffset;

	// 入力デバイスおよびキー割り当てに対応した画像を取得
	const auto command	= CommandHandler::GetInstance();
	const auto device	= InputChecker::GetInstance()->GetCurrentInputDevice();
	switch (device)
	{
	case DeviceKind::kKeyboard:
		m_button_icon_graphic = m_button_graphic_resource->GetButtonGraphicer(command->GetInputCode(device, CommandKind::kAttack, CommandSlotKind::kMain));
		m_button_icon_graphic->SetScale(0.18f);
		break;

	case DeviceKind::kPad:
		m_button_icon_graphic = m_button_graphic_resource->GetButtonGraphicer(command->GetInputCode(device, CommandKind::kAttack, CommandSlotKind::kStatic1));
		m_button_icon_graphic->SetScale(0.13f);
		break;
	}

	m_button_icon_graphic->SetCenterPos(kScreenCenterPos + kGraphicOffset);

	m_stealth_kill_icon_screen->UseScreen();
	m_button_icon_graphic->Draw();
	m_knife_graphic		 ->Draw();
	m_stealth_kill_icon_screen->UnuseScreen();
}
