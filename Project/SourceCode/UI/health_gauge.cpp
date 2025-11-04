#include "health_gauge.hpp"

// MEMO : パラメーターでゲージサイズや太さの変更を可能にするために
//		　スクリーン生成およびマスクを使用しゲージを生成する

HealthGauge::HealthGauge(std::shared_ptr<Gauge>& health) : 
	m_health									(health),
	m_current_health_gauge_graphic				(std::make_shared<Graphicer>(UIGraphicPath.CURRENT_HEALTH_GAUGE)),
	m_gauge_particle_graphic					(std::make_shared<Graphicer>(UIGraphicPath.HEALTH_GAUGE_PARTICLE)),
	m_damage_particle_graphic					(std::make_shared<Graphicer>(UIGraphicPath.DAMAGE_GAUGE_PARTICLE)),
	m_frame_particle_graphic					(std::make_shared<Graphicer>(UIGraphicPath.GAUGE_FRAME_PARTICLE)),
	m_warning_circle_graphic					(std::make_shared<Graphicer>(UIGraphicPath.CIRCLE_BLUR_RED)),
	m_basis_circle_screen						(std::make_shared<ScreenCreator>(kScreenSize)),
	m_warning_circle_screen						(std::make_shared<ScreenCreator>(kScreenSize)),
	m_current_max_health_frame_circle_screen	(std::make_shared<ScreenCreator>(kScreenSize)),
	m_max_health_frame_circle_screen			(std::make_shared<ScreenCreator>(kScreenSize)),
	m_current_health_gauge_screen				(std::make_shared<ScreenCreator>(kScreenSize)),
	m_damage_gauge_screen						(std::make_shared<ScreenCreator>(kScreenSize)),
	m_recover_gauge_screen						(std::make_shared<ScreenCreator>(kScreenSize)),
	m_max_health_frame_gauge_screen				(std::make_shared<ScreenCreator>(kScreenSize)),
	m_gauge_screen								(std::make_shared<ScreenCreator>(kScreenSize, kCenterPos)),
	m_result_screen								(std::make_shared<ScreenCreator>(kScreenSize)),
	m_current_health_mask_screen				(std::make_shared<ScreenCreator>(m_current_health_gauge_graphic->GetOriginSize(), kCenterPos)),
	m_warning_gauge_screen						(nullptr),
	m_mask_creator								(std::make_shared<MaskCreator>()),
	m_current_health_gauge_virtual_percent		(0.0f),
	m_prev_health_gauge_virtual_percent			(0.0f),
	m_current_max_health_gauge_virtual_percent	(0.0f),
	m_damage_gauge_percent						(0.0f),
	m_recover_gauge_percent						(0.0f),
	m_prev_health								(m_health->GetCurrentValue()),
	m_prev_max_health							(m_health->GetCurrentMaxValue()),
	m_is_recover_gauge							(false),
	m_gauge_hue									(kMaxHue),
	m_prev_gauge_hue							(m_gauge_hue),
	m_gauge_particle_angle						(0.0f),
	m_blinking_sin								(0.0f),
	m_blinking_alpha_blend_num					(0)
{
	// ゲージに使用する円をスクリーン化
	m_basis_circle_screen->UseScreen();
	DrawCircle(kCenterPos.x, kCenterPos.y, kRadius, kBaseGaugeColor, TRUE, kBasisGaugeThickness);
	m_basis_circle_screen->UnuseScreen();

	m_warning_circle_screen->UseScreen();
	DrawCircle(kCenterPos.x, kCenterPos.y, kRadius, kWarningGaugeColor, TRUE, kWarningGaugeThickness);
	m_warning_circle_screen->UnuseScreen();

	m_current_max_health_frame_circle_screen->UseScreen();
	DrawCircle(kCenterPos.x, kCenterPos.y, kRadius, 0xffffff, TRUE, kBasisGaugeThickness + 2);
	m_current_max_health_frame_circle_screen->UnuseScreen();

	m_max_health_frame_circle_screen->UseScreen();
	DrawCircle(
		kCenterPos.x, kCenterPos.y, 
		static_cast<int>(kRadius - kBasisGaugeThickness * 0.5f + kMaxHealthFrameThickness * 0.5f),
		0xffffff, FALSE, kMaxHealthFrameThickness);
	m_max_health_frame_circle_screen->UnuseScreen();

	m_max_health_frame_gauge_screen->UseScreen();
	DrawCircleGauge(kCenterPos.x, kCenterPos.y, 75.0, m_max_health_frame_circle_screen->GetScreenHandle(), 0.0, 1.0);
	m_max_health_frame_gauge_screen->UnuseScreen();


	// 中心座標を設定
	m_current_health_gauge_graphic	->SetCenterPos(kCenterPos);
	m_gauge_particle_graphic		->SetCenterPos(kCenterPos);
	m_damage_particle_graphic		->SetCenterPos(kCenterPos);
	m_frame_particle_graphic		->SetCenterPos(kCenterPos);
	m_warning_circle_graphic		->SetCenterPos(kCenterPos);

	m_warning_circle_graphic->SetBlendNum(0);


	// マスク用スクリーンを生成
	m_current_health_mask_screen->UseScreen();
	m_current_health_gauge_graphic->Draw();
	m_current_health_mask_screen->UnuseScreen();
	GraphFilter(m_current_health_mask_screen->GetScreenHandle(), DX_GRAPH_FILTER_HSB, 0, m_gauge_hue, 0, 0);


	// ゲージ率
	CalcGaugePercent();
	m_current_max_health_gauge_virtual_percent	= m_current_health_gauge_virtual_percent;
	m_damage_gauge_percent	= m_current_health_gauge_virtual_percent;
	m_recover_gauge_percent	= m_current_health_gauge_virtual_percent;
}

