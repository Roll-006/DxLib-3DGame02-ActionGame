#include "weapon_shortcut_drawer.hpp"

WeaponShortcutDrawer::WeaponShortcutDrawer(
	std::unordered_map<TimeKind, std::shared_ptr<WeaponActionStateBase<Player>>>& state,
	const std::shared_ptr<WeaponShortcutSelecter> weapon_shortcut_selecter) :
	m_state						(state),
	m_weapon_shortcut_selecter	(weapon_shortcut_selecter),
	m_screen_creator			(std::make_shared<ScreenCreator>(kScreenSize)),
	m_alpha_blend_num			(0),
	m_end_draw_time				(kDrawEndTime),
	m_end_draw_timer			(0.0f),
	m_enter_scale_timer			(0.0f),
	m_exit_scale_timer			(0.0f),
	m_is_selected				(false)
{
	CreateShortcutIcon();

	m_screen_creator->GetGraphicer()->SetCenterPos(Vector2D<int>(static_cast<int>(Window::kScreenSize.x * 0.72f), Window::kScreenHalfSize.y));

	m_weapon_graphic_pair[ObjName.ASSAULT_RIFLE]	= std::make_shared<Graphicer>(UIGraphicPath.ASSAULT_RIFLE);
	m_weapon_graphic_pair[ObjName.ASSAULT_RIFLE]->SetScale(0.07f);

	m_weapon_graphic_pair[ObjName.ROCKET_LAUNCHER]	= std::make_shared<Graphicer>(UIGraphicPath.ROCKET_LAUNCHER);
	m_weapon_graphic_pair[ObjName.ROCKET_LAUNCHER]->SetScale(0.05f);
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

		icon.second->AttachGraphic(m_weapon_graphic_pair[weapon->GetName()]);
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
}

void WeaponShortcutDrawer::Draw() const
{
	if (m_alpha_blend_num <= 0) { return; }

	const WeaponShortcutPosKind current_select_shortcut	= m_weapon_shortcut_selecter->GetCurrentSelectShortcut();
	const Vector2D<int>			current_center_pos		= m_center_pos.at(current_select_shortcut);

	m_screen_creator->UseScreen();

	for (const auto& icon : m_weapon_shortcut_icons)
	{
		icon.second->Draw();
	}

	DrawBox(
		static_cast<int>(current_center_pos.x - kIconWidth  * 0.5f),
		static_cast<int>(current_center_pos.y - kIconHeight * 0.5f),
		static_cast<int>(current_center_pos.x + kIconWidth  * 0.5f),
		static_cast<int>(current_center_pos.y + kIconHeight * 0.5f),
		0xffffff, FALSE);

	m_screen_creator->UnuseScreen();
	m_screen_creator->Draw();
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

void WeaponShortcutDrawer::UpdateAnim()
{
	const auto delta_time	= GameTimeManager::GetInstance()->GetDeltaTime(TimeScaleController::LayerKind::kUI);
	const auto graphicer	= m_screen_creator->GetGraphicer();
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

	graphicer->SetAlphaBlendNum(m_alpha_blend_num);
	graphicer->SetScale(m_scale);
}
