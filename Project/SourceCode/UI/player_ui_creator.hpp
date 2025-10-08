#pragma once
#include "../Interface/i_ui_creator.hpp"

#include "melee_target_icon.hpp"
#include "status_icons.hpp"
#include "weapon_shortcut_drawer.hpp"

#include "../Part/player_state_controller.hpp"

class PlayerUICreator final : public IUICreator
{
public:
	PlayerUICreator(const std::shared_ptr<Player>& player);
	~PlayerUICreator()	override;

	void LateUpdate() override;
	void OnDraw(const int main_screen_handle) override;

	/// @brief アクティブ化する
	void Activate() 	override { m_is_active = true; }
	/// @brief 非アクティブ化する(削除せずに機能を停止)
	void Deactivate()	override { m_is_active = false; }

	[[nodiscard]] std::string GetName()		const override { return m_name; }
	[[nodiscard]] int		  GetPriority() const override { return m_priority; }
	[[nodiscard]] bool		  IsActive()	const override { return m_is_active; }

private:
	///// @brief 銃のクロスヘアを生成する
	//void CreateCrossHair();
	//void CreateSniperRifleCrossHair();

private:
	std::string m_name;
	int			m_priority;
	bool		m_is_active;

	std::shared_ptr<Player>					m_player;
	std::shared_ptr<MeleeTargetIcon>		m_melee_target_icon;
	std::shared_ptr<StatusIcons>			m_status_icons;
	std::shared_ptr<WeaponShortcutDrawer>	m_weapon_shortcut;

	//std::shared_ptr<ShapeBase>	m_2d_diffusion_shape;	// 二次元上の拡散範囲図形
};