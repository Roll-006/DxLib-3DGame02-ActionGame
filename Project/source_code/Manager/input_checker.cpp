#include "input_checker.hpp"

InputChecker* InputChecker::m_instance = nullptr;

void InputChecker::Generate()
{
	if (!m_instance)
	{
		m_instance = new InputChecker;
	}
}

void InputChecker::Delete()
{
	delete m_instance;
	m_instance = nullptr;
}

InputChecker* InputChecker::GetInstance()
{
	return m_instance ? m_instance : nullptr;
}

InputChecker::InputChecker():
	m_csv					(std::make_unique<CSV>()),
	m_xinput				(-1),
	m_current_device		(DeviceKind::kKeyboard),
	m_is_active_mouse_cursor(true),
	m_is_init_mouse_pos		(false)
{	
	SetUseDirectInputFlag(TRUE);

	m_mouse_data[TimeState::kPrev].pos	     = m_mouse_data[TimeState::kCurrent].pos	   = Vector2D<int>(Window::kHalfWidth, Window::kHalfHeight);
	m_mouse_data[TimeState::kPrev].direction = m_mouse_data[TimeState::kCurrent].direction = Vector2D<float>(0.0f, 0.0f);
	m_mouse_data[TimeState::kPrev].velocity  = m_mouse_data[TimeState::kCurrent].velocity  = Vector2D<float>(0.0f, 0.0f);

	InitMouseCursor();
	InitMousePos();

	m_key_number = m_csv->Read1DCSV<std::vector<int>>("../data/csv/key_number.csv", false);
	for (auto& key_kind : m_key_number)
	{
		m_input_data.emplace_back(InputKind::kKey, key_kind, TimeState::kPrev,	  InputData());
		m_input_data.emplace_back(InputKind::kKey, key_kind, TimeState::kCurrent, InputData());
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

	GetMousePoint(&m_mouse_data.at(TimeState::kCurrent).pos.x, &m_mouse_data.at(TimeState::kCurrent).pos.y);
	m_mouse_data.at(TimeState::kCurrent).wheel_rotation = GetMouseWheelRotVol();
	if (m_is_init_mouse_pos) { InitMousePos(); }
	CalcMouseVelocity();
	CalcMouseDirection();

	GetJoypadXInputState(DX_INPUT_PAD1, &m_xinput);

	CheckInputAll();
	CountInputTimeAll();

	DetectCurrentInputDevice();
}

void InputChecker::InitMousePos()
{
	m_mouse_data.at(TimeState::kPrev).pos	 = Vector2D<int>(Window::kHalfWidth, Window::kHalfHeight);
	m_mouse_data.at(TimeState::kCurrent).pos = Vector2D<int>(Window::kHalfWidth, Window::kHalfHeight);

	SetMousePoint(m_mouse_data.at(TimeState::kCurrent).pos.x, m_mouse_data.at(TimeState::kCurrent).pos.y);
}

void InputChecker::InitMouseCursor()const
{
	SetMouseDispFlag(m_is_active_mouse_cursor);
}

void InputChecker::ActivateMouseCursor()
{
	m_is_active_mouse_cursor = true;
	InitMouseCursor();
}

void InputChecker::DeactivateMouseCursor()
{
	m_is_active_mouse_cursor = false;
	InitMouseCursor();
}

bool InputChecker::IsInputMouseButton(const int input_num)const
{
	return (GetMouseInput() & input_num) != 0;
}

int InputChecker::GetMouseWheelParameter(MouseWheelKind input_num)const
{
	int rota = m_mouse_data.at(TimeState::kCurrent).wheel_rotation;

	switch (input_num)
	{
	case MouseWheelKind::kUp:	return rota > 0 ? rota : 0;	break;
	case MouseWheelKind::kDown:	return rota < 0 ? rota : 0;	break;
	}
	return 0;
}

bool InputChecker::IsSlideMouse(MouseSlideDirectionKind input_num)const
{
	switch (input_num)
	{
	case MouseSlideDirectionKind::kLeft:  if (m_mouse_data.at(TimeState::kCurrent).pos.x < m_mouse_data.at(TimeState::kPrev).pos.x) { return true; } break;
	case MouseSlideDirectionKind::kRight: if (m_mouse_data.at(TimeState::kCurrent).pos.x > m_mouse_data.at(TimeState::kPrev).pos.x) { return true; } break;
	case MouseSlideDirectionKind::kDown:  if (m_mouse_data.at(TimeState::kCurrent).pos.y > m_mouse_data.at(TimeState::kPrev).pos.y) { return true; } break;
	case MouseSlideDirectionKind::kUp:    if (m_mouse_data.at(TimeState::kCurrent).pos.y < m_mouse_data.at(TimeState::kPrev).pos.y) { return true; } break;
	}
	return false;
}

int InputChecker::GetPadStickParameter(StickKind input_num)const
{
	switch (input_num)
	{
	case StickKind::kLSLeft:  if (m_xinput.ThumbLX < -kStickDeadZone) { return m_xinput.ThumbLX; } break;
	case StickKind::kLSRight: if (m_xinput.ThumbLX >  kStickDeadZone) { return m_xinput.ThumbLX; } break;
	case StickKind::kLSDown:  if (m_xinput.ThumbLY < -kStickDeadZone) { return m_xinput.ThumbLY; } break;
	case StickKind::kLSUp:    if (m_xinput.ThumbLY >  kStickDeadZone) { return m_xinput.ThumbLY; } break;
	case StickKind::kRSLeft:  if (m_xinput.ThumbRX < -kStickDeadZone) { return m_xinput.ThumbRX; } break;
	case StickKind::kRSRight: if (m_xinput.ThumbRX >  kStickDeadZone) { return m_xinput.ThumbRX; } break;
	case StickKind::kRSDown:  if (m_xinput.ThumbRY < -kStickDeadZone) { return m_xinput.ThumbRY; } break;
	case StickKind::kRSUp:	  if (m_xinput.ThumbRY >  kStickDeadZone) { return m_xinput.ThumbRY; } break;
	}
	return 0;
}

int InputChecker::GetPadTriggerParameter(TriggerKind input_num)const
{
	switch (input_num)
	{
	case TriggerKind::kLT: if (m_xinput.LeftTrigger  > kTriggerDeadZone) { return m_xinput.LeftTrigger; }  break;
	case TriggerKind::kRT: if (m_xinput.RightTrigger > kTriggerDeadZone) { return m_xinput.RightTrigger; } break;
	}
	return 0;
}

float InputChecker::GetKeyInputTime(const int input_num, TimeState time_state)
{
	return SupportGetInputTime(InputKind::kKey, input_num, time_state);
}

InputState InputChecker::GetKeyInputState(const int input_num)
{
	return SupportGetInputState(InputKind::kKey, input_num);
}

void InputChecker::AddInputData(InputKind kind, const int input_num)
{
	for (int i = 0; i < input_num; ++i)
	{
		m_input_data.emplace_back(kind, i, TimeState::kPrev,	InputData());
		m_input_data.emplace_back(kind, i, TimeState::kCurrent, InputData());
	}
}

void InputChecker::CalcMouseDirection()
{
	m_mouse_data.at(TimeState::kCurrent).direction = v2d::GetNormalizedVector(m_mouse_data.at(TimeState::kCurrent).velocity);
}

void InputChecker::CalcMouseVelocity()
{
	Vector2D<int> distance = m_mouse_data.at(TimeState::kCurrent).pos - m_mouse_data.at(TimeState::kPrev).pos;
	m_mouse_data.at(TimeState::kCurrent).velocity = v2d::ConvertVecType<float>(distance);
}

void InputChecker::CountInputTimeAll()
{
	for (auto& [input_k, input_n, state_t, data] : m_input_data)
	{
		if (state_t == TimeState::kPrev) { continue; }

		if (data.is_input)
		{
			data.input_time += FPS::GetDeltaTime();
			continue;
		}
		data.input_time = 0.0f;
	}
}

void InputChecker::CheckInputAll()
{
	for (auto& [input_k, input_n, state_t, data] : m_input_data)
	{
		if (state_t == TimeState::kPrev) { continue; }

		switch (input_k)
		{
		case InputKind::kKey:
			data.is_input = CheckHitKey(input_n) ? true : false;
			break;

		case InputKind::kMouseButton:
			data.is_input = IsInputMouseButton(input_n) ? true : false;
			break;

		case InputKind::kMouseWheel:
			data.is_input = GetMouseWheelParameter(static_cast<MouseWheelKind>(input_n)) ? true : false;
			break;

		case InputKind::kMouseSlide:
			data.is_input = IsSlideMouse(static_cast<MouseSlideDirectionKind>(input_n)) ? true : false;
			break;

		case InputKind::kPadButton:
			data.is_input = m_xinput.Buttons[input_n] ? true : false;
			break;

		case InputKind::kPadTrigger:
			data.is_input = GetPadTriggerParameter(static_cast<TriggerKind>(input_n)) ? true : false;
			break;

		case InputKind::kPadStick:
			data.is_input = GetPadStickParameter(static_cast<StickKind>(input_n)) ? true : false;
			break;
		}
	}
}

void InputChecker::ShiftDataCureentToPrev()
{
	m_mouse_data.at(TimeState::kPrev) = m_mouse_data.at(TimeState::kCurrent);

	for (const auto& [current_input_k, current_input_n, current_state_t, current_data] : m_input_data)
	{
		if (current_state_t == TimeState::kPrev) { continue; }

		for (auto& [prev_input_k, prev_input_n, prev_state_t, prev_data] : m_input_data)
		{
			if (prev_state_t == TimeState::kCurrent) { continue; }

			if (prev_input_k == current_input_k && prev_input_n == current_input_n)
			{
				prev_data = current_data;
			}
		}
	}
}

void InputChecker::DetectCurrentInputDevice()
{
	for (const auto& [input_k, input_n, state_t, data] : m_input_data)
	{
		if (state_t == TimeState::kPrev) { continue; }
		if (!data.is_input)				 { continue; }

		// マウスをスライドさせただけでは入力デバイスに影響を与えない
		if (input_k == InputKind::kKey || input_k == InputKind::kMouseButton || input_k == InputKind::kMouseWheel)
		{
			m_current_device = DeviceKind::kKeyboard;
			return;
		}
		if (input_k == InputKind::kPadButton || input_k == InputKind::kPadTrigger || input_k == InputKind::kPadStick)
		{
			m_current_device = DeviceKind::kPad;
			return;
		}
	}
}

float InputChecker::SupportGetInputTime(InputKind input_kind, const int input_num, TimeState time_state)
{
	for (const auto& [input_k, input_n, state_t, data] : m_input_data)
	{
		if (input_k == input_kind && input_n == input_num && state_t == time_state)
		{
			return data.input_time;
		}
	}
	return 0.0f;
}

InputState InputChecker::SupportGetInputState(InputKind input_kind, const int input_num)
{
	bool prev_is_input	  = false;
	bool current_is_input = false;

	for (const auto& [input_k, input_n, state_t, data] : m_input_data)
	{
		if (input_k == input_kind && input_n == input_num)
		{
			if (state_t == TimeState::kPrev)
			{
				prev_is_input = data.is_input;
			}
			else if (state_t == TimeState::kCurrent)
			{
				current_is_input = data.is_input;
			}
		}
	}

	if (current_is_input)
	{
		return prev_is_input ? InputState::kHold : InputState::kSingle;
	}
	return prev_is_input ? InputState::kPrev : InputState::kNone;
}
