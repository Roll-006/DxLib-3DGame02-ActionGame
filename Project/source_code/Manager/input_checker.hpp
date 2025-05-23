#pragma once
#include <vector>
#include <unordered_map>
#include <tuple>

#include "../Base/singleton_base.hpp"

#include "../FPS/fps.hpp"
#include "../Window/window.hpp"

#include "../Data/CSV/csv.hpp"
#include "../Data/input_data.hpp"
#include "../Data/Kind/device_kind.hpp"
#include "../Concept/input_concepts.hpp"

#include "../Data/IncludeList/vector.hpp"

/// @brief 役割 : 入力デバイスの検出, 入力状態の取得, 入力時間の計測
class InputChecker final : public SingletonBase<InputChecker>
{
public:
	enum class TimeState
	{
		kPrev,
		kCurrent,
	};

public:
	void Update();

	void InitMousePos();
	void InitMouseCursor()const;
	void ActivateMouseCursor();
	void DeactivateMouseCursor();


	#pragma region マウス情報
	[[nodiscard]] Vector2D<int>   GetMousePos	   (const TimeState time_state)const { return m_mouse_data.at(time_state).pos; }
	[[nodiscard]] Vector2D<float> GetMouseDir	   (const TimeState time_state)const { return m_mouse_data.at(time_state).dir; }
	[[nodiscard]] Vector2D<float> GetMouseVelocity (const TimeState time_state)const { return m_mouse_data.at(time_state).velocity; }
	#pragma endregion


	/// @brief 入力判定
	template<input_concepts::InputT InputT>
	[[nodiscard]] bool IsInput(const InputT& input_code)const
	{
		// キー
		if (std::is_same_v<int, InputT>)
		{
			return CheckHitKey(static_cast<int>(input_code));
		}
		// マウスボタン
		if (std::is_same_v<mouse::ButtonKind, InputT>)
		{
			return (GetMouseInput() & static_cast<int>(input_code)) != 0;
		}
		// マウスホイール
		if (std::is_same_v<mouse::WheelKind, InputT>)
		{
			return GetInputParameter(static_cast<mouse::WheelKind>(input_code));
		}
		// マウススライド
		if (std::is_same_v<mouse::SlideDirKind, InputT>)
		{
			switch (static_cast<mouse::SlideDirKind>(input_code))
			{
			case mouse::SlideDirKind::kLeft:  if (m_mouse_data.at(TimeState::kCurrent).pos.x < m_mouse_data.at(TimeState::kPrev).pos.x) { return true; } break;
			case mouse::SlideDirKind::kRight: if (m_mouse_data.at(TimeState::kCurrent).pos.x > m_mouse_data.at(TimeState::kPrev).pos.x) { return true; } break;
			case mouse::SlideDirKind::kDown:  if (m_mouse_data.at(TimeState::kCurrent).pos.y > m_mouse_data.at(TimeState::kPrev).pos.y) { return true; } break;
			case mouse::SlideDirKind::kUp:    if (m_mouse_data.at(TimeState::kCurrent).pos.y < m_mouse_data.at(TimeState::kPrev).pos.y) { return true; } break;
			}
		}
		// パッドボタン
		if (std::is_same_v<pad::ButtonKind, InputT>)
		{
			return m_xinput.Buttons[static_cast<int>(input_code)];
		}
		// パッドトリガー
		if (std::is_same_v<pad::TriggerKind, InputT>)
		{
			return GetInputParameter(static_cast<pad::TriggerKind>(input_code));
		}
		// パッドスティック
		if (std::is_same_v<pad::StickKind, InputT>)
		{
			return GetInputParameter(static_cast<pad::StickKind>(input_code));
		}
		return false;
	}

	/// @brief 入力パラメータを取得
	template<input_concepts::ParameterT InputT>
	[[nodiscard]] int GetInputParameter(const InputT& input_code)const
	{
		// マウススライド
		if (std::is_same_v<mouse::WheelKind, InputT>)
		{
			const int rota = m_mouse_data.at(TimeState::kCurrent).wheel_rotation;

			switch (static_cast<mouse::WheelKind>(input_code))
			{
			case mouse::WheelKind::kUp:		return rota > 0 ? rota : 0;	break;
			case mouse::WheelKind::kDown:	return rota < 0 ? rota : 0;	break;
			}
		}
		// パッドトリガー
		if (std::is_same_v<pad::TriggerKind, InputT>)
		{
			switch (static_cast<pad::TriggerKind>(input_code))
			{
			case pad::TriggerKind::kLT: if (m_xinput.LeftTrigger  > kTriggerDeadZone) { return m_xinput.LeftTrigger; }  break;
			case pad::TriggerKind::kRT: if (m_xinput.RightTrigger > kTriggerDeadZone) { return m_xinput.RightTrigger; } break;
			}
		}
		// パッドスティック
		if (std::is_same_v<pad::StickKind, InputT>)
		{
			switch (static_cast<pad::StickKind>(input_code))
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
		}
		return 0;
	}

