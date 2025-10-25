#pragma once
#include "../Interface/i_ui_creator.hpp"

#include "melee_target_icon.hpp"
#include "escape_icon.hpp"
#include "status_icons.hpp"
#include "weapon_shortcut_drawer.hpp"
#include "cross_hair.hpp"

#include "../Part/player_state_controller.hpp"

class PlayerUICreator final : public IUICreator
{
public:
	PlayerUICreator(std::shared_ptr<Player>& player);
	~PlayerUICreator()	override;

	void Init() override;
	void LateUpdate() override;
	void OnDraw(const int main_screen_handle) override;

	/// @brief アクティブ化する
	void Activate() 	override { m_is_active = true; }
	/// @brief 非アクティブ化する(削除せずに機能を停止)
	void Deactivate()	override { m_is_active = false; }


	#pragma region Event
	void Deactivate(const DeadPlayerEvent& event);
	#pragma endregion


	[[nodiscard]] std::string GetName()		const override { return m_name; }
	[[nodiscard]] int		  GetPriority() const override { return m_priority; }
	[[nodiscard]] bool		  IsActive()	const override { return m_is_active; }

private:
	std::string								m_name;
	int										m_priority;
	bool									m_is_active;

	std::shared_ptr<MeleeTargetIcon>		m_melee_target_icon;
	std::shared_ptr<EscapeIcon>				m_escape_icon;
	std::shared_ptr<StatusIcons>			m_status_icons;
	std::shared_ptr<CrossHair>				m_cross_hair;
	std::shared_ptr<WeaponShortcutDrawer>	m_weapon_shortcut;
};
