#pragma once
#include <vector>
#include <unordered_map>
#include <tuple>
#include <DxLib.h>

#include "../Base/singleton_base.hpp"

#include "../FPS/fps.hpp"
#include "../Window/window.hpp"
#include "../Data/CSV/csv.hpp"
#include "../Data/input_data.hpp"
#include "../Data/Kind/input_kind.hpp"
#include "../Concept/input_concepts.hpp"
#include "../Math/Vector/vector.hpp"

/// @brief 役割 : 入力デバイスの検出, 入力状態の取得, 入力時間の計測
class InputChecker : public SingletonBase<InputChecker>
{
public:
	friend SingletonBase<InputChecker>;

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
	[[nodiscard]] Vector2D<int>   GetMousePos	   (TimeState time_state)const { return m_mouse_data.at(time_state).pos; }
	[[nodiscard]] Vector2D<float> GetMouseDir(TimeState time_state)const { return m_mouse_data.at(time_state).dir; }
	[[nodiscard]] Vector2D<float> GetMouseVelocity (TimeState time_state)const { return m_mouse_data.at(time_state).velocity; }
	#pragma endregion


	#pragma region 入力判定
	[[nodiscard]] bool IsInputMouseButton	 (const int				  input_num)const;
	[[nodiscard]] int  GetMouseWheelParameter(MouseWheelKind		  input_num)const;
	[[nodiscard]] bool IsSlideMouse			 (MouseSlideDirKind input_num)const;
	[[nodiscard]] bool IsInputPadButton		 (const int				  input_num)const { return m_xinput.Buttons[input_num]; }
	[[nodiscard]] int  GetPadStickParameter	 (StickKind				  input_num)const;
	[[nodiscard]] int  GetPadTriggerParameter(TriggerKind			  input_num)const;
	#pragma endregion


	#pragma region 入力時間
	[[nodiscard]] float GetKeyInputTime  (const int     input_num, TimeState time_state);
	template<input_concepts::MouseInputT MouseT>
	[[nodiscard]] float GetMouseInputTime(const MouseT& input_num, TimeState time_state)const
	{
		if (std::is_same_v<MouseT, int>)
		{
			return SupportGetInputTime(InputKind::kMouseButton, input_num, time_state);
		}
		if (std::is_same_v<MouseT, MouseSlideDirKind>)
		{
			return SupportGetInputTime(InputKind::kMouseWheel, input_num, time_state);
		}
		return SupportGetInputTime(InputKind::kMouseSlide, input_num, time_state);
	}
	template<input_concepts::PadInputT   PadT>
	[[nodiscard]] float GetPadInputTime  (const PadT&   input_num, TimeState time_state)const
	{
		if (std::is_same_v<PadT, int>)
		{
			return SupportGetInputTime(InputKind::kPadButton, input_num, time_state);
		}
		if (std::is_same_v<PadT, TriggerKind>)
		{
			return SupportGetInputTime(InputKind::kPadTrigger, input_num, time_state);
		}
		return SupportGetInputTime(InputKind::kPadStick, input_num, time_state);
	}
	#pragma endregion


	#pragma region 入力状態
	[[nodiscard]] InputState GetKeyInputState  (const int     input_num);
	template<input_concepts::MouseInputT MouseT>
	[[nodiscard]] InputState GetMouseInputState(const MouseT& input_num)const
	{
		InputKind kind = InputKind::kMouseButton;

		if (std::is_same_v<MouseT, MouseSlideDirKind>) { kind = InputKind::kMouseWheel; }
		if (std::is_same_v<MouseT, MouseWheelKind>)			 { kind = InputKind::kMouseSlide; }

		return SupportGetInputState(kind, input_num);
	}
	template<input_concepts::PadInputT   PadT>
	[[nodiscard]] InputState GetPadInputState  (const PadT&   input_num)const
	{
		InputKind kind = InputKind::kPadButton;

		if (std::is_same_v<PadT, TriggerKind>) { kind = InputKind::kPadTrigger; }
		if (std::is_same_v<PadT, StickKind>)   { kind = InputKind::kPadStick; }

		return SupportGetInputState(kind, input_num);
	}
	#pragma endregion


	/// @brief 現在の入力デバイスを取得
	/// @brief キーボードとパッド両方が入力された場合はキーボードを優先
	[[nodiscard]] DeviceKind GetCurrentInputDevice()const { return m_current_device; }

private:
	InputChecker();
	~InputChecker()override;

	void AddInputData(InputKind kind, const int input_num);

	void CalcMouseDir();
	void CalcMouseVelocity ();

	void CountInputTimeAll();
	void CheckInputAll();

	/// @brief 入力データを移行する
	void ShiftDataCureentToPrev();

	/// @brief 現在の入力デバイスを検出
	void DetectCurrentInputDevice();

	/// @brief 入力時間の取得を援助する
	[[nodiscard]] float		 SupportGetInputTime (InputKind input_kind, const int input_num, TimeState time_state);
	/// @brief 入力状態の取得を援助する
	[[nodiscard]] InputState SupportGetInputState(InputKind input_kind, const int input_num);

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

	static InputChecker* m_instance;
};
