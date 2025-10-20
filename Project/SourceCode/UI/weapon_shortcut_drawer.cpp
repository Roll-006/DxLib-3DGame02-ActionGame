#include "weapon_shortcut_drawer.hpp"

WeaponShortcutDrawer::WeaponShortcutDrawer(
	std::unordered_map<TimeKind, std::shared_ptr<WeaponActionStateBase<Player>>>& state,
	const std::shared_ptr<WeaponShortcutSelecter>& weapon_shortcut_selecter) :
	m_state						(state),
	m_weapon_shortcut_selecter	(weapon_shortcut_selecter),
	m_weapon_graphic			(std::make_shared<WeaponGraphicGetter>()),
	m_icons_screen				(std::make_shared<ScreenCreator>(kScreenSize, Vector2D<int>(static_cast<int>(Window::kScreenSize.x * 0.72f), Window::kScreenHalfSize.y))),
	m_mask_resource_icons_screen(std::make_shared<ScreenCreator>(kScreenSize, Vector2D<int>(static_cast<int>(Window::kScreenSize.x * 0.72f), Window::kScreenHalfSize.y))),
	m_mask_screen				(std::make_shared<ScreenCreator>(Window::kScreenSize, Window::kCenterPos)),
	m_mask_creator				(std::make_shared<MaskCreator>()),
	m_alpha_blend_num			(0),
	m_end_draw_time				(kDrawEndTime),
	m_end_draw_timer			(0.0f),
	m_enter_scale_timer			(0.0f),
	m_exit_scale_timer			(0.0f),
	m_is_selected				(false)
{
	CreateShortcutIcon();
}

WeaponShortcutDrawer::~WeaponShortcutDrawer()
{

}

void WeaponShortcutDrawer::LateUpdate()
{
	// 描画する画像をアタッチ
	for (const auto& icon : m_weapon_shortcut_icons)
	{
		const auto weapon = m_weapon_shortcut_selecter->GetShortcutWeapon(icon.first);
		if (!weapon) { continue; }

		icon.second->AttachGraphic(m_weapon_graphic->GetWeaponGraphicer(weapon->GetName()));
	}

	const auto state = m_state.at(TimeKind::kCurrent);
	if (state->GetStateKind() == static_cast<int>(player_state::WeaponActionStateKind::kAimGun))
	{
		m_end_draw_time = 0.5f;
	}
	else
	{
		m_end_draw_time = kDrawEndTime;
	}

	UpdateAnim();
	CreateIconsCreen();
}

void WeaponShortcutDrawer::Draw(const int main_screen_handle) const
{
	if (m_alpha_blend_num <= 0) { return; }

	m_mask_screen ->UseScreen();
	m_mask_resource_icons_screen->Draw();
	m_mask_screen ->UnuseScreen();

	m_mask_creator->CreateMask();
	m_mask_creator->UseMask(m_mask_screen->GetScreenHandle(), true);
	DrawGraph(0, 0, main_screen_handle, TRUE);
	m_mask_creator->UnuseMask();
	m_mask_creator->DeleteMask();

	m_icons_screen->Draw();
}

void WeaponShortcutDrawer::CreateShortcutIcon()
{
	// WARNING : WeaponShortcutPosKindの定義順に依存している
	for (int i = 0; i < 8; ++i)
	{
		Vector2D<int> center_pos = kCenterPos;
		const int first_offset = (0b00110011 >> i) & 1 ? -kFirstIntervalPos : kFirstIntervalPos;

		if (i % 2 == 0)
		{
			const int offset_y_size = i < 4 ? static_cast<int>(kIconHeight * 0.5f) : static_cast<int>(kIconHeight * 1.5f) + kIntervalPos;
			const int offset_y		= (0b00110011 >> i) & 1 ? -offset_y_size : offset_y_size;
			center_pos.y += first_offset + offset_y;
		}
		else
		{
			const int offset_x_size = i < 4 ? static_cast<int>(kIconWidth * 0.5f) : static_cast<int>(kIconWidth * 1.5f) + kIntervalPos;
			const int offset_x		= (0b00110011 >> i) & 1 ? -offset_x_size : offset_x_size;
			center_pos.x += first_offset + offset_x;
		}

		m_center_pos			[static_cast<WeaponShortcutPosKind>(i)] = center_pos;
		m_weapon_shortcut_icons	[static_cast<WeaponShortcutPosKind>(i)] = std::make_shared<WeaponShortcutIcon>(center_pos, kIconWidth, kIconHeight);
	}
}

void WeaponShortcutDrawer::CreateIconsCreen()
{
	const WeaponShortcutPosKind current_select_shortcut = m_weapon_shortcut_selecter->GetCurrentSelectShortcut();
	const Vector2D<int>			current_center_pos = m_center_pos.at(current_select_shortcut);

	m_icons_screen->UseScreen();
	for (const auto& icon : m_weapon_shortcut_icons) { icon.second->Draw(); }
	DrawBox(
		static_cast<int>(current_center_pos.x - kIconWidth  * 0.5f),
		static_cast<int>(current_center_pos.y - kIconHeight * 0.5f),
		static_cast<int>(current_center_pos.x + kIconWidth  * 0.5f),
		static_cast<int>(current_center_pos.y + kIconHeight * 0.5f),
		0xffffff, FALSE);
	m_icons_screen->UnuseScreen();

	m_mask_resource_icons_screen->UseScreen();
	for (const auto& icon : m_weapon_shortcut_icons) { icon.second->DrawToMaskResource(); }
	m_mask_resource_icons_screen->UnuseScreen();
}

void WeaponShortcutDrawer::UpdateAnim()
{
	const auto delta_time = GameTimeManager::GetInstance()->GetDeltaTime(TimeScaleLayerKind::kUI);
	float t = 0.0f;

	if (m_weapon_shortcut_selecter->isSelecting())
	{
		m_end_draw_timer = 0.0f;

		if (!m_is_selected)
		{
			m_is_selected		= true;
			m_enter_scale_timer = 0.0f;
			m_exit_scale_timer	= kExitAnimTime;
		}
	}
	else
	{
		m_end_draw_timer += delta_time;
		if (m_end_draw_timer > m_end_draw_time)
		{
			m_is_selected = false;
		}
	}

	// 係数を取得
	if (m_is_selected)
	{
		math::Increase(m_enter_scale_timer, delta_time, kEnterAnimTime, false);
		t = math::GetUnitValue<float, float>(0.0f, kEnterAnimTime, m_enter_scale_timer);
	}
	else
	{
		math::Decrease(m_exit_scale_timer, delta_time, 0.0f);
		t = math::GetUnitValue<float, float>(0.0f, kExitAnimTime, m_exit_scale_timer);
	}

	// 線形補間
	m_alpha_blend_num	= math::GetLerp(0, 255, t);
	m_scale				= math::GetLerp(0.5f, 1.0f, t);

	m_icons_screen->GetGraphicer()->SetAlphaBlendNum(m_alpha_blend_num);
	m_icons_screen->GetGraphicer()->SetScale(m_scale);
	m_mask_resource_icons_screen->GetGraphicer()->SetAlphaBlendNum(m_alpha_blend_num);
	m_mask_resource_icons_screen->GetGraphicer()->SetScale(m_scale);
}
