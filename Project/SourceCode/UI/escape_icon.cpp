#include "escape_icon.hpp"
#include "../Command/command_handler.hpp"

EscapeIcon::EscapeIcon(std::shared_ptr<IGrabbable> grab_target) : 
	m_grab_target				(grab_target),
	m_basis_circle_screen		(std::make_shared<ScreenCreator>(kScreenSize)),
	m_escape_circle_screen		(std::make_shared<ScreenCreator>(kScreenSize)),
	m_button_graphic_resource	(std::make_shared<ButtonGraphicGetter>()),
	m_button_icon_graphic		(nullptr),
	m_blur_circle_graphic		(std::make_shared<Graphicer>(UIGraphicPath.CIRCLE_BLUR_WHITE)),
	m_escape_gauge_percent		(0.0),
	m_icon_scale				(1.0f),
	m_scale_sin					(0.0f),
	m_blur_circle_alpha_num		(255),
	m_blur_circle_scale			(0.0f),
	m_delete_wait_timer			(1.0f)
{
	m_basis_circle_screen->UseScreen();
	DrawCircle(kScreenCenterPos.x, kScreenCenterPos.y, kGaugeRadius, kBaseGaugeColor, TRUE, kGaugeThickness);
	m_basis_circle_screen->UnuseScreen();

	m_escape_circle_screen->UseScreen();
	DrawCircle(kScreenCenterPos.x, kScreenCenterPos.y, kGaugeRadius, kEscapeGaugeColor, TRUE, kGaugeThickness);
	m_escape_circle_screen->UnuseScreen();

	m_blur_circle_graphic->SetCenterPos(Window::kCenterPos);
	
}

EscapeIcon::~EscapeIcon()
{

}

void EscapeIcon::LateUpdate()
{
	CalcGaugePercent();

	if (!m_grab_target->CanEscape())
	{
		const auto delta_time = GameTimeManager::GetInstance()->GetDeltaTime(TimeScaleLayerKind::kUI);
		m_delete_wait_timer += delta_time;
		return;
	}

	CalcIconScale();
	CalcBlurCircleParameter();
	SetIconGraphic();

	m_delete_wait_timer = 0.0f;
}

void EscapeIcon::Draw() const
{
	if (!m_grab_target->CanEscape() && m_delete_wait_timer > 0.4f) { return; }

	// サークル
	m_blur_circle_graphic->Draw();

	// 基礎ゲージ
	DrawCircleGauge(Window::kCenterPos.x, Window::kCenterPos.y, 100, m_basis_circle_screen->GetScreenHandle(), 0.0, 1.0);

	// 脱出ゲージ
	DrawCircleGauge(Window::kCenterPos.x, Window::kCenterPos.y, m_escape_gauge_percent, m_escape_circle_screen->GetScreenHandle(), 0.0, 1.0);

	// ボタンアイコン
	m_button_icon_graphic->Draw();
}

void EscapeIcon::CalcGaugePercent()
{
	const auto escape_gauge = m_grab_target->GetEscapeGauge();
	m_escape_gauge_percent = escape_gauge->GetCurrentValue() / escape_gauge->GetMaxValue() * 100;
}

void EscapeIcon::CalcIconScale()
{
	const auto	delta_time	= GameTimeManager::GetInstance()->GetDeltaTime(TimeScaleLayerKind::kUI);
	const auto	input_mode	= CommandHandler ::GetInstance()->GetInputModeKind(CommandKind::kEscape);
	auto		sin_num		= 0.0f;
	auto		rate		= 0.0f;

	switch (input_mode)
	{
	case InputModeKind::kSingle:
		math::Increase(m_scale_sin, 20.0f * delta_time, DX_TWO_PI_F, true);

		sin_num = sin(m_scale_sin);
		rate = (sin_num * 0.5f + 0.5f);

		// y = sinθのグラフ上でyが減少に向かっている場合、変化量を緩やかにする
		// powを使用するのは戻り値を0～1に保てるからである
		if (m_scale_sin >= DX_PI_F && m_scale_sin < 3 / 2 * DX_PI_F)
		{
			rate *= pow(rate, 2.0f);
		}
		else
		{
			rate = pow(rate, 0.4f);
		}

		// 1.0～0.4の範囲にスケーリング
		m_icon_scale = rate * 0.6f + 0.4f;
		break;

	case InputModeKind::kHold:
		m_icon_scale = 1.0f;
		break;
	}
}

void EscapeIcon::CalcBlurCircleParameter()
{
	const auto delta_time = GameTimeManager::GetInstance()->GetDeltaTime(TimeScaleLayerKind::kUI);

	// スケール計算
	math::Increase(m_blur_circle_scale, 2.4f * delta_time, 0.9f, true);
	m_blur_circle_graphic->SetScale(m_blur_circle_scale);

	// スケールに依存してアルファ値を計算
	m_blur_circle_alpha_num = math::ConvertValueNewRange<float, float>(0.0f, 1.0f, 255, 80, m_blur_circle_scale);
	m_blur_circle_graphic->SetAlphaBlendNum(m_blur_circle_alpha_num);
}

void EscapeIcon::SetIconGraphic()
{
	// 入力デバイスおよびキー割り当てに対応した画像を取得
	const auto command = CommandHandler::GetInstance();
	switch (InputChecker::GetInstance()->GetCurrentInputDevice())
	{
	case DeviceKind::kKeyboard:
		m_button_icon_graphic = m_button_graphic_resource->GetWeaponGraphicer(command->GetKeyInputCode(CommandKind::kEscape, CommandSlotKind::kMain));
		break;

	case DeviceKind::kPad:
		m_button_icon_graphic = m_button_graphic_resource->GetWeaponGraphicer(command->GetPadInputCode(CommandKind::kEscape, CommandSlotKind::kStatic1));
		break;
	}

	m_button_icon_graphic->SetCenterPos(Window::kCenterPos);
	m_button_icon_graphic->SetScale(0.22f * m_icon_scale);
}
