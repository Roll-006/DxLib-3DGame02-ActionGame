#pragma once
#include <vector>
#include "../Base/ui_button_base.hpp"
#include "../Base/ui_slider_base.hpp"

class UISelector final
{
public:
	UISelector(const int init_button_index, const bool is_first_stop_select, const bool is_loop_select);
	~UISelector();

	void Init();
	void Update();

	void AddUIButton(const std::shared_ptr<IUIElement> ui_button);

	[[nodiscard]] std::vector<std::shared_ptr<IUIElement>> GetUIButtons() const { return m_ui_buttons; }
	[[nodiscard]] int GetCurrentButtonIndex() const { return m_current_button_index; }

private:
	void CalcButtonIndex();

private:
	static constexpr float kSelectIntervalTime	= 0.2f;
	static constexpr float kFirstSelectStopTime = 0.7f;

	std::vector<std::shared_ptr<IUIElement>> m_ui_buttons;

	int   m_init_button_index;
	int   m_current_button_index;
	float m_select_timer;
	bool  m_is_first_stop_select;	// 最初の選択時に一瞬停止するかを判定
	bool  m_selected_stop;
	bool  m_is_loop_select;			// 長押しでループ選択するかを判定
};
