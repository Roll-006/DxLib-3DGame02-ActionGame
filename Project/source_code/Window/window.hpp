#pragma once
#include <cassert>
#include <DxLib.h>

#include "../Base/one_instance_singleton_base.hpp"

#include "../FPS/fps.hpp"
#include "../Manager/input_checker.hpp"

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
	static constexpr int kWidth		 = 800;
	static constexpr int kHeight	 = 600;
	static constexpr int kHalfWidth  = static_cast<int>(kWidth  * 0.5f);
	static constexpr int kHalfHeight = static_cast<int>(kHeight * 0.5f);

private:
	int  m_change_window_count;
	bool m_is_full_screen;
};
