#pragma once
#include "../Interface/i_ui_creator.hpp"

#include "weapon_shortcut_drawer.hpp"
#include "hit_points_gauge.hpp"

#include "../Part/player_state_controller.hpp"

class PlayerUICreator final : public IUICreator
{
public:
	PlayerUICreator(const std::shared_ptr<Player> player);
	~PlayerUICreator()	override;

	void LateUpdate()	override;
	void OnDraw()		override;

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
	std::shared_ptr<WeaponShortcutDrawer>	m_weapon_shortcut;

	// 仮
	std::shared_ptr<HitPointsGauge> m_sample_hit_points_gauge;

	// 仮
	//std::shared_ptr<ShapeBase>	m_2d_diffusion_shape;	// 二次元上の拡散範囲図形
};