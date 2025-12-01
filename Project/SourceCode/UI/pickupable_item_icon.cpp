#include "pickupable_item_icon.hpp"

#include "../Manager/obj_manager.hpp"
#include "../Command/command_handler.hpp"
#include "../Object/assault_rifle_ammo_box.hpp"
#include "../Object/rocket_bomb_box.hpp"

PickupableItemIcon::PickupableItemIcon(std::shared_ptr<IItem>& pickupable_item, std::vector<SpottedObjData>& pick_up_candidate_items) :
	m_pickupable_item					(pickupable_item),
	m_pick_up_candidate_items			(pick_up_candidate_items),
	m_button_graphic_resource			(std::make_shared<ButtonGraphicGetter>()),
	m_button_icon_graphic				(nullptr),
	m_pickupable_cursor_graphic			(std::make_shared<Graphicer>(UIGraphicPath.CURSOR_01)),
	m_candidate_cursor_graphic			(std::make_shared<Graphicer>(UIGraphicPath.CURSOR_04)),
	m_explanatory_text_box_blur_graphic	(std::make_shared<Graphicer>(UIGraphicPath.MELEE_EXPLANATORY_TEXT_BOX_BLUR)),
	m_pickupable_icon_screen			(std::make_shared<ScreenCreator>(kScreenSize)),
	m_mask_graphic						(std::make_shared<Graphicer>(UIGraphicPath.MELEE_EXPLANATORY_TEXT_BOX)),
	m_mask_screen						(std::make_shared<ScreenCreator>(Window::kScreenSize)),
	m_explanatory_text_screen			(std::make_shared<ScreenCreator>(kScreenSize)),
	m_mask_creator						(std::make_shared<MaskCreator>()),
	m_icon_pos							(v3d::GetZeroV()),
	m_icon_size							(0.0f),
	m_font_handle						(FontHandler::GetInstance()->GetFontHandle(FontName.EXPLANATORY_TEXT)),
	m_text								(""),
	m_font_size							(v2d::GetZeroV<Vector2D<int>>())
{
	m_pickupable_cursor_graphic->SetCenterPos(kScreenCenterPos + kCursorOffset);
	m_pickupable_cursor_graphic->SetScale(0.2f);

	m_candidate_cursor_graphic->SetCenterPos(kScreenCenterPos);
	m_candidate_cursor_graphic->SetScale(0.4f);

	m_mask_graphic						->SetScale(Vector2D<float>(1.1f, 0.7f));
	m_explanatory_text_box_blur_graphic	->SetScale(Vector2D<float>(1.1f, 0.7f));
}

PickupableItemIcon::~PickupableItemIcon()
{

}

void PickupableItemIcon::LateUpdate()
{
	CreatePickupableItemIconScreen();
	CalcResultScreenCenterPos();
	CreateText();
}

void PickupableItemIcon::Draw(const int main_screen_handle) const
{
	const auto obj_manager = ObjManager::GetInstance();
	auto obj_handle = -1;

	if (m_pickupable_item)
	{
		obj_handle = std::dynamic_pointer_cast<ObjBase>(m_pickupable_item)->GetObjHandle();

		m_mask_screen	->UseScreen();
		m_mask_graphic	->Draw();
		m_mask_screen	->UnuseScreen();

		m_mask_creator->CreateMask();
		m_mask_creator->UseMask(m_mask_screen->GetScreenHandle(), true);
		DrawGraph(0, 0, main_screen_handle, TRUE);
		m_explanatory_text_box_blur_graphic->Draw();
		m_mask_creator->UnuseMask();
		m_mask_creator->DeleteMask();

		DrawStringToHandle(
			static_cast<int>(m_explanatory_text_box_blur_graphic->GetCenterPos().x - m_font_size.x * 0.5f),
			static_cast<int>(m_explanatory_text_box_blur_graphic->GetCenterPos().y - m_font_size.y * 0.5f),
			m_text.c_str(), 0xffffff, m_font_handle);

		m_pickupable_icon_screen->Draw();
	}

	// 取得候補者
	for (const auto& candidate : m_pick_up_candidate_items)
	{
		if (candidate.target_obj_handle == obj_handle) { continue; }

		const auto obj = obj_manager->GetObj<ObjBase>(candidate.target_obj_handle);
		if (!obj) { continue; }

		const auto screen_pos_v3d = ConvWorldPosToScreenPos(obj->GetTransform()->GetPos(CoordinateKind::kWorld));
		const auto screen_pos_v2d = Vector2D<int>(screen_pos_v3d.x, screen_pos_v3d.y);
		m_candidate_cursor_graphic->SetCenterPos(screen_pos_v2d);
		m_candidate_cursor_graphic->Draw();
	}
}