HealthGauge::~HealthGauge()
{

}

void HealthGauge::LateUpdate()
{
	CalcGaugePercent();
	CalcBlinkingAlphaBlendNum();
	CalcGaugeParticleAngle();
	ChangeGaugeColor();

	CreateCurrentHealthGaugeScreen();
	CreateWarningGaugeScreen();
	CreateDamageGaugeScreen();
	CreateRecoverGaugeScreen();
	CreateHealthGaugeScreen();
	CreateResultScreen();

	m_prev_health						= m_health->GetCurrentValue();
	m_prev_max_health					= m_health->GetCurrentMaxValue();
	m_prev_health_gauge_actual_percent	= m_current_health_gauge_actual_percent;
	m_prev_health_gauge_virtual_percent = m_current_health_gauge_virtual_percent;
}


#pragma region スクリーン生成
void HealthGauge::CreateCurrentHealthGaugeScreen()
{
	m_current_health_gauge_screen->UseScreen();

	DrawCircleGauge(
		kCenterPos.x, kCenterPos.y,
		static_cast<double>(m_current_health_gauge_virtual_percent),
		m_basis_circle_screen->GetScreenHandle(), 0.0, 1.0);

	m_current_health_gauge_screen->UnuseScreen();
}

void HealthGauge::CreateWarningGaugeScreen()
{
	// TODO : のちに最適化
	m_warning_gauge_screen = std::make_shared<ScreenCreator>(kScreenSize, kCenterPos);

	m_warning_gauge_screen->UseScreen();
	
	DrawCircleGauge(
		kCenterPos.x, kCenterPos.y,
		static_cast<double>(m_current_max_health_gauge_virtual_percent + 3.5f),
		m_warning_circle_screen->GetScreenHandle(), -3.5, 1.0);

	m_warning_gauge_screen->UnuseScreen();
	
	GraphFilter(m_warning_gauge_screen->GetScreenHandle(), DX_GRAPH_FILTER_GAUSS, 16, 1700);
}

void HealthGauge::CreateDamageGaugeScreen()
{
	m_damage_gauge_screen->UseScreen();

	DrawCircleGauge(
		kCenterPos.x, kCenterPos.y,
		static_cast<double>(m_damage_gauge_percent),
		m_basis_circle_screen->GetScreenHandle(), 0.0, 1.0);

	m_damage_gauge_screen->UnuseScreen();
}

void HealthGauge::CreateRecoverGaugeScreen()
{
	m_recover_gauge_screen->UseScreen();

	DrawCircleGauge(
		kCenterPos.x, kCenterPos.y,
		static_cast<double>(m_recover_gauge_percent),
		m_basis_circle_screen->GetScreenHandle(), 0.0, 1.0);

	m_recover_gauge_screen->UnuseScreen();
}

