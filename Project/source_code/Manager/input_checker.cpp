#include "input_checker.hpp"

InputChecker::InputChecker():
	m_csv					(std::make_unique<CSV>()),
	m_xinput				(-1),
	m_current_device		(DeviceKind::kKeyboard),
	m_is_lock_mouse_pos		(true)
{	
	SetUseDirectInputFlag(TRUE);

	m_mouse_data[TimeState::kPrev].pos	     = m_mouse_data[TimeState::kCurrent].pos	   = Vector2D<int>(Window::kHalfWidth, Window::kHalfHeight);
	m_mouse_data[TimeState::kPrev].dir		 = m_mouse_data[TimeState::kCurrent].dir	   = Vector2D<float>(0.0f, 0.0f);
	m_mouse_data[TimeState::kPrev].velocity  = m_mouse_data[TimeState::kCurrent].velocity  = Vector2D<float>(0.0f, 0.0f);

	DeactivateCursor();
	LockCursor();

	m_key_number = m_csv->Read1DCSV<std::vector<int>>("data/csv/key_number.csv", false);
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

	UpdateMouse();

	GetJoypadXInputState(DX_INPUT_PAD1, &m_xinput);

	CheckInputAll();
	CountInputTimeAll();

	DetectCurrentInputDevice();
}

void InputChecker::UpdateMouse()
{
	GetMousePoint(&m_mouse_data.at(TimeState::kCurrent).pos.x, &m_mouse_data.at(TimeState::kCurrent).pos.y);
	m_mouse_data.at(TimeState::kCurrent).wheel_rotation = GetMouseWheelRotVol();
	CalcMouseVelocity();
	CalcMouseDir();
}

void InputChecker::LockCursor()
{
	if (!m_is_lock_mouse_pos) { return; }

	Vector2D<int> center_pos = Vector2D<int>(Window::kHalfWidth, Window::kHalfHeight);
	m_mouse_data.at(TimeState::kPrev).pos = m_mouse_data.at(TimeState::kCurrent).pos = center_pos;
	SetMousePoint(center_pos.x, center_pos.y);
}

void InputChecker::AddInputData(const InputKind kind, const int input_code)
{
	for (int i = 0; i < input_code; ++i)
	{
		m_input_data.emplace_back(kind, i, TimeState::kPrev,	InputData());
		m_input_data.emplace_back(kind, i, TimeState::kCurrent, InputData());
	}
}

void InputChecker::CalcMouseDir()
{
	m_mouse_data.at(TimeState::kCurrent).dir = v2d::GetNormalizedVector(m_mouse_data.at(TimeState::kCurrent).velocity);
}

void InputChecker::CalcMouseVelocity()
{
	const Vector2D<int> distance = m_mouse_data.at(TimeState::kCurrent).pos - m_mouse_data.at(TimeState::kPrev).pos;
	m_mouse_data.at(TimeState::kCurrent).velocity = v2d::ConvertVecType<float>(distance);
}

void InputChecker::CountInputTimeAll()
{
	for (auto& [input_c, state_t, data] : m_input_data)
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
	for (auto& [input_c, state_t, data] : m_input_data)
	{
		if (state_t == TimeState::kPrev) { continue; }

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
	m_mouse_data.at(TimeState::kPrev) = m_mouse_data.at(TimeState::kCurrent);

	for (const auto& [current_input_c, current_state_t, current_data] : m_input_data)
	{
		// 1フレーム前の情報であった場合はスキップ
		if (current_state_t == TimeState::kPrev) { continue; }

		for (auto& [prev_input_c, prev_state_t, prev_data] : m_input_data)
		{
			// 現在のフレームの情報であった場合はスキップ
			if (prev_state_t == TimeState::kCurrent) { continue; }

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
		if (state_t == TimeState::kPrev) { continue; }
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
