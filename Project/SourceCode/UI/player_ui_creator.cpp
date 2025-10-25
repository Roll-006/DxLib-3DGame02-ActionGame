#include "player_ui_creator.hpp"

PlayerUICreator::PlayerUICreator(std::shared_ptr<Player>& player) : 
	m_name				(UICreatorName.PLAYER_UI_CREATOR),
	m_priority			(10),
	m_is_active			(true),
	m_melee_target_icon	(std::make_shared<MeleeTargetIcon>(player->GetMeleeTarget())),
	m_escape_icon		(std::make_shared<EscapeIcon>(std::dynamic_pointer_cast<IGrabbable>(player))),
	m_status_icons		(std::make_shared<StatusIcons>(player)),
	m_cross_hair		(std::make_shared<CrossHair>(player)),
	m_weapon_shortcut	(std::make_shared<WeaponShortcutDrawer>(
							player->GetStateController()->GetWeaponActionState(),
							player->GetWeaponShortcutSelecter())),
	m_result_screen		(std::make_shared<ScreenCreator>(Window::kScreenSize, Window::kCenterPos)),
	m_alpha_blend_num	(255),
	m_is_active_cutscene(false)
{
	// イベント登録
	EventSystem::GetInstance()->Subscribe<DeadPlayerEvent>					(this, &PlayerUICreator::Deactivate);
	EventSystem::GetInstance()->Subscribe<StartRocketLauncherCutsceneEvent>	(this, &PlayerUICreator::ActivateCutscene);
	EventSystem::GetInstance()->Subscribe<EndRocketLauncherCutsceneEvent>	(this, &PlayerUICreator::DeactivateCutscene);
}

PlayerUICreator::~PlayerUICreator()
{
	// イベントの登録解除
	EventSystem::GetInstance()->Unsubscribe<DeadPlayerEvent>					(this, &PlayerUICreator::Deactivate);
	EventSystem::GetInstance()->Unsubscribe<StartRocketLauncherCutsceneEvent>	(this, &PlayerUICreator::ActivateCutscene);
	EventSystem::GetInstance()->Unsubscribe<EndRocketLauncherCutsceneEvent>		(this, &PlayerUICreator::DeactivateCutscene);
}

void PlayerUICreator::Init()
{
	m_is_active = true;
}

void PlayerUICreator::LateUpdate()
{
	if (!m_is_active) { return; }

	m_melee_target_icon	->LateUpdate();
	m_escape_icon		->LateUpdate();
	m_status_icons		->LateUpdate();
	m_cross_hair		->LateUpdate();
	m_weapon_shortcut	->LateUpdate();

	CalcAlphaBlendNum();
}

void PlayerUICreator::OnDraw(const int main_screen_handle)
{
	if (!m_is_active) { return; }

	m_result_screen->UseScreen();

	m_melee_target_icon	->Draw();
	m_escape_icon		->Draw();
	m_status_icons		->Draw();
	m_cross_hair		->Draw();
	m_weapon_shortcut	->Draw(main_screen_handle);

	m_result_screen->UnuseScreen();
	m_result_screen->Draw();
}


#pragma region Event
void PlayerUICreator::Deactivate(const DeadPlayerEvent& event)
{
	m_is_active = false;
}

void PlayerUICreator::ActivateCutscene(const StartRocketLauncherCutsceneEvent& event)
{
	m_is_active_cutscene = true;
}

void PlayerUICreator::DeactivateCutscene(const EndRocketLauncherCutsceneEvent& event)
{
	m_is_active_cutscene = false;
}
#pragma endregion


void PlayerUICreator::CalcAlphaBlendNum()
{
	// TODO : マジックナンバーの削除

	const auto delta_time = GameTimeManager::GetInstance()->GetDeltaTime(TimeScaleLayerKind::kUI);

	if (m_is_active_cutscene)
	{
		math::Decrease(m_alpha_blend_num, static_cast<int>(700.0f * delta_time), 0);
	}
	else
	{
		math::Increase(m_alpha_blend_num, static_cast<int>(700.0f * delta_time), 255, false);
	}

	m_result_screen->GetGraphicer()->SetBlendNum(m_alpha_blend_num);
}
