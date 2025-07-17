#pragma once
#include <cassert>
#include <DxLib.h>

#include "../Base/one_instance_singleton_base.hpp"

#include "../FPS/fps.hpp"
#include "../Input/input_checker.hpp"

class Window final : public OneInstanceSingletonBase<Window>
{
public:
	Window();
	~Window();

	void Update();

private:
	void SetWindowMode();
	void SetWindowMode(const bool is_full_screen);

public:
	static constexpr int	kWidth		= 1920;
	static constexpr int	kHeight		= 1080;
	static constexpr int	kHalfWidth  = static_cast<int>(kWidth  * 0.5f);
	static constexpr int	kHalfHeight = static_cast<int>(kHeight * 0.5f);
	static constexpr VECTOR	kCenterPos	= { kHalfWidth, kHalfHeight, 0.0f };

private:
	bool m_is_full_screen;
	int  m_change_window_count;
};
