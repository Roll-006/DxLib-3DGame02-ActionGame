#include "melee_target_icon.hpp"

#include "../Base/character_base.hpp"
#include "../Command/command_handler.hpp"

MeleeTargetIcon::MeleeTargetIcon(std::shared_ptr<IMeleeHittable>& melee_target, std::shared_ptr<IMeleeHittable>& visible_downed_character) :
	m_melee_target						(melee_target),
	m_visible_downed_character			(visible_downed_character),
	m_button_graphic_resource			(std::make_shared<ButtonGraphicGetter>()),
	m_button_icon_graphic				(nullptr),
	m_melee_cursor_graphic				(std::make_shared<Graphicer>(UIGraphicPath.CURSOR_01)),
	m_down_cursor_graphic				(std::make_shared<Graphicer>(UIGraphicPath.CURSOR_02)),
	m_explanatory_text_box_blur_graphic	(std::make_shared<Graphicer>(UIGraphicPath.MELEE_EXPLANATORY_TEXT_BOX_BLUR)),
	m_melee_icon_screen					(std::make_shared<ScreenCreator>(kScreenSize)),
	m_mask_graphic						(std::make_shared<Graphicer>(UIGraphicPath.MELEE_EXPLANATORY_TEXT_BOX)),
	m_mask_screen						(std::make_shared<ScreenCreator>(Window::kScreenSize)),
	m_explanatory_text_screen			(std::make_shared<ScreenCreator>(kScreenSize)),
	m_mask_creator						(std::make_shared<MaskCreator>()),
	m_icon_pos							(v3d::GetZeroV()),
	m_icon_size							(0.0f),
	m_font_handle						(FontHandler::GetInstance()->GetFontHandle(FontName.EXPLANATORY_TEXT)),
	m_text								("メレー"),
	m_font_size							(Vector2D<int>(GetDrawStringWidthToHandle(m_text.c_str(), -1, m_font_handle), GetFontSizeToHandle(m_font_handle)))
{
	m_melee_cursor_graphic->SetCenterPos(kScreenCenterPos + kCursorOffset);
	m_melee_cursor_graphic->SetScale(0.2f);

	m_down_cursor_graphic->SetCenterPos(kScreenCenterPos);
	m_down_cursor_graphic->SetScale(0.2f);

	m_mask_graphic->SetScale(0.7f);
	m_explanatory_text_box_blur_graphic->SetScale(0.7f);
}

MeleeTargetIcon::~MeleeTargetIcon()
{

}

void MeleeTargetIcon::LateUpdate()
{
	CreateMeleeIconScreen();
	CreateDownIconScreen();
	CalcResultScreenCenterPos();
}

void MeleeTargetIcon::Draw(const int main_screen_handle) const
{
	if (m_melee_target)
	{
		m_mask_screen->UseScreen();
		m_mask_graphic->Draw();
		m_mask_screen->UnuseScreen();

		m_mask_creator->CreateMask();
		m_mask_creator->UseMask(m_mask_screen->GetScreenHandle(), true);
		DrawGraph(0, 0, main_screen_handle, TRUE);
		m_explanatory_text_box_blur_graphic->Draw();
		m_mask_creator->UnuseMask();
		m_mask_creator->DeleteMask();

		DrawStringToHandle(
			static_cast<int>(m_explanatory_text_box_blur_graphic->GetCenterPos().x - m_font_size.x * 0.5f) - 30,
			static_cast<int>(m_explanatory_text_box_blur_graphic->GetCenterPos().y - m_font_size.y * 0.5f),
			m_text.c_str(), 0xffffff, m_font_handle);

		m_melee_icon_screen->Draw();
	}
	else if (m_visible_downed_character)
	{
		m_melee_icon_screen->Draw();
	}
}

void MeleeTargetIcon::CalcResultScreenCenterPos()
{
	const auto camera_pos	= GetCameraPosition();
	const auto distance		= VSize(m_icon_pos - camera_pos);

	// DrawBillboard3D関数は距離に応じて描画サイズが変更されるため
	// 距離に応じて拡大する
	m_icon_size = kIconSize * distance * 0.01f;

	// 3D座標をスクリーン上に変換し適用
	const auto screen_pos_v3d = ConvWorldPosToScreenPos(m_icon_pos);
	const auto screen_pos_v2d = Vector2D<int>(screen_pos_v3d.x, screen_pos_v3d.y);
	m_melee_icon_screen->GetGraphicer()->SetCenterPos(screen_pos_v2d);

	m_mask_graphic->SetCenterPos(screen_pos_v2d + Vector2D<int>(110, 0));
	m_explanatory_text_box_blur_graphic->SetCenterPos(screen_pos_v2d + Vector2D<int>(110, 0));
}

void MeleeTargetIcon::CreateMeleeIconScreen()
{
	if (!m_melee_target) { return; }

	const auto	model_handle = std::dynamic_pointer_cast<CharacterBase>(m_melee_target)->GetModeler()->GetModelHandle();
	auto		head_m		 = MV1GetFrameLocalWorldMatrix(model_handle, MV1SearchFrame(model_handle, FramePath.HEAD));

	m_icon_pos = MGetTranslateElem(head_m) + kIconOffset;

	// 入力デバイスおよびキー割り当てに対応した画像を取得
	// TODO : リファクタリング可能 (優先度 : 低)
	const auto command	= CommandHandler::GetInstance();
	const auto device	= InputChecker::GetInstance()->GetCurrentInputDevice();
	switch (device)
	{
	case DeviceKind::kKeyboard:
		m_button_icon_graphic = m_button_graphic_resource->GetButtonGraphicer(command->GetInputCode(device, CommandKind::kMelee, CommandSlotKind::kMain));
		m_button_icon_graphic->SetCenterPos(kScreenCenterPos);
		m_button_icon_graphic->SetScale(0.2f);
		break;

	case DeviceKind::kPad:
		m_button_icon_graphic = m_button_graphic_resource->GetButtonGraphicer(command->GetInputCode(device, CommandKind::kMelee, CommandSlotKind::kStatic1));
		m_button_icon_graphic->SetCenterPos(kScreenCenterPos);
		m_button_icon_graphic->SetScale(0.2f);
		break;
	}

	m_melee_icon_screen->UseScreen();
	m_button_icon_graphic ->Draw();
	m_melee_cursor_graphic->Draw();
	m_melee_icon_screen->UnuseScreen();
}

void MeleeTargetIcon::CreateDownIconScreen()
{
	if (!m_visible_downed_character) { return; }

	const auto	model_handle = std::dynamic_pointer_cast<CharacterBase>(m_visible_downed_character)->GetModeler()->GetModelHandle();
	auto		head_m = MV1GetFrameLocalWorldMatrix(model_handle, MV1SearchFrame(model_handle, FramePath.HEAD));

	m_icon_pos = MGetTranslateElem(head_m) + kIconOffset;

	m_melee_icon_screen->UseScreen();
	m_down_cursor_graphic->Draw();
	m_melee_icon_screen->UnuseScreen();
}
