#include "melee_target_icon.hpp"

#include "../Base/character_base.hpp"
#include "../Command/command_handler.hpp"
#include "../Font/text.hpp"

MeleeTargetIcon::MeleeTargetIcon(std::shared_ptr<IMeleeHittable>& melee_target, std::shared_ptr<IMeleeHittable>& visible_downed_character) :
	m_melee_target						(melee_target),
	m_visible_downed_character			(visible_downed_character),
	m_button_graphic_resource			(std::make_shared<ButtonGraphicGetter>()),
	m_button_icon_graphic				(nullptr),
	m_melee_cursor_graphic				(nullptr),
	m_down_cursor_graphic				(nullptr),
	m_explanatory_text_box_blur_graphic	(nullptr),
	m_melee_icon_screen					(nullptr),
	m_mask_graphic						(nullptr),
	m_mask_screen						(std::make_shared<ScreenCreator>(Window::kScreenSize)),
	m_explanatory_text_screen			(nullptr),
	m_mask_creator						(std::make_shared<MaskCreator>()),
	m_icon_pos							(v3d::GetZeroV()),
	m_icon_size							(0.0f)
{
	nlohmann::json j_data;
	if (json_loader::Load("Data/JSON/melee_target_icon_data.json", j_data))
	{
		data = j_data.at("melee_target_icon_data").get<MeleeTargetIconData>();
	}

	// テキスト構築
	text::CreateText(data.text_data);

	m_melee_cursor_graphic				= std::make_shared<Graphicer>(data.melee_cursor_graphic_path);
	m_down_cursor_graphic				= std::make_shared<Graphicer>(data.down_cursor_graphic_path);
	m_explanatory_text_box_blur_graphic = std::make_shared<Graphicer>(data.explanatory_text_box_blur_graphic_path);
	m_melee_icon_screen					= std::make_shared<ScreenCreator>(data.screen_size);
	m_mask_graphic						= std::make_shared<Graphicer>(data.mask_graphic_path);
	m_explanatory_text_screen			= std::make_shared<ScreenCreator>(data.screen_size);

	m_screen_center_pos = data.screen_size * 0.5f;

	m_melee_cursor_graphic->SetCenterPos(m_screen_center_pos + data.cursor_offset);
	m_melee_cursor_graphic->SetScale(data.melee_cursor_graphic_scale);

	m_down_cursor_graphic->SetCenterPos(m_screen_center_pos);
	m_down_cursor_graphic->SetScale(data.down_cursor_graphic_scale);

	m_mask_graphic->SetScale(data.mask_graphic_scale);
	m_explanatory_text_box_blur_graphic->SetScale(data.explanatory_text_box_blur_graphic_scale);
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
			static_cast<int>(m_explanatory_text_box_blur_graphic->GetCenterPos().x - data.text_data.size.x * 0.5f) - 30,
			static_cast<int>(m_explanatory_text_box_blur_graphic->GetCenterPos().y - data.text_data.size.y * 0.5f),
			data.text_data.text.c_str(), 0xffffff, data.text_data.font_handle);

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
	m_icon_size = data.icon_size * distance * data.icon_size_ratio;

	// 3D座標をスクリーン上に変換し適用
	const auto screen_pos_v3d = ConvWorldPosToScreenPos(m_icon_pos);
	const auto screen_pos_v2d = Vector2D<int>(screen_pos_v3d.x, screen_pos_v3d.y);
	m_melee_icon_screen->GetGraphicer()->SetCenterPos(screen_pos_v2d);

	m_mask_graphic->SetCenterPos(screen_pos_v2d + data.mask_graphic_offset);
	m_explanatory_text_box_blur_graphic->SetCenterPos(screen_pos_v2d + data.explanatory_text_box_blur_graphic_offset);
}

void MeleeTargetIcon::CreateMeleeIconScreen()
{
	if (!m_melee_target) { return; }

	const auto	model_handle = std::dynamic_pointer_cast<const CharacterBase>(m_melee_target)->GetModeler()->GetModelHandle();
	auto		head_m		 = MV1GetFrameLocalWorldMatrix(model_handle, MV1SearchFrame(model_handle, FramePath.HEAD));

	m_icon_pos = matrix::GetPos(head_m) + data.icon_offset;

	// 入力デバイスおよびキー割り当てに対応した画像を取得
	// TODO : リファクタリング可能 (優先度 : 低)
	const auto command	= CommandHandler::GetInstance();
	const auto device	= InputChecker::GetInstance()->GetCurrentInputDevice();
	switch (device)
	{
	case DeviceKind::kKeyboard:
		m_button_icon_graphic = m_button_graphic_resource->GetButtonGraphicer(command->GetInputCode(device, CommandKind::kMelee, CommandSlotKind::kMain));
		break;

	case DeviceKind::kPad:
		m_button_icon_graphic = m_button_graphic_resource->GetButtonGraphicer(command->GetInputCode(device, CommandKind::kMelee, CommandSlotKind::kStatic1));
		break;
	}

	m_button_icon_graphic->SetCenterPos(m_screen_center_pos);
	m_button_icon_graphic->SetScale(data.button_icon_graphic_scale);

	m_melee_icon_screen->UseScreen();
	m_button_icon_graphic ->Draw();
	m_melee_cursor_graphic->Draw();
	m_melee_icon_screen->UnuseScreen();
}

void MeleeTargetIcon::CreateDownIconScreen()
{
	if (!m_visible_downed_character) { return; }

	const auto	model_handle = std::dynamic_pointer_cast<const CharacterBase>(m_visible_downed_character)->GetModeler()->GetModelHandle();
	auto		head_m = MV1GetFrameLocalWorldMatrix(model_handle, MV1SearchFrame(model_handle, FramePath.HEAD));

	m_icon_pos = matrix::GetPos(head_m) + data.icon_offset;

	m_melee_icon_screen->UseScreen();
	m_down_cursor_graphic->Draw();
	m_melee_icon_screen->UnuseScreen();
}
