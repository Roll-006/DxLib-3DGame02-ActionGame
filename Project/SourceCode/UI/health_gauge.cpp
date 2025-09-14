#include "health_gauge.hpp"

HealthGauge::HealthGauge(std::shared_ptr<Health>& health) : 
	m_health							(health),
	m_current_health_gauge_graphic		(std::make_shared<Graphicer>(UIGraphicPath.CURRENT_HEALTH_GAUGE)),
	m_damage_gauge_graphic				(std::make_shared<Graphicer>(UIGraphicPath.DAMAGE_GAUGE)),
	m_recover_gauge_graphic				(std::make_shared<Graphicer>(UIGraphicPath.RECOVER_GAUGE)),
	m_basis_circle_screen				(std::make_shared<ScreenCreator>(kScreenSize)),
	m_frame_circle_screen				(std::make_shared<ScreenCreator>(kScreenSize)),
	m_current_health_gauge_screen		(std::make_shared<ScreenCreator>(kScreenSize)),
	m_damage_gauge_screen				(std::make_shared<ScreenCreator>(kScreenSize)),
	m_recover_gauge_screen				(std::make_shared<ScreenCreator>(kScreenSize)),
	m_gauge_screen						(std::make_shared<ScreenCreator>(kScreenSize)),
	m_result_screen						(std::make_shared<ScreenCreator>(kScreenSize)),
	m_current_health_mask_screen		(std::make_shared<ScreenCreator>(m_current_health_gauge_graphic->GetOriginSize())),
	m_damage_mask_screen				(std::make_shared<ScreenCreator>(m_damage_gauge_graphic->GetOriginSize())),
	m_recover_mask_screen				(std::make_shared<ScreenCreator>(m_recover_gauge_graphic->GetOriginSize())),
	m_mask_creator						(std::make_shared<MaskCreator>()),
	m_current_health_gauge_percent		(0.0f),
	m_prev_health_gauge_percent			(0.0f),
	m_current_max_health_gauge_percent	(0.0f),
	m_damage_gauge_percent				(0.0f),
	m_recover_gauge_percent				(0.0f),
	m_prev_health						(m_health->GetCurrentHealth()),
	m_prev_max_health					(m_health->GetCurrentMaxHealth()),
	m_is_recover_gauge					(false),
	m_gauge_hue							(kMaxHue),
	m_prev_gauge_hue					(m_gauge_hue)
{
	// ゲージに使用する円をスクリーン化
	m_basis_circle_screen->UseScreen();
	DrawCircle(
		static_cast<int>(kScreenSize.x * 0.5f),
		static_cast<int>(kScreenSize.y * 0.5f),
		kRadius, kBaseGaugeColor, TRUE, kThickness);
	m_basis_circle_screen->UnuseScreen();

	m_frame_circle_screen->UseScreen();
	DrawCircle(
		static_cast<int>(kScreenSize.x * 0.5f),
		static_cast<int>(kScreenSize.y * 0.5f),
		static_cast<int>(kRadius - kThickness * 0.5f + 0.5f), 
		0xffffff, FALSE, 1);
	m_frame_circle_screen->UnuseScreen();

	// 中心座標を設定
	m_gauge_screen->GetGraphicer()->SetCenterPos(Vector2D<int>(kScreenSize.x * 0.5f, kScreenSize.y * 0.5f));
	m_current_health_gauge_graphic->SetCenterPos(Vector2D<int>(kScreenSize.x * 0.5f, kScreenSize.y * 0.5f));
	m_current_health_mask_screen->GetGraphicer()->SetCenterPos(m_current_health_gauge_graphic->GetCenterPos());
	m_damage_gauge_graphic->SetCenterPos(Vector2D<int>(kScreenSize.x * 0.5f, kScreenSize.y * 0.5f));
	m_damage_mask_screen->GetGraphicer()->SetCenterPos(m_damage_gauge_graphic->GetCenterPos());
	m_recover_gauge_graphic->SetCenterPos(Vector2D<int>(kScreenSize.x * 0.5f, kScreenSize.y * 0.5f));
	m_recover_mask_screen->GetGraphicer()->SetCenterPos(m_recover_gauge_graphic->GetCenterPos());
	m_frame_circle_screen->GetGraphicer()->SetCenterPos(Vector2D<int>(kScreenSize.x * 0.5f, kScreenSize.y * 0.5f));

	// マスク用スクリーンを生成
	m_current_health_mask_screen->UseScreen();
	m_current_health_gauge_graphic->Draw();
	m_current_health_mask_screen->UnuseScreen();
	GraphFilter(m_current_health_mask_screen->GetScreenHandle(), DX_GRAPH_FILTER_HSB, 0, m_gauge_hue, 0, 0);

	m_damage_mask_screen->UseScreen();
	m_damage_gauge_graphic->Draw();
	m_damage_mask_screen->UnuseScreen();

	m_recover_mask_screen->UseScreen();
	m_recover_gauge_graphic->Draw();
	m_recover_mask_screen->UnuseScreen();

	// ゲージ率
	CalcGaugePercent();
	m_current_max_health_gauge_percent	= m_current_health_gauge_percent;
	m_damage_gauge_percent				= m_current_health_gauge_percent;
	m_recover_gauge_percent				= m_current_health_gauge_percent;
}

