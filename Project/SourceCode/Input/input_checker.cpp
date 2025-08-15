#include "input_checker.hpp"

InputChecker::InputChecker():
	m_csv					(std::make_unique<CSV>()),
	m_xinput				(-1),
	m_current_device		(DeviceKind::kKeyboard),
	m_is_lock_mouse_pos		(true)
{	
	SetUseDirectInputFlag(TRUE);
	SetMouseDispFlag(FALSE);

	m_mouse_data[TimeKind::kPrev].pos	    = m_mouse_data[TimeKind::kCurrent].pos		= Window::kCenterPos;
	m_mouse_data[TimeKind::kPrev].dir		= m_mouse_data[TimeKind::kCurrent].dir		= Vector2D<float>(0.0f, 0.0f);
	m_mouse_data[TimeKind::kPrev].velocity  = m_mouse_data[TimeKind::kCurrent].velocity = Vector2D<float>(0.0f, 0.0f);

	LockCursor();

	m_key_code = m_csv->Read1DCSV<std::vector<int>>("Data/CSV/key_number.csv", false);
	for (auto& code : m_key_code)
	{
		m_input_data.emplace_back(InputCode(InputKind::kKey, code), TimeKind::kPrev,	 InputData());
		m_input_data.emplace_back(InputCode(InputKind::kKey, code), TimeKind::kCurrent, InputData());
	}
	AddInputData(InputKind::kMouseButton, kMouseButtonNum);
	AddInputData(InputKind::kMouseWheel,  kMouseWheelNum);
	AddInputData(InputKind::kMouseSlide,  kMouseSlideNum);
	AddInputData(InputKind::kPadButton,   kPadButtonNum);
	AddInputData(InputKind::kPadTrigger,  kPadTriggerNum);
	AddInputData(InputKind::kPadStick,    kPadStickNum);
}

InputChecker::~InputChecker()
{
	// 処理なし
}

void InputChecker::Update()
{
	ShiftDataCureentToPrev();

	UpdateMouse();

	GetJoypadXInputState(DX_INPUT_PAD1, &m_xinput);

	CheckInputAll();
	CountInputTimeAll();

	DetectCurrentInputDevice();
}

void InputChecker::LateUpdate()
{
	LockCursor();
}

void InputChecker::UpdateMouse()
{
	GetMousePoint(&m_mouse_data.at(TimeKind::kCurrent).pos.x, &m_mouse_data.at(TimeKind::kCurrent).pos.y);
	m_mouse_data.at(TimeKind::kCurrent).wheel_rotation = GetMouseWheelRotVol();
	CalcMouseVelocity();
	CalcMouseDir();
}

void InputChecker::LockCursor()
{
	if (!m_is_lock_mouse_pos) { return; }

	m_mouse_data.at(TimeKind::kPrev).pos = m_mouse_data.at(TimeKind::kCurrent).pos = Window::kCenterPos;
	SetMousePoint(m_mouse_data.at(TimeKind::kCurrent).pos.x, m_mouse_data.at(TimeKind::kCurrent).pos.y);
}

bool InputChecker::IsInput(const InputCode& input_code)
{
	switch (input_code.kind)
	{
	case InputKind::kKey:
		return CheckHitKey(static_cast<int>(input_code.code));
		break;

	case InputKind::kMouseButton:
		return (GetMouseInput() & static_cast<int>(input_code.code)) != 0;
		break;

	case InputKind::kMouseWheel:
		return GetInputParameter(static_cast<mouse::WheelKind>(input_code.code));
		break;

	case InputKind::kMouseSlide:
		switch (static_cast<mouse::SlideDirKind>(input_code.code))
		{
		case mouse::SlideDirKind::kLeft:  if (m_mouse_data.at(TimeKind::kCurrent).pos.x < m_mouse_data.at(TimeKind::kPrev).pos.x) { return true; } break;
		case mouse::SlideDirKind::kRight: if (m_mouse_data.at(TimeKind::kCurrent).pos.x > m_mouse_data.at(TimeKind::kPrev).pos.x) { return true; } break;
		case mouse::SlideDirKind::kDown:  if (m_mouse_data.at(TimeKind::kCurrent).pos.y > m_mouse_data.at(TimeKind::kPrev).pos.y) { return true; } break;
		case mouse::SlideDirKind::kUp:    if (m_mouse_data.at(TimeKind::kCurrent).pos.y < m_mouse_data.at(TimeKind::kPrev).pos.y) { return true; } break;
		}
		break;

	case InputKind::kPadButton:
		return m_xinput.Buttons[static_cast<int>(input_code.code)];
		break;

	case InputKind::kPadTrigger:
		return GetInputParameter(static_cast<pad::TriggerKind>(input_code.code));
		break;

	case InputKind::kPadStick:
		return GetInputParameter(static_cast<pad::StickKind>(input_code.code));
		break;
	}
	return false;
}