void HealthGauge::CreateHealthGaugeScreen()
{
	m_gauge_screen->UseScreen();

	// 警告ゲージ
	m_warning_gauge_screen->GetGraphicer()->SetBlendNum(m_blinking_alpha_blend_num);
	m_warning_gauge_screen->Draw();

	// 最大HPフレーム
	m_mask_creator->CreateMask();
	m_mask_creator->UseMask(m_max_health_frame_gauge_screen->GetScreenHandle(), true);
	m_frame_particle_graphic->Draw();
	m_mask_creator->UnuseMask();
	m_mask_creator->DeleteMask();

	// 現在の最大HPフレーム
	DrawCircleGauge(
		kCenterPos.x, kCenterPos.y,
		static_cast<double>(m_current_max_health_gauge_virtual_percent + 0.2f),
		m_current_max_health_frame_circle_screen->GetScreenHandle(), -0.2, 1.0);

	// 現在の最大HPゲージ
	DrawCircleGauge(
		kCenterPos.x, kCenterPos.y,
		static_cast<double>(m_current_max_health_gauge_virtual_percent),
		m_basis_circle_screen->GetScreenHandle(), 0.0, 1.0);

	// 回復した際のゲージ
	m_mask_creator->CreateMask();
	m_mask_creator->UseMask(m_recover_gauge_screen->GetScreenHandle(), true);
	DrawBox(
		static_cast<int>(kCenterPos.x - kScreenSize.x * 0.5f), 
		static_cast<int>(kCenterPos.y - kScreenSize.y * 0.5f), 
		static_cast<int>(kCenterPos.x + kScreenSize.x * 0.5f), 
		static_cast<int>(kCenterPos.y + kScreenSize.y * 0.5f), 
		0x48f075, TRUE);
	m_gauge_particle_graphic->Draw();
	m_mask_creator->UnuseMask();

	// ダメージを受けた際のゲージ
	m_mask_creator->UseMask(m_damage_gauge_screen->GetScreenHandle(), true);
	DrawBox(
		static_cast<int>(kCenterPos.x - kScreenSize.x * 0.5f),
		static_cast<int>(kCenterPos.y - kScreenSize.y * 0.5f),
		static_cast<int>(kCenterPos.x + kScreenSize.x * 0.5f),
		static_cast<int>(kCenterPos.y + kScreenSize.y * 0.5f),
		0xf04848, TRUE);
	m_damage_particle_graphic->Draw();
	m_mask_creator->UnuseMask();
	m_mask_creator->DeleteMask();

	m_gauge_screen->UnuseScreen();
}

void HealthGauge::CreateResultScreen()
{
	m_result_screen->UseScreen();

	// 警告サークル
	m_warning_circle_graphic->Draw();

	// 各ゲージ
	m_gauge_screen->Draw();

	// ゲージ画像を現在のHP分だけ描画
	m_mask_creator->CreateMask();
	m_mask_creator->UseMask(m_current_health_gauge_screen->GetScreenHandle(), true);
	m_current_health_mask_screen->Draw();
	m_gauge_particle_graphic	->Draw();
	m_mask_creator->UnuseMask();
	m_mask_creator->DeleteMask();

	m_result_screen->UnuseScreen();
}
#pragma endregion


void HealthGauge::CalcGaugePercent()
{
	const auto delta_time = GameTimeManager::GetInstance()->GetDeltaTime(TimeScaleLayerKind::kUI);
	const auto max_health = m_health->GetMaxValue();

	// 実際の値を取得
	m_current_health_gauge_actual_percent = m_health->GetCurrentValue() / max_health * kMaxGaugePercent;

	if (m_health->GetCurrentValue() <= m_prev_health && !m_is_recover_gauge)
	{
		m_current_health_gauge_virtual_percent = m_current_health_gauge_actual_percent;
		m_recover_gauge_percent = 0.0f;
	}
	else
	{
		m_is_recover_gauge = true;

		// 現在のHPのゲージ率を、徐々に回復ゲージ率に近づける
		m_recover_gauge_percent = m_current_health_gauge_actual_percent;
		math::Increase(m_current_health_gauge_virtual_percent, kGaugeIncreaseSpeed * delta_time, m_recover_gauge_percent, false);
		
		if (m_recover_gauge_percent == m_current_health_gauge_virtual_percent)
		{
			m_is_recover_gauge = false;
		}

		// ゲージ回復中にダメージを受けた場合、強制的に回復演出を終了
		if (m_health->GetCurrentValue() < m_prev_health)
		{
			m_current_health_gauge_virtual_percent	= m_current_health_gauge_actual_percent;
			m_recover_gauge_percent					= 0.0f;
			m_damage_gauge_percent					= m_prev_health_gauge_actual_percent;
			m_is_recover_gauge						= false;
		}
	}

	// 最大HPのゲージ率
	m_current_max_health_gauge_virtual_percent	= m_health->GetCurrentMaxValue() / max_health * kMaxGaugePercent;

	// ダメージを受けた際の減少ゲージ率
	if (m_is_recover_gauge)
	{
		m_damage_gauge_percent = m_current_health_gauge_virtual_percent;
	}
	else
	{
		math::Decrease(m_damage_gauge_percent, kGaugeIncreaseSpeed * delta_time, m_current_health_gauge_virtual_percent);
	}
}

