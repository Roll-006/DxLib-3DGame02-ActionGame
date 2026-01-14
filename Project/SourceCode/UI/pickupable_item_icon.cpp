#include "pickupable_item_icon.hpp"

#include "../Accessor/obj_accessor.hpp"
#include "../Command/command_handler.hpp"
#include "../Object/assault_rifle_ammo_box.hpp"
#include "../Object/rocket_bomb_box.hpp"
#include "../Font/text.hpp"

PickupableItemIcon::PickupableItemIcon(std::shared_ptr<IItem>& pickupable_item, const std::vector<SpottedObjData>& pick_up_candidate_items) :
	data								(PickupableItemIconData()),
	m_pickupable_item					(pickupable_item),
	m_pick_up_candidate_items			(pick_up_candidate_items),
	m_button_graphic_resource			(std::make_shared<ButtonGraphicGetter>()),
	m_button_icon_graphic				(nullptr),
	m_pickupable_cursor_graphic			(nullptr),
	m_candidate_cursor_graphic			(nullptr),
	m_explanatory_text_box_blur_graphic	(nullptr),
	m_pickupable_icon_screen			(nullptr),
	m_mask_graphic						(nullptr),
	m_mask_screen						(std::make_shared<ScreenCreator>(Window::kScreenSize)),
	m_explanatory_text_screen			(nullptr),
	m_mask_creator						(std::make_shared<MaskCreator>()),
	m_icon_pos							(v3d::GetZeroV()),
	m_icon_size							(0.0f)
{
	nlohmann::json j_data;
	if (json_loader::Load("Data/JSON/pickupable_item_icon_data.json", j_data))
	{
		data = j_data.at("pickupable_item_icon_data").get<PickupableItemIconData>();
	}

	// テキスト構築
	text::CreateText(data.text_data);

	m_pickupable_cursor_graphic			= std::make_shared<Graphicer>(data.pickupable_cursor_graphic_path);
	m_candidate_cursor_graphic			= std::make_shared<Graphicer>(data.candidate_cursor_graphic_path);
	m_explanatory_text_box_blur_graphic = std::make_shared<Graphicer>(data.explanatory_text_box_blur_graphic_path);
	m_pickupable_icon_screen			= std::make_shared<ScreenCreator>(data.screen_size);
	m_mask_graphic						= std::make_shared<Graphicer>(data.mask_graphic_path);
	m_explanatory_text_screen			= std::make_shared<ScreenCreator>(data.screen_size);

	m_screen_center_pos = data.screen_size * 0.5f;

	m_pickupable_cursor_graphic->SetCenterPos(m_screen_center_pos + data.cursor_offset);
	m_pickupable_cursor_graphic->SetScale(data.pickupable_cursor_graphic_scale);

	m_candidate_cursor_graphic->SetCenterPos(m_screen_center_pos);
	m_candidate_cursor_graphic->SetScale(data.candidate_cursor_graphic_scale);

	m_mask_graphic						->SetScale(data.mask_graphic_scale);
	m_explanatory_text_box_blur_graphic	->SetScale(data.explanatory_text_box_blur_graphic_scale);
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
	const auto obj_accessor = ObjAccessor::GetInstance();
	auto obj_handle = -1;

	if (m_pickupable_item)
	{
		const auto obj = std::dynamic_pointer_cast<const PhysicalObjBase>(m_pickupable_item);
		if (obj)
		{
			obj_handle = obj->GetObjHandle();
		}

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
			static_cast<int>(m_explanatory_text_box_blur_graphic->GetCenterPos().x - (m_explanatory_text_box_blur_graphic->GetOriginSize().x * m_explanatory_text_box_blur_graphic->GetScale().x) * 0.5f) + data.offset,
			static_cast<int>(m_explanatory_text_box_blur_graphic->GetCenterPos().y - data.text_data.size.y * 0.5f),
			data.text_data.text.c_str(), data.text_data.u_int_color, data.text_data.font_handle);

		m_pickupable_icon_screen->Draw();
	}

	// 取得候補者
	for (const auto& candidate : m_pick_up_candidate_items)
	{
		if (candidate.target_obj_handle == obj_handle) { continue; }

		const auto obj = obj_accessor->GetObj<ObjBase>(candidate.target_obj_handle);
		if (!obj) { continue; }

		const auto screen_pos_v3d = ConvWorldPosToScreenPos(obj->GetTransform()->GetPos(CoordinateKind::kWorld));
		const auto screen_pos_v2d = Vector2D<int>(screen_pos_v3d.x, screen_pos_v3d.y);
		m_candidate_cursor_graphic->SetCenterPos(screen_pos_v2d);
		m_candidate_cursor_graphic->Draw();
	}
}

void PickupableItemIcon::CalcResultScreenCenterPos()
{
	const auto camera_pos		= GetCameraPosition();
	const auto distance			= VSize(m_icon_pos - camera_pos);

	// DrawBillboard3D関数は距離に応じて描画サイズが変更されるため
	// 距離に応じて拡大する
	m_icon_size					= data.icon_size * distance * data.icon_size_ratio;

	// 3D座標をスクリーン上に変換し適用
	const auto screen_pos_v3d	= ConvWorldPosToScreenPos(m_icon_pos);
	const auto screen_pos_v2d	= Vector2D<int>(screen_pos_v3d.x, screen_pos_v3d.y);
	m_pickupable_icon_screen->GetGraphicer()->SetCenterPos(screen_pos_v2d);

	m_mask_graphic->SetCenterPos(screen_pos_v2d + data.mask_graphic_offset);
	m_explanatory_text_box_blur_graphic->SetCenterPos(screen_pos_v2d + data.explanatory_text_box_blur_graphic_offset);
}

void PickupableItemIcon::CreatePickupableItemIconScreen()
{
	if (!m_pickupable_item) { return; }

	const auto obj = std::dynamic_pointer_cast<const ObjBase>(m_pickupable_item);
	if (!obj) { return; }

	m_icon_pos = obj->GetTransform()->GetPos(CoordinateKind::kWorld) + data.icon_offset;

	// 入力デバイスおよびキー割り当てに対応した画像を取得
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

	m_pickupable_icon_screen	->UseScreen();
	m_button_icon_graphic		->Draw();
	m_pickupable_cursor_graphic	->Draw();
	m_pickupable_icon_screen	->UnuseScreen();
}

void PickupableItemIcon::CreateText()
{
	if (!m_pickupable_item) { return; }
	
	const auto item_kind = m_pickupable_item->GetItemKind();

	data.text_data.text = "";

	// 弾
	if (item_kind == ItemKind::kAmmoBox)
	{
		if (std::dynamic_pointer_cast<const AssaultRifleAmmoBox>(m_pickupable_item))
		{
			data.text_data.text = data.assault_rifle_ammo_box_name;
		}
		else if (std::dynamic_pointer_cast<const RocketBombBox>(m_pickupable_item))
		{
			data.text_data.text = data.rocket_bomb_box_name;
		}

		const auto num = std::to_string(std::dynamic_pointer_cast<const IAmmoBox>(m_pickupable_item)->GetCurrentHaveNum());
		data.text_data.text += num;
	}

	text::CreateText(data.text_data);
}
