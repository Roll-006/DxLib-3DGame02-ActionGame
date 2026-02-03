#pragma once
#include <queue>
#include "guidance_ui.hpp"
#include "../Event/event_system.hpp"

/*
・UIの待機時間は、自身より前に描画するものがあるかないかで変化させる
*/

class GuidanceUIActivator final
{
public:
	GuidanceUIActivator();
	~GuidanceUIActivator();

	void LateUpdate();

	// FIXME : 簡易的にGetさせる
	[[nodiscard]] const std::shared_ptr<const GuidanceUI> GetActiveGaidanceUI() const { return m_active_ui; }

private:
	void Activate(const std::string& ui_name);


	#pragma region Event
	void ActivateOnSceneChange(const ChangeSceneEvent& event);
	#pragma endregion

private:
	std::unordered_map<std::string, std::shared_ptr<GuidanceUI>>	m_stock_ui;		// すべてのガイダンスUI
	std::queue<std::shared_ptr<GuidanceUI>>							m_wait_ui;		// アクティブ状態を待っているUI
	std::shared_ptr<GuidanceUI>										m_active_ui;	// アクティブ化されているUI
};
