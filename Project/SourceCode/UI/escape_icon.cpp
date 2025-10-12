#include "escape_icon.hpp"
#include "../Command/command_handler.hpp"

EscapeIcon::EscapeIcon(std::shared_ptr<IGrabbable> grab_target) : 
	m_grab_target				(grab_target),
	m_basis_circle_screen		(std::make_shared<ScreenCreator>(kScreenSize)),
	m_escape_circle_screen		(std::make_shared<ScreenCreator>(kScreenSize)),
	m_button_graphic_resource	(std::make_shared<ButtonGraphicGetter>()),
	m_button_icon_graphic		(nullptr),
	m_escape_gauge_percent		(0.0),
	m_icon_scale				(1.0f),
	m_scale_sin					(0.0f)
{
	m_basis_circle_screen->UseScreen();
	DrawCircle(kScreenCenterPos.x, kScreenCenterPos.y, kGaugeRadius, kBaseGaugeColor, TRUE, kGaugeThickness);
	m_basis_circle_screen->UnuseScreen();

	m_escape_circle_screen->UseScreen();
	DrawCircle(kScreenCenterPos.x, kScreenCenterPos.y, kGaugeRadius, kEscapeGaugeColor, TRUE, kGaugeThickness);
	m_escape_circle_screen->UnuseScreen();
}

EscapeIcon::~EscapeIcon()
{

}

void EscapeIcon::LateUpdate()
{
	if (!m_grab_target->IsGrabbed()) { return; }

	CalcGaugePercent();
	CalcIconScale();
	SetIconGraphic();
}

void EscapeIcon::Draw() const
{
	if (!m_grab_target->IsGrabbed()) { return; }

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
	const auto delta_time = GameTimeManager::GetInstance()->GetDeltaTime(TimeScaleLayerKind::kUI);

	const auto input_mode = CommandHandler::GetInstance()->GetInputModeKind(CommandKind::kEscape);
	switch (input_mode)
	{
	case InputModeKind::kSingle:
		math::Increase(m_scale_sin, 10.0f * delta_time, DX_PI_F * 2, true);
		// sinの結果を 1.0〜0.4 の範囲に変換
		m_icon_scale = (sin(m_scale_sin) * 0.3f) + 0.7f;
		break;

	case InputModeKind::kHold:
		m_icon_scale = 1.0f;
		break;
	}
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