HealthGauge::~HealthGauge()
{

}

void HealthGauge::LateUpdate()
{
	CalcGaugePercent();
	ChangeGaugeColor();

	CreateCurrentHealthGaugeScreen();
	CreateDamageGaugeScreen();
	CreateRecoverGaugeScreen();
	CreateHealthGaugeScreen();
	CreateResultScreen();

	m_prev_health				= m_health->GetCurrentHealth();
	m_prev_max_health			= m_health->GetCurrentMaxHealth();
	m_prev_health_gauge_percent = m_health->GetCurrentHealth() / m_health->GetMaxHealth() * kMaxGaugePercent;
}


#pragma region スクリーン生成
void HealthGauge::CreateCurrentHealthGaugeScreen()
{
	m_current_health_gauge_screen->UseScreen();

	ClearDrawScreen();
	DrawCircleGauge(
		static_cast<int>(kScreenSize.x * 0.5f),
		static_cast<int>(kScreenSize.y * 0.5f),
		static_cast<double>(m_current_health_gauge_percent),
		m_basis_circle_screen->GetScreenHandle(), 0.0, 1.0);

	m_current_health_gauge_screen->UnuseScreen();
}

void HealthGauge::CreateDamageGaugeScreen()
{
	m_damage_gauge_screen->UseScreen();

	ClearDrawScreen();
	DrawCircleGauge(
		static_cast<int>(kScreenSize.x * 0.5f),
		static_cast<int>(kScreenSize.y * 0.5f),
		static_cast<double>(m_damage_gauge_percent),
		m_basis_circle_screen->GetScreenHandle(), 0.0, 1.0);

	m_damage_gauge_screen->UnuseScreen();
}

void HealthGauge::CreateRecoverGaugeScreen()
{
	m_recover_gauge_screen->UseScreen();

	ClearDrawScreen();
	DrawCircleGauge(
		static_cast<int>(kScreenSize.x * 0.5f),
		static_cast<int>(kScreenSize.y * 0.5f),
		static_cast<double>(m_recover_gauge_percent),
		m_basis_circle_screen->GetScreenHandle(), 0.0, 1.0);

	m_recover_gauge_screen->UnuseScreen();
}

void HealthGauge::CreateHealthGaugeScreen()
{
	m_gauge_screen->UseScreen();
	ClearDrawScreen();

	// 最大HPフレーム
	DrawCircleGauge(
		static_cast<int>(kScreenSize.x * 0.5f),
		static_cast<int>(kScreenSize.y * 0.5f),
		75.0, m_frame_circle_screen->GetScreenHandle(), 0.0, 1.0);

	// 現在の最大HPゲージ
	DrawCircleGauge(
		static_cast<int>(kScreenSize.x * 0.5f),
		static_cast<int>(kScreenSize.y * 0.5f),
		static_cast<double>(m_current_max_health_gauge_percent),
		m_basis_circle_screen->GetScreenHandle(), 0.0, 1.0);

	// 回復した際のゲージ
	m_mask_creator->UseMask(m_recover_gauge_screen->GetScreenHandle(), true);
	m_recover_mask_screen->Draw();
	m_mask_creator->UnuseMask();

	// ダメージを受けた際のゲージ
	m_mask_creator->UseMask(m_damage_gauge_screen->GetScreenHandle(), true);
	m_damage_mask_screen->Draw();
	m_mask_creator->UnuseMask();

	m_gauge_screen->UnuseScreen();
}

