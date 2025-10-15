#pragma once
#include <vector>
#include "ui_button.hpp"
#include "ui_slider.hpp"

class UISelector final
{
public:
	UISelector();
	~UISelector();

	void Update();

private:
	std::vector<IUIElement> m_ui_elements;
};
