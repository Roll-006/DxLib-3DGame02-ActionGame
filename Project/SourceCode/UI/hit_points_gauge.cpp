#include "hit_points_gauge.hpp"

HitPointsGauge::HitPointsGauge(std::shared_ptr<HitPoints>& hit_points) : 
	m_hit_points							(hit_points),
	m_circle_screen							(std::make_shared<ScreenCreator>(kScreenSize)),
	m_current_hit_points_gauge_screen		(std::make_shared<ScreenCreator>(kScreenSize)),
	m_gauge_screen							(std::make_shared<ScreenCreator>(kScreenSize)),
	m_graphicer								(std::make_shared<Graphicer>("Data/Graphic/UI/HitPointGauge/gradation.png")),
	m_current_hit_points_gauge_percent		(0.0f),
	m_current_max_hit_points_gauge_percent	(0.0f)
{
	SetMaskReverseEffectFlag(TRUE);

	// ゲージに使用する円をスクリーン化
	m_circle_screen->UseScreen();
	DrawCircle(
		static_cast<int>(kScreenSize.x * 0.5f),
		static_cast<int>(kScreenSize.y * 0.5f),
		static_cast<int>(kScreenSize.x * 0.5f - kThickness),
		kBaseGaugeColor, TRUE, kThickness);
	m_circle_screen->UnuseScreen();

	m_gauge_screen->GetGraphicer()->SetCenterPos(Vector2D<int>(kScreenSize.x * 0.5f, kScreenSize.y * 0.5f));
	m_graphicer->SetCenterPos(Vector2D<int>(kScreenSize.x * 0.5f, kScreenSize.y * 0.5f));
}

HitPointsGauge::~HitPointsGauge()
{

}

void HitPointsGauge::LateUpdate()
{
	const auto max = m_hit_points->GetMaxHitPoints();
	
	m_current_hit_points_gauge_percent		= m_hit_points->GetCurrentHitPoints()	 / max * kMaxGaugePercent;
	m_current_max_hit_points_gauge_percent	= m_hit_points->GetCurrentMaxHitPoints() / max * kMaxGaugePercent;
}

void HitPointsGauge::Draw() const
{
	// 現在のHPゲージをスクリーン化
	m_current_hit_points_gauge_screen->UseScreen();
	ClearDrawScreen();
	DrawCircleGauge(
		static_cast<int>(kScreenSize.x * 0.5f),
		static_cast<int>(kScreenSize.y * 0.5f),
		static_cast<double>(m_current_hit_points_gauge_percent),
		m_circle_screen->GetScreenHandle(), 0.0, 1.0);
	m_current_hit_points_gauge_screen->UnuseScreen();

	// HPゲージをスクリーン化
	m_gauge_screen->UseScreen();
	ClearDrawScreen();
	DrawCircleGauge(
		static_cast<int>(kScreenSize.x * 0.5f),
		static_cast<int>(kScreenSize.y * 0.5f),
		static_cast<double>(m_current_max_hit_points_gauge_percent),
		m_circle_screen->GetScreenHandle(), 0.0, 1.0);
	m_gauge_screen->UnuseScreen();
	m_gauge_screen->Draw();

	// ゲージ用画像にマスクを適用
	CreateMaskScreen();
	SetMaskScreenGraph(m_current_hit_points_gauge_screen->GetScreenHandle());
	SetUseMaskScreenFlag(TRUE);
	m_graphicer->Draw();
	SetUseMaskScreenFlag(FALSE);
	DeleteMaskScreen();

	// 色を調整
	//GraphFilter(m_graphicer->GetGraphicHandle(), DX_GRAPH_FILTER_HSB, 0, 100, 90, -60);
}
