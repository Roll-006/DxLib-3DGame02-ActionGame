#include "health_gauge.hpp"

HealthGauge::HealthGauge(std::shared_ptr<Health>& health) : 
	m_health							(health),
	m_circle_screen						(std::make_shared<ScreenCreator>(kScreenSize)),
	m_current_health_gauge_screen		(std::make_shared<ScreenCreator>(kScreenSize)),
	m_gauge_screen						(std::make_shared<ScreenCreator>(kScreenSize)),
	m_result_screen						(std::make_shared<ScreenCreator>(kScreenSize)),
	m_mask_creator						(std::make_shared<MaskCreator>()),
	m_graphicer							(std::make_shared<Graphicer>(UIGraphicPath.HEALTH_GAUGE)),
	m_current_health_gauge_percent		(0.0f),
	m_current_max_health_gauge_percent	(0.0f),
	m_prev_health						(m_health->GetCurrentHealth()),
	m_prev_max_health					(m_health->GetCurrentMaxHealth()),
	m_gauge_hue							(kMaxHue)
{
	// ゲージに使用する円をスクリーン化
	m_circle_screen->UseScreen();
	DrawCircle(
		static_cast<int>(kScreenSize.x * 0.5f),
		static_cast<int>(kScreenSize.y * 0.5f),
		kRadius, kBaseGaugeColor, TRUE, kThickness);
	m_circle_screen->UnuseScreen();

	m_gauge_screen->GetGraphicer()->SetCenterPos(Vector2D<int>(kScreenSize.x * 0.5f, kScreenSize.y * 0.5f));
	m_graphicer->SetCenterPos(Vector2D<int>(kScreenSize.x * 0.5f, kScreenSize.y * 0.5f));
}

HealthGauge::~HealthGauge()
{

}

void HealthGauge::LateUpdate()
{
	CalcGaugePercent();
	ChangeGaugeColor();

	CreateCurrentHealthGaugeScreen();
	CreateHealthGaugeScreen();
	CreateResultScreen();
}

void HealthGauge::CreateCurrentHealthGaugeScreen()
{
	m_current_health_gauge_screen->UseScreen();

	ClearDrawScreen();
	DrawCircleGauge(
		static_cast<int>(kScreenSize.x * 0.5f),
		static_cast<int>(kScreenSize.y * 0.5f),
		static_cast<double>(m_current_health_gauge_percent),
		m_circle_screen->GetScreenHandle(), 0.0, 1.0);

	m_current_health_gauge_screen->UnuseScreen();
}

void HealthGauge::CreateHealthGaugeScreen()
{
	m_gauge_screen->UseScreen();

	ClearDrawScreen();
	DrawCircleGauge(
		static_cast<int>(kScreenSize.x * 0.5f),
		static_cast<int>(kScreenSize.y * 0.5f),
		static_cast<double>(m_current_max_health_gauge_percent),
		m_circle_screen->GetScreenHandle(), 0.0, 1.0);

	m_gauge_screen->UnuseScreen();
}

void HealthGauge::CreateResultScreen()
{
	m_result_screen->UseScreen();

	m_gauge_screen->Draw();

	// ゲージ画像を現在のHP分だけ描画
	m_mask_creator->UseMask(m_current_health_gauge_screen->GetScreenHandle(), true);
	m_graphicer->Draw();
	m_mask_creator->UnuseMask();

	m_result_screen->UnuseScreen();
}

void HealthGauge::CalcGaugePercent()
{
	const auto max = m_health->GetMaxHealth();

	m_current_health_gauge_percent		= m_health->GetCurrentHealth()	  / max * kMaxGaugePercent;
	m_current_max_health_gauge_percent	= m_health->GetCurrentMaxHealth() / max * kMaxGaugePercent;
}

void HealthGauge::ChangeGaugeColor()
{

}
