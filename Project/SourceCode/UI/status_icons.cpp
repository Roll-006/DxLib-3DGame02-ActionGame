#include "status_icons.hpp"

StatusIcons::StatusIcons(std::shared_ptr<Player>& player) : 
	m_result_screen					(std::make_shared<ScreenCreator>(kScreenSize)),
	m_health_gauge					(std::make_shared<HealthGauge>(player->GetHealth(HealthPartKind::kMain))),
	m_equip_weapon_icon				(std::make_shared<EquipWeaponIcon>(player->GetCurrentEquipWeapons())),
	m_remaining_bullets_ui			(std::make_shared<RemainingBulletsUI>(player)),
	m_health_gauge_graphic			(std::make_shared<Graphicer>(m_health_gauge->GetScreenHandle())),
	m_equip_weapon_icon_graphic		(std::make_shared<Graphicer>(m_equip_weapon_icon->GetScreenHandle())),
	m_remaining_bullets_ui_graphic	(std::make_shared<Graphicer>(m_remaining_bullets_ui->GetScreenHandle())),
	m_alpha_blend_num				(UCHAR_MAX),
	m_is_active_cutscene			(false)
{
	// イベント登録
	EventSystem::GetInstance()->Subscribe<StartRocketLauncherCutsceneEvent>	(this, &StatusIcons::ActivateCutscene);
	EventSystem::GetInstance()->Subscribe<EndRocketLauncherCutsceneEvent>	(this, &StatusIcons::DeactivateCutscene);

	m_result_screen->GetGraphicer()->SetCenterPos(Vector2D<int>(
		static_cast<int>(Window::kScreenSize.x * 0.891f), 
		static_cast<int>(Window::kScreenSize.y * 0.81f)));

	m_health_gauge_graphic			->SetCenterPos(kCenterPos);
	m_equip_weapon_icon_graphic		->SetCenterPos(kCenterPos);
	m_remaining_bullets_ui_graphic	->SetCenterPos(kCenterPos + Vector2D<int>(15, 40));
}

StatusIcons::~StatusIcons()
{
	// イベントの登録解除
	EventSystem::GetInstance()->Unsubscribe<StartRocketLauncherCutsceneEvent>	(this, &StatusIcons::ActivateCutscene);
	EventSystem::GetInstance()->Unsubscribe<EndRocketLauncherCutsceneEvent>		(this, &StatusIcons::DeactivateCutscene);
}

void StatusIcons::LateUpdate()
{
	m_health_gauge			->LateUpdate();
	m_equip_weapon_icon		->LateUpdate();
	m_remaining_bullets_ui	->LateUpdate();

	CalcResultScreenAlphaBlendNum();
	CreateScreen();
}

void StatusIcons::Draw() const
{
	m_result_screen->Draw();
}


#pragma region Event
void StatusIcons::ActivateCutscene(const StartRocketLauncherCutsceneEvent& event)
{
	m_is_active_cutscene = true;
}

void StatusIcons::DeactivateCutscene(const EndRocketLauncherCutsceneEvent& event)
{
	m_is_active_cutscene = false;
}
#pragma endregion


void StatusIcons::CreateScreen()
{
	m_result_screen->UseScreen();

	m_health_gauge_graphic			->Draw();
	m_equip_weapon_icon_graphic		->Draw();
	m_remaining_bullets_ui_graphic	->Draw();

	m_result_screen->UnuseScreen();
}

void StatusIcons::CalcResultScreenAlphaBlendNum()
{
	// TODO : マジックナンバーの削除
	const auto delta_time = GameTimeManager::GetInstance()->GetDeltaTime(TimeScaleLayerKind::kUI);

	if (m_is_active_cutscene)
	{
		math::Decrease(m_alpha_blend_num, static_cast<int>(700.0f * delta_time), 0);
	}
	else
	{
		math::Increase(m_alpha_blend_num, static_cast<int>(700.0f * delta_time), UCHAR_MAX, false);
	}

	m_result_screen->GetGraphicer()->SetBlendNum(m_alpha_blend_num);
}
