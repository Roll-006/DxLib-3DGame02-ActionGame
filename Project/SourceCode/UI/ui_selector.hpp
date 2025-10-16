#pragma once
#include <vector>
#include "ui_button.hpp"
#include "ui_slider.hpp"

class UISelector final
{
public:
	UISelector(const int init_button_index, const bool is_first_stop_select, const bool is_loop_select);
	~UISelector();

	void Init();
	void Update();

	void AddUIButton(const std::shared_ptr<IUIElement> ui_button);

	[[nodiscard]] std::vector<std::shared_ptr<IUIElement>> GetUIButtons() const { return m_ui_buttons; }

private:
	void CalcButtonIndex();

private:
	static constexpr float kSelectIntervalTime	= 1.0f;
	static constexpr float kFirstSelectStopTime = 3.0f;

	std::vector<std::shared_ptr<IUIElement>> m_ui_buttons;
	int   m_init_button_index;
	int   m_prev_button_index;
	int   m_current_button_index;
	float m_select_timer;
	bool  m_is_first_stop_select;	// 最初の選択時に一瞬停止するかを判定
	bool  m_selected_stop;
	bool  m_is_loop_select;			// 長押しでループ選択するかを判定
};
