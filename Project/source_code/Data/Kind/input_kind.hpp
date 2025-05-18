#pragma once

enum class DeviceKind
{
	kKeyboard,
	kPad,
};

enum class InputState
{
	kNone,		// 入力なし
	kPrev,		// 1フレーム前に入力された
	kSingle,	// 単押し
	kHold,		// 長押し
};

enum class InputKind
{
	kKey,
	kMouseButton,
	kMouseWheel,
	kMouseSlide,
	kPadButton,
	kPadTrigger,
	kPadStick,
};

enum class MouseSlideDirKind
{
	kLeft,
	kRight,
	kDown,
	kUp,
};

enum class MouseWheelKind
{
	kUp,
	kDown,
};

enum class StickKind
{
	kLSLeft,	// 左スティック左方向
	kLSRight,	// 左スティック右方向
	kLSDown,	// 左スティック下方向
	kLSUp,		// 左スティック上方向
	kRSLeft,	// 右スティック左方向
	kRSRight,	// 右スティック右方向
	kRSDown,	// 右スティック下方向
	kRSUp,		// 右スティック上方向
};

enum class TriggerKind
{
	kLT,
	kRT,
};