void HealthGauge::CreateResultScreen()
{
	m_result_screen->UseScreen();

	m_gauge_screen->Draw();

	// ゲージ画像を現在のHP分だけ描画
	m_mask_creator->UseMask(m_current_health_gauge_screen->GetScreenHandle(), true);
	m_current_health_mask_screen->Draw();
	m_mask_creator->UnuseMask();

	m_result_screen->UnuseScreen();
}
#pragma endregion


void HealthGauge::CalcGaugePercent()
{
	const auto delta_time = GameTimeManager::GetInstance()->GetDeltaTime(TimeScaleController::LayerKind::kUI);
	const auto max_health = m_health->GetMaxHealth();

	if (m_health->GetCurrentHealth() <= m_prev_health && !m_is_recover_gauge)
	{
		m_current_health_gauge_percent = m_health->GetCurrentHealth() / max_health * kMaxGaugePercent;
		m_recover_gauge_percent = 0.0f;
	}
	else
	{
		m_is_recover_gauge = true;

		// 現在のHPのゲージ率を、徐々に回復ゲージ率に近づける
		m_recover_gauge_percent = m_health->GetCurrentHealth() / max_health * kMaxGaugePercent;
		math::Increase(m_current_health_gauge_percent, kIncreaseSpeed * delta_time, m_recover_gauge_percent, false);
		if (m_recover_gauge_percent == m_current_health_gauge_percent)
		{
			m_is_recover_gauge = false;
		}
	}

	// 最大HPのゲージ率
	m_current_max_health_gauge_percent	= m_health->GetCurrentMaxHealth() / max_health * kMaxGaugePercent;

	// ダメージを受けた際の減少ゲージ率
	math::Decrease(m_damage_gauge_percent, kDecreaseSpeed * delta_time, m_current_health_gauge_percent);
}

void HealthGauge::ChangeGaugeColor()
{
	if (!m_prev_health == m_health->GetCurrentHealth() && !m_is_recover_gauge) { return; }

	// 真円を100%とた時、12.5%未満の場合
	if (m_current_health_gauge_percent < 12.5f)
	{
		// ゲージ回復中は12.5%未満で合っても12.5%以上の色を採用する
		if (m_is_recover_gauge)
		{
			if (m_prev_gauge_hue == kMiddleHue) { return; }
			m_gauge_hue = kMiddleHue;
		}
		else
		{
			if (m_prev_gauge_hue == kMinHue) { return; }
			m_gauge_hue = kMinHue;
		}
	}
	// 12.5%以上・37.5%未満の場合
	else if (m_current_health_gauge_percent < 37.5f)
	{
		m_gauge_hue = static_cast<int>(math::ConvertValueNewRange<float, float>(12.5f, 37.5f, kMiddleHue, kMaxHue, m_current_health_gauge_percent));
	}
	// 37.5%以上の場合
	else
	{
		if (m_prev_gauge_hue == kMaxHue) { return; }
		m_gauge_hue = kMaxHue;
	}

	// DxLib-GraphFilterの色相変更は不可逆的であり現在の色相値を0として変更値が適用されるため、
	// 色の変換が行われるたびにスクリーンを再構築する
	m_current_health_mask_screen = std::make_shared<ScreenCreator>(m_current_health_gauge_graphic->GetOriginSize());
	m_current_health_mask_screen->GetGraphicer()->SetCenterPos(m_current_health_gauge_graphic->GetCenterPos());

	// スクリーン生成
	m_current_health_mask_screen->UseScreen();
	m_current_health_gauge_graphic->Draw();
	m_current_health_mask_screen->UnuseScreen();
	GraphFilter(m_current_health_mask_screen->GetScreenHandle(), DX_GRAPH_FILTER_HSB, 0, m_gauge_hue, 0, 0);

	m_prev_gauge_hue = m_gauge_hue;
}
