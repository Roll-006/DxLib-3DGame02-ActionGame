#pragma once
#include "../Part/screen_creator.hpp"
#include "../Part/mask_creator.hpp"
#include "../Part/health.hpp"

#include "../Path/ui_graphic_path.hpp"

class HealthGauge final
{
public:
	HealthGauge(std::shared_ptr<Health>& health);
	~HealthGauge();

	void LateUpdate();

	[[nodiscard]] int GetScreenHandle() const { return m_result_screen->GetScreenHandle(); }

private:
	void CreateCurrentHealthGaugeScreen();
	void CreateHealthGaugeScreen();
	void CreateResultScreen();

	void CalcGaugePercent();
	void ChangeGaugeColor();

private:
	static constexpr Vector2D<int>	kScreenSize			= { 256, 256 };
	static constexpr float			kMaxGaugePercent	= 75.0f;
	static constexpr int			kBaseGaugeColor		= 0x404040;
	static constexpr int			kRadius				= 71;
	static constexpr int			kThickness			= 9;
	static constexpr int			kMaxHue				= 0;
	static constexpr int			kMinHue				= -90;

	std::shared_ptr<Health>&		m_health;
	std::shared_ptr<ScreenCreator>	m_circle_screen;				// ゲージに使用する円を描画するスクリーン
	std::shared_ptr<ScreenCreator>	m_current_health_gauge_screen;	// 現在のHPを描画するスクリーン
	std::shared_ptr<ScreenCreator>	m_gauge_screen;					// 現在のHPゲージを除くゲージを描画するスクリーン
	std::shared_ptr<ScreenCreator>	m_result_screen;				// 最終的な描画結果を描画するスクリーン
	std::shared_ptr<MaskCreator>	m_mask_creator;
	std::shared_ptr<Graphicer>		m_graphicer;

	float m_current_health_gauge_percent;
	float m_current_max_health_gauge_percent;
	float m_prev_health;							// 1フレーム前のHP
	float m_prev_max_health;						// 1フレーム前の最大HP

	int	  m_gauge_hue;								// ゲージの色相
};