int InputChecker::GetInputParameter(const InputCode& input_code) const
{
	int rota = 0;

	switch (input_code.kind)
	{
	case InputKind::kMouseWheel:
		rota = m_mouse_data.at(TimeKind::kCurrent).wheel_rotation;
		switch (static_cast<mouse::WheelKind>(input_code.code))
		{
		case mouse::WheelKind::kUp:		return rota > 0 ? rota : 0;	break;
		case mouse::WheelKind::kDown:	return rota < 0 ? rota : 0;	break;
		}
		break;

	case InputKind::kPadTrigger:
		switch (static_cast<pad::TriggerKind>(input_code.code))
		{
		case pad::TriggerKind::kLT: if (m_xinput.LeftTrigger  > kTriggerDeadZone)	{ return m_xinput.LeftTrigger ; } break;
		case pad::TriggerKind::kRT: if (m_xinput.RightTrigger > kTriggerDeadZone)	{ return m_xinput.RightTrigger; } break;
		}
		break;

	case InputKind::kPadStick:
		switch (static_cast<pad::StickKind>(input_code.code))
		{
		case pad::StickKind::kLSLeft:  if (m_xinput.ThumbLX < -kStickDeadZone) { return m_xinput.ThumbLX; } break;
		case pad::StickKind::kLSRight: if (m_xinput.ThumbLX >  kStickDeadZone) { return m_xinput.ThumbLX; } break;
		case pad::StickKind::kLSDown:  if (m_xinput.ThumbLY < -kStickDeadZone) { return m_xinput.ThumbLY; } break;
		case pad::StickKind::kLSUp:    if (m_xinput.ThumbLY >  kStickDeadZone) { return m_xinput.ThumbLY; } break;
		case pad::StickKind::kRSLeft:  if (m_xinput.ThumbRX < -kStickDeadZone) { return m_xinput.ThumbRX; } break;
		case pad::StickKind::kRSRight: if (m_xinput.ThumbRX >  kStickDeadZone) { return m_xinput.ThumbRX; } break;
		case pad::StickKind::kRSDown:  if (m_xinput.ThumbRY < -kStickDeadZone) { return m_xinput.ThumbRY; } break;
		case pad::StickKind::kRSUp:	   if (m_xinput.ThumbRY >  kStickDeadZone) { return m_xinput.ThumbRY; } break;
		}
		break;

	default:
		break;
	}
	return 0;
}

float InputChecker::GetInputTime(const InputCode& input_code, const TimeKind time_kind)
{
	for (const auto& [input_c, state_t, data] : m_input_data)
	{
		if (input_c.kind == input_code.kind && input_c.code == input_code.code && state_t == time_kind)
		{
			return data.input_time;
		}
	}
	return 0.0f;
}

InputState InputChecker::GetInputState(const InputCode& input_code)
{
	bool prev_is_input	  = false;
	bool current_is_input = false;

	for (const auto& [input_c, state_t, data] : m_input_data)
	{
		if (input_c.kind == input_code.kind && input_c.code == input_code.code)
		{
			switch (state_t)
			{
			case TimeKind::kPrev:		prev_is_input	 = data.is_input;	break;
			case TimeKind::kCurrent:	current_is_input = data.is_input;	break;
			}
		}
	}

	if (current_is_input)
	{
		return prev_is_input ? InputState::kHold : InputState::kSingle;
	}
	return prev_is_input ? InputState::kPrev : InputState::kNone;
}

