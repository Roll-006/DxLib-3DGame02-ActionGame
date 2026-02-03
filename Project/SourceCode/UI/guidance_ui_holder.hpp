#pragma once
#include <queue>
#include "guidance_ui.hpp"

/*
・UIの待機時間は、自身より前に描画するものがあるかないかで変化させる
*/

class GuidanceUIHolder final
{
public:
	GuidanceUIHolder();
	~GuidanceUIHolder();

	void LateUpdate();
	void Draw(const int main_screen_handle) const;

private:
	void Activate(const std::string& ui_name);

private:
	std::unordered_map<std::string, std::shared_ptr<GuidanceUI>>	m_stock_ui;		// すべてのガイダンスUI
	std::queue<std::shared_ptr<GuidanceUI>>							m_wait_ui;		// アクティブ状態を待っているUI
	std::shared_ptr<GuidanceUI>										m_active_ui;	// アクティブ化されているUI
};