	/// @brief 入力時間を取得
	template<input_concepts::InputT InputT>
	[[nodiscard]] float GetInputTime(const InputT& input_code, const TimeState time_state)
	{
		InputKind kind = InputKind::kKey;

		if (std::is_same_v<mouse::ButtonKind, InputT>)	{ kind = InputKind::kMouseButton; }
		if (std::is_same_v<mouse::WheelKind, InputT>)	{ kind = InputKind::kMouseWheel; }
		if (std::is_same_v<mouse::SlideDirKind, InputT>){ kind = InputKind::kMouseSlide; }
		if (std::is_same_v<pad::ButtonKind, InputT>)	{ kind = InputKind::kPadButton; }
		if (std::is_same_v<pad::TriggerKind, InputT>)	{ kind = InputKind::kPadTrigger; }
		if (std::is_same_v<pad::StickKind, InputT>)		{ kind = InputKind::kPadStick; }

		for (const auto& [input_k, input_n, state_t, data] : m_input_data)
		{
			if (input_k == kind && input_n == input_code && state_t == time_state)
			{
				return data.input_time;
			}
		}
		return 0.0f;
	}

	/// @brief 入力状態を取得
	template<input_concepts::InputT InputT>
	[[nodiscard]] InputState GetInputState(const InputT& input_code)
	{
		InputKind kind			= InputKind::kKey;
		bool prev_is_input		= false;
		bool current_is_input	= false;

		if (std::is_same_v<mouse::ButtonKind, InputT>)	{ kind = InputKind::kMouseButton; }
		if (std::is_same_v<mouse::WheelKind, InputT>)	{ kind = InputKind::kMouseWheel; }
		if (std::is_same_v<mouse::SlideDirKind, InputT>){ kind = InputKind::kMouseSlide; }
		if (std::is_same_v<pad::ButtonKind, InputT>)	{ kind = InputKind::kPadButton; }
		if (std::is_same_v<pad::TriggerKind, InputT>)	{ kind = InputKind::kPadTrigger; }
		if (std::is_same_v<pad::StickKind, InputT>)		{ kind = InputKind::kPadStick; }

		for (const auto& [input_k, input_n, state_t, data] : m_input_data)
		{
			if (input_k == kind && input_n == input_code)
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

	/// @brief 現在の入力デバイスを取得
	/// @brief キーボードとパッド両方が入力された場合はキーボードを優先
	[[nodiscard]] DeviceKind GetCurrentInputDevice()const { return m_current_device; }

private:
	InputChecker();
	~InputChecker()override;

	void UpdateMouse();

	void AddInputData(const InputKind kind, const int input_code);

	void CalcMouseDir();
	void CalcMouseVelocity();

	void CountInputTimeAll();
	void CheckInputAll();

	/// @brief 入力データを移行する
	void ShiftDataCureentToPrev();

	/// @brief 現在の入力デバイスを検出
	void DetectCurrentInputDevice();

public:
	static constexpr short			kStickMaxTilt		= SHRT_MAX;		// 傾きの最大値
	static constexpr short			kStickMinTilt		= SHRT_MIN;		// 傾きの最小値
	static constexpr short			kStickDeadZone		= 10000;		// スティック入力無効範囲(指定可能範囲 : -32768～32767)
	static constexpr unsigned char	kTriggerDeadZone	= 50;			// トリガー入力無効範囲  (指定可能範囲 : 0～255)

private:
	static constexpr int			kMouseButtonNum		= 8;
	static constexpr int			kMouseWheelNum		= 2;
	static constexpr int			kMouseSlideNum		= 4;
	static constexpr int			kPadButtonNum		= 16;
	static constexpr int			kPadTriggerNum		= 2;
	static constexpr int			kPadStickNum		= 8;

	std::unique_ptr<CSV>			m_csv;
	XINPUT_STATE					m_xinput;
	DeviceKind						m_current_device;
	bool							m_is_active_mouse_cursor;
	bool							m_is_init_mouse_pos;
	std::vector<int>				m_key_number;

	std::vector<std::tuple<InputKind, int, TimeState, InputData>> m_input_data;
	std::unordered_map<TimeState, MouseData> m_mouse_data;
	
	friend SingletonBase<InputChecker>;
};