void InputChecker::AddInputData(const InputKind kind, const int input_code_num)
{
	for (int i = 0; i < input_code_num; ++i)
	{
		// 中身は空で追加
		m_input_data.emplace_back(InputCode(kind, i), TimeKind::kPrev,		InputData());
		m_input_data.emplace_back(InputCode(kind, i), TimeKind::kCurrent,	InputData());
	}
}

void InputChecker::CalcMouseDir()
{
	m_mouse_data.at(TimeKind::kCurrent).dir = v2d::GetNormalizedV(m_mouse_data.at(TimeKind::kCurrent).velocity);
}

void InputChecker::CalcMouseVelocity()
{
	const Vector2D<int> distance = m_mouse_data.at(TimeKind::kCurrent).pos - m_mouse_data.at(TimeKind::kPrev).pos;
	m_mouse_data.at(TimeKind::kCurrent).velocity = v2d::ConvertVecType<float>(distance);
}

void InputChecker::CountInputTimeAll()
{
	for (auto& [input_c, state_t, data] : m_input_data)
	{
		// 現在の入力時間のみ変更
		if (state_t == TimeKind::kPrev) { continue; }

		if (data.is_input)
		{
			const auto time_manager = GameTimeManager::GetInstance();
			data.input_time += time_manager->GetDeltaTime(TimeScaleController::LayerKind::kNoneScale);
			continue;
		}
		data.input_time = 0.0f;
	}
}

void InputChecker::CheckInputAll()
{
	for (auto& [input_c, state_t, data] : m_input_data)
	{
		// 現在の入力のみ判定
		if (state_t == TimeKind::kPrev) { continue; }

		switch (input_c.kind)
		{
		case InputKind::kKey:
			data.is_input = IsInput(input_c.code)										? true : false;
			break;

		case InputKind::kMouseButton:
			data.is_input = IsInput(static_cast<mouse::ButtonKind>(input_c.code))		? true : false;
			break;

		case InputKind::kMouseWheel:
			data.is_input = IsInput(static_cast<mouse::WheelKind>(input_c.code))		? true : false;
			break;

		case InputKind::kMouseSlide:
			data.is_input = IsInput(static_cast<mouse::SlideDirKind>(input_c.code))		? true : false;
			break;

		case InputKind::kPadButton:
			data.is_input = IsInput(static_cast<pad::ButtonKind>(input_c.code))			? true : false;
			break;

		case InputKind::kPadTrigger:
			data.is_input = IsInput(static_cast<pad::TriggerKind>(input_c.code))		? true : false;
			break;

		case InputKind::kPadStick:
			data.is_input = IsInput(static_cast<pad::StickKind>(input_c.code))			? true : false;
			break;
		}
	}
}

void InputChecker::ShiftDataCureentToPrev()
{
	// 入力情報のシフト
	m_mouse_data.at(TimeKind::kPrev) = m_mouse_data.at(TimeKind::kCurrent);

	for (const auto& [current_input_c, current_state_t, current_data] : m_input_data)
	{
		// 1フレーム前の情報であった場合はスキップ
		if (current_state_t == TimeKind::kPrev) { continue; }

		for (auto& [prev_input_c, prev_state_t, prev_data] : m_input_data)
		{
			// 現在のフレームの情報であった場合はスキップ
			if (prev_state_t == TimeKind::kCurrent) { continue; }

			if (prev_input_c.kind == current_input_c.kind && prev_input_c.code == current_input_c.code)
			{
				prev_data = current_data;
			}
		}
	}
}

void InputChecker::DetectCurrentInputDevice()
{
	for (const auto& [input_c, state_t, data] : m_input_data)
	{
		if (state_t == TimeKind::kPrev) { continue; }
		if (!data.is_input)				 { continue; }

		// マウスをスライドさせただけでは入力デバイスに影響を与えない
		if (input_c.kind == InputKind::kKey || input_c.kind == InputKind::kMouseButton || input_c.kind == InputKind::kMouseWheel)
		{
			m_current_device = DeviceKind::kKeyboard;
			return;
		}
		if (input_c.kind == InputKind::kPadButton || input_c.kind == InputKind::kPadTrigger || input_c.kind == InputKind::kPadStick)
		{
			m_current_device = DeviceKind::kPad;
			return;
		}
	}
}