void HealthGauge::CalcBlinkingAlphaBlendNum()
{
	const auto delta_time = GameTimeManager::GetInstance()->GetDeltaTime(TimeScaleLayerKind::kUI);
	auto is_loop = false;

	// HPが12.5%未満の場合、常に点滅させる
	if (m_current_health_gauge_actual_percent < 12.5f)
	{
		if (m_prev_health_gauge_actual_percent >= 12.5f)
		{
			m_blinking_sin = 0.0f;
		}

		is_loop = true;
	}
	// ダメージを受けた場合、sinを0にして点滅を再開させる
	else if (m_health->GetCurrentValue() < m_prev_health)
	{
		m_blinking_sin = 0.0f;
	}
	else if (m_health->GetCurrentValue() > m_prev_health)
	{
		m_blinking_sin = DX_PI_F;
	}

	math::Increase(m_blinking_sin, kBlinkingSpeed * delta_time, DX_PI_F, is_loop);
	m_blinking_alpha_blend_num = (sin(m_blinking_sin) * 0.5f + 0.5f) * UCHAR_MAX;

	// 警告サークルは形状が不変であるため即座に適用
	// HPが12.5%以下の場合のみ警告点滅を描画
	if (m_current_health_gauge_actual_percent < 12.5f)
	{
		m_warning_circle_graphic->SetBlendNum(m_blinking_alpha_blend_num);
	}
	else
	{
		m_warning_circle_graphic->SetBlendNum(0);
	}
}

void HealthGauge::CalcGaugeParticleAngle()
{
	const auto delta_time = GameTimeManager::GetInstance()->GetDeltaTime(TimeScaleLayerKind::kUI);
	
	m_gauge_particle_angle += 10.0f * math::kDegToRad * delta_time;
	m_gauge_particle_graphic	->SetAngle( m_gauge_particle_angle);
	m_damage_particle_graphic	->SetAngle(-m_gauge_particle_angle);
	m_frame_particle_graphic	->SetAngle( m_gauge_particle_angle);
}

void HealthGauge::ChangeGaugeColor()
{
	if (m_prev_health == m_health->GetCurrentValue() && !m_is_recover_gauge) { return; }

	// 真円を100%とた時、12.5%未満の場合
	if (m_current_health_gauge_virtual_percent < 12.5f)
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
	else if (m_current_health_gauge_virtual_percent < 37.5f)
	{
		m_gauge_hue = static_cast<int>(math::ConvertValueNewRange<float, float>(12.5f, 37.5f, kMiddleHue, kMaxHue, m_current_health_gauge_virtual_percent));
	}
	// 37.5%以上の場合
	else
	{
		if (m_prev_gauge_hue == kMaxHue) { return; }
		m_gauge_hue = kMaxHue;
	}

	// DxLib-GraphFilterの色相変更は不可逆的であり現在の色相値を0として変更値が適用されるため、
	// 色の変換が行われるたびにスクリーンを再構築する
	m_current_health_mask_screen = std::make_shared<ScreenCreator>(m_current_health_gauge_graphic->GetOriginSize(), kCenterPos);

	// スクリーン生成
	m_current_health_mask_screen->UseScreen();
	m_current_health_gauge_graphic->Draw();
	m_current_health_mask_screen->UnuseScreen();
	GraphFilter(m_current_health_mask_screen->GetScreenHandle(), DX_GRAPH_FILTER_HSB, 0, m_gauge_hue, 0, 0);

	m_prev_gauge_hue = m_gauge_hue;
}
