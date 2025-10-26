#include "player_ui_creator.hpp"

PlayerUICreator::PlayerUICreator(std::shared_ptr<Player>& player) : 
	m_name				(UICreatorName.PLAYER_UI_CREATOR),
	m_priority			(10),
	m_is_active			(true),
	m_melee_target_icon	(std::make_shared<MeleeTargetIcon>(player->GetMeleeTarget(), player->GetTopPriorityVisibleDownedCharacter())),
	m_escape_icon		(std::make_shared<EscapeIcon>(std::dynamic_pointer_cast<IGrabbable>(player))),
	m_status_icons		(std::make_shared<StatusIcons>(player)),
	m_cross_hair		(std::make_shared<CrossHair>(player)),
	m_weapon_shortcut	(std::make_shared<WeaponShortcutDrawer>(
							player->GetStateController()->GetWeaponActionState(),
							player->GetWeaponShortcutSelecter()))
{
	// ƒCƒxƒ“ƒg“o˜^
	EventSystem::GetInstance()->Subscribe<DeadPlayerEvent>(this, &PlayerUICreator::Deactivate);
}

PlayerUICreator::~PlayerUICreator()
{
	// ƒCƒxƒ“ƒg‚Ì“o˜^‰ðœ
	EventSystem::GetInstance()->Unsubscribe<DeadPlayerEvent>(this, &PlayerUICreator::Deactivate);
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
}

void PlayerUICreator::OnDraw(const int main_screen_handle)
{
	if (!m_is_active) { return; }

	m_melee_target_icon	->Draw(main_screen_handle);
	m_escape_icon		->Draw();
	m_status_icons		->Draw();
	m_cross_hair		->Draw();
	m_weapon_shortcut	->Draw(main_screen_handle);
}


#pragma region Event
void PlayerUICreator::Deactivate(const DeadPlayerEvent& event)
{
	m_is_active = false;
}
#pragma endregion