void PickupableItemIcon::CalcResultScreenCenterPos()
{
	const auto camera_pos	= GetCameraPosition();
	const auto distance		= VSize(m_icon_pos - camera_pos);

	// DrawBillboard3D関数は距離に応じて描画サイズが変更されるため
	// 距離に応じて拡大する
	m_icon_size = kIconSize * distance * 0.01f;

	// 3D座標をスクリーン上に変換し適用
	const auto screen_pos_v3d = ConvWorldPosToScreenPos(m_icon_pos);
	const auto screen_pos_v2d = Vector2D<int>(screen_pos_v3d.x, screen_pos_v3d.y);
	m_pickupable_icon_screen->GetGraphicer()->SetCenterPos(screen_pos_v2d);

	m_mask_graphic->SetCenterPos(screen_pos_v2d + Vector2D<int>(180, 0));
	m_explanatory_text_box_blur_graphic->SetCenterPos(screen_pos_v2d + Vector2D<int>(180, 0));
}

void PickupableItemIcon::CreatePickupableItemIconScreen()
{
	if (!m_pickupable_item) { return; }

	const auto obj = std::dynamic_pointer_cast<ObjBase>(m_pickupable_item);
	if (!obj) { return; }

	m_icon_pos = obj->GetTransform()->GetPos(CoordinateKind::kWorld) + kIconOffset;

	// 入力デバイスおよびキー割り当てに対応した画像を取得
	const auto command = CommandHandler::GetInstance();
	switch (InputChecker::GetInstance()->GetCurrentInputDevice())
	{
	case DeviceKind::kKeyboard:
		m_button_icon_graphic = m_button_graphic_resource->GetWeaponGraphicer(command->GetKeyInputCode(CommandKind::kMelee, CommandSlotKind::kMain));
		m_button_icon_graphic->SetCenterPos(kScreenCenterPos);
		m_button_icon_graphic->SetScale(0.175f);
		break;

	case DeviceKind::kPad:
		m_button_icon_graphic = m_button_graphic_resource->GetWeaponGraphicer(command->GetPadInputCode(CommandKind::kMelee, CommandSlotKind::kStatic1));
		m_button_icon_graphic->SetCenterPos(kScreenCenterPos);
		m_button_icon_graphic->SetScale(0.175f);
		break;
	}

	m_pickupable_icon_screen	->UseScreen();
	m_button_icon_graphic		->Draw();
	m_pickupable_cursor_graphic	->Draw();
	m_pickupable_icon_screen	->UnuseScreen();
}

void PickupableItemIcon::CreateText()
{
	if (!m_pickupable_item) { return; }
	
	m_text = "";

	const auto item_kind = m_pickupable_item->GetItemKind();

	// 弾
	if (item_kind == ItemKind::kAmmoBox)
	{
		if (std::dynamic_pointer_cast<AssaultRifleAmmoBox>(m_pickupable_item))
		{
			m_text = "アサルトライフルの弾 ×";
		}
		else if (std::dynamic_pointer_cast<RocketBombBox>(m_pickupable_item))
		{
			m_text = "ロケット弾 ×";
		}

		const auto num = std::to_string(std::dynamic_pointer_cast<IAmmoBox>(m_pickupable_item)->GetCurrentHaveNum());
		m_text += num;
	}

	m_font_size = Vector2D<int>(GetDrawStringWidthToHandle(m_text.c_str(), -1, m_font_handle), GetFontSizeToHandle(m_font_handle));
}
