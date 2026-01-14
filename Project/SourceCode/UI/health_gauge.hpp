#pragma once
#include "../Part/screen_creator.hpp"
#include "../Part/mask_creator.hpp"
#include "../Part/gauge.hpp"

#include "../Path/ui_graphic_path.hpp"

class HealthGauge final
{
public:
	HealthGauge(const std::shared_ptr<Gauge>& health);
	~HealthGauge();

	void LateUpdate();

	[[nodiscard]] int GetScreenHandle() const { return m_result_screen->GetScreenHandle(); }

private:
	#pragma region スクリーン生成
	void CreateCurrentHealthGaugeScreen();
	void CreateWarningGaugeScreen();
	void CreateDamageGaugeScreen();
	void CreateRecoverGaugeScreen();
	void CreateHealthGaugeScreen();
	void CreateResultScreen();
	#pragma endregion


	/// @brief ゲージのパーセントを計算する
	void CalcGaugePercent();

	/// @brief 警告透過値を計算する
	void CalcBlinkingAlphaBlendNum();

	/// @brief ゲージの粒子の角度を計算する
	void CalcGaugeParticleAngle();

	/// @brief ゲージの色を変更する
	void ChangeGaugeColor();

private:
	static constexpr Vector2D<int>	kScreenSize					= { 256, 256 };
	static constexpr Vector2D<int>	kCenterPos					= { static_cast<int>(kScreenSize.x * 0.5f), static_cast<int>(kScreenSize.y * 0.5f) };
	static constexpr float			kMaxGaugePercent			= 75.0f;
	static constexpr int			kBaseGaugeColor				= 0x363636;
	static constexpr int			kWarningGaugeColor			= 0xe32d2d;
	static constexpr int			kRadius						= 90;
	static constexpr int			kBasisGaugeThickness		= 11;
	static constexpr int			kWarningGaugeThickness		= 50;
	static constexpr int			kMaxHealthFrameThickness	= 2;
	static constexpr int			kMaxHue						= 0;
	static constexpr int			kMiddleHue					= -90;
	static constexpr int			kMinHue						= -170;

	static constexpr float			kGaugeIncreaseSpeed			= 8.0f;
	static constexpr float			kBlinkingSpeed				= 6.0f;

private:
	std::shared_ptr<Gauge>			m_health;

	std::shared_ptr<Graphicer>		m_current_health_gauge_graphic;
	std::shared_ptr<Graphicer>		m_gauge_particle_graphic;
	std::shared_ptr<Graphicer>		m_damage_particle_graphic;
	std::shared_ptr<Graphicer>		m_frame_particle_graphic;
	std::shared_ptr<Graphicer>		m_warning_circle_graphic;

	std::shared_ptr<ScreenCreator>	m_basis_circle_screen;						// ゲージに使用する円を描画するスクリーン
	std::shared_ptr<ScreenCreator>	m_warning_circle_screen;					// 警告ゲージに使用する円を描画するスクリーン
	std::shared_ptr<ScreenCreator>	m_current_max_health_frame_circle_screen;
	std::shared_ptr<ScreenCreator>	m_max_health_frame_circle_screen;
	
	std::shared_ptr<ScreenCreator>	m_current_health_gauge_screen;				// 現在のHPを描画するスクリーン
	std::shared_ptr<ScreenCreator>	m_damage_gauge_screen;						// ダメージ量を描画するスクリーン
	std::shared_ptr<ScreenCreator>	m_recover_gauge_screen;						// 回復量を描画するスクリーン
	std::shared_ptr<ScreenCreator>	m_max_health_frame_gauge_screen;
	std::shared_ptr<ScreenCreator>	m_warning_gauge_screen;						// ダメージを受けたことを警告するゲージを描画するスクリーン
	std::shared_ptr<ScreenCreator>	m_gauge_screen;								// 現在のHPゲージを除くゲージを描画するスクリーン
	
	std::shared_ptr<ScreenCreator>	m_result_screen;							// 最終的な描画結果を描画するスクリーン
	
	std::shared_ptr<ScreenCreator>	m_current_health_mask_screen;
	std::shared_ptr<MaskCreator>	m_mask_creator;

	float m_current_health_gauge_actual_percent;			// 現在のHPゲージのパーセント(実際の値)
	float m_current_health_gauge_virtual_percent;			// 現在のHPゲージのパーセント(見かけ上の値)
	float m_prev_health_gauge_actual_percent;				// 1フレーム前のHPゲージのパーセント(実際の値)
	float m_prev_health_gauge_virtual_percent;				// 1フレーム前のHPゲージのパーセント(見かけ上の値)
	float m_current_max_health_gauge_virtual_percent;		// 現在の最大HPゲージのパーセント(見かけ上の値)
	float m_damage_gauge_percent;
	float m_recover_gauge_percent;

	float m_prev_health;									// 1フレーム前のHP
	float m_prev_max_health;								// 1フレーム前の最大HP
	bool  m_is_recover_gauge;								// ゲージを回復中であるかを判定

	int	  m_gauge_hue;										// ゲージの色相
	int   m_prev_gauge_hue;

	float m_gauge_particle_angle;
	float m_blinking_sin;
	int	  m_blinking_alpha_blend_num;
};
