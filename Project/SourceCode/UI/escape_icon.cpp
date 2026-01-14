#include "escape_icon.hpp"
#include "../Command/command_handler.hpp"
#include "../Font/text.hpp"

EscapeIcon::EscapeIcon(const std::shared_ptr<IGrabbable>& grab_target) :
	data						(EscapeIconData()),
	m_grab_target				(grab_target),
	m_current_input_mode_kind	(InputModeKind::kSingle),
	m_basis_circle_screen		(nullptr),
	m_escape_circle_screen		(nullptr),
	m_button_graphic_resource	(std::make_shared<ButtonGraphicGetter>()),
	m_button_icon_graphic		(nullptr),
	m_blur_circle_graphic		(nullptr),
	m_escape_gauge_percent		(0.0),
	m_icon_scale				(1.0f),
	m_scale_sin					(0.0f),
	m_blur_circle_alpha_num		(UCHAR_MAX),
	m_blur_circle_scale			(0.0f),
	m_delete_wait_timer			(1.0f)
{
	nlohmann::json j_data;
	if (json_loader::Load("Data/JSON/escape_icon_data.json", j_data))
	{
		data = j_data.at("escape_icon_data").get<EscapeIconData>();
	}

	// カラータイプ変換
	data.base_gauge_u_int_color		= type_converter::ConvertHEXToUINT(data.base_gauge_hex_color);
	data.escape_gauge_u_int_color	= type_converter::ConvertHEXToUINT(data.escape_gauge_hex_color);

	// テキスト構築
	text::CreateText(data.escape_text_data);
	text::CreateText(data.hold_text_data);

	m_screen_center_pos = data.screen_size * 0.5f;

	// スクリーン生成
	m_basis_circle_screen = std::make_shared<ScreenCreator>(data.screen_size);
	m_basis_circle_screen->UseScreen();
	DrawCircle(m_screen_center_pos.x, m_screen_center_pos.y, data.gauge_radius, data.base_gauge_u_int_color, TRUE, data.gauge_thickness);
	m_basis_circle_screen->UnuseScreen();

	m_escape_circle_screen = std::make_shared<ScreenCreator>(data.screen_size);
	m_escape_circle_screen->UseScreen();
	DrawCircle(m_screen_center_pos.x, m_screen_center_pos.y, data.gauge_radius, data.escape_gauge_u_int_color, TRUE, data.gauge_thickness);
	m_escape_circle_screen->UnuseScreen();

	// 画像生成
	m_blur_circle_graphic = std::make_shared<Graphicer>(data.blur_circle_graphic_path);
	m_blur_circle_graphic->SetCenterPos(Window::kCenterPos);
}

EscapeIcon::~EscapeIcon()
{

}

void EscapeIcon::LateUpdate()
{
	CalcGaugePercent();

	if (m_grab_target->CanEscape())
	{
		CalcIconScale();
		CalcBlurCircleParameter();
		SetIconGraphic();

		m_delete_wait_timer = 0.0f;
	}
	else
	{
		const auto delta_time = GameTimeManager::GetInstance()->GetDeltaTime(TimeScaleLayerKind::kUI);
		m_delete_wait_timer += delta_time;

		m_blur_circle_alpha_num = 0;
		m_blur_circle_scale		= 0.9f;
		m_icon_scale			= 1.0f;

		if (m_button_icon_graphic) { m_button_icon_graphic->SetScale(0.22f * m_icon_scale); }
	}
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

	// 脱出テキスト
	DrawStringToHandle(
		static_cast<int>((Window::kScreenSize.x - data.escape_text_data.size.x) * 0.5f) + data.escape_text_offset.x,
		static_cast<int>((Window::kScreenSize.y - data.escape_text_data.size.y) * 0.5f) + data.escape_text_offset.y,
		data.escape_text_data.text.c_str(), data.escape_text_data.u_int_color, data.escape_text_data.font_handle);

	// HOLDテキスト
	if (m_current_input_mode_kind == InputModeKind::kHold)
	{
		DrawStringToHandle(
			static_cast<int>((Window::kScreenSize.x - data.hold_text_data.size.x) * 0.5f) + data.hold_text_offset.x,
			static_cast<int>((Window::kScreenSize.y - data.hold_text_data.size.y) * 0.5f) + data.hold_text_offset.y,
			data.hold_text_data.text.c_str(), data.hold_text_data.u_int_color, data.hold_text_data.font_handle);
	}
}

void EscapeIcon::CalcGaugePercent()
{
	const auto escape_gauge = m_grab_target->GetEscapeGauge();
	m_escape_gauge_percent = escape_gauge->GetCurrentValue() / escape_gauge->GetMaxValue() * 100;
}

void EscapeIcon::CalcIconScale()
{
	m_current_input_mode_kind	= CommandHandler ::GetInstance()->GetInputModeKind(CommandKind::kEscape);
	const auto	delta_time		= GameTimeManager::GetInstance()->GetDeltaTime(TimeScaleLayerKind::kUI);
	auto		sin_num			= 0.0f;
	auto		rate			= 0.0f;

	switch (m_current_input_mode_kind)
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
	m_blur_circle_alpha_num = math::ConvertValueNewRange<float, float>(0.0f, 1.0f, UCHAR_MAX, 80, m_blur_circle_scale);
	m_blur_circle_graphic->SetBlendNum(m_blur_circle_alpha_num);
}

void EscapeIcon::SetIconGraphic()
{
	// 入力デバイスおよびキー割り当てに対応した画像を取得
	const auto command	= CommandHandler::GetInstance();
	const auto device	= InputChecker::GetInstance()->GetCurrentInputDevice();

	switch (device)
	{
	case DeviceKind::kKeyboard:
		m_button_icon_graphic = m_button_graphic_resource->GetButtonGraphicer(command->GetInputCode(device, CommandKind::kEscape, CommandSlotKind::kMain));
		break;

	case DeviceKind::kPad:
		m_button_icon_graphic = m_button_graphic_resource->GetButtonGraphicer(command->GetInputCode(device, CommandKind::kEscape, CommandSlotKind::kStatic1));
		break;
	}

	m_button_icon_graphic->SetCenterPos(Window::kCenterPos);
	m_button_icon_graphic->SetScale(0.22f * m_icon_scale);
}
