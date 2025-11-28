#pragma once
#include <memory>
#include <type_traits>
#include <unordered_map>

#include "../Algorithm/algorithm.hpp"
#include "../Base/singleton_base.hpp"
#include "../Interface/i_tab.hpp"

namespace tab_concepts
{
	/// @brief Tab型である
	template<typename T>
	concept TabT = std::is_base_of_v<ITab, T>;
}

class TabDrawer final : public SingletonBase<TabDrawer>
{
public:
	template<tab_concepts::TabT TabT>
	void AddTab(const std::shared_ptr<TabT>& tab)
	{
		if (m_tabs.contains(tab->GetTabHandle())) { return; }

		m_tabs[tab->GetTabHandle()] = tab;

		// 優先順位を追加・ソート
		m_draw_priority.emplace_back(tab->GetTabHandle(), tab->GetPriority());
		m_draw_priority = algorithm::Sort(m_draw_priority, SortKind::kAscending);
	}
	void RemoveTab(const int tab_handle);

	void Draw(const int main_screen_handle) const;

	/// @brief オブジェクトをアクティブ化する
	void Activate  (const int tab_handle);
	/// @brief オブジェクトを非アクティブ化する(削除せずに機能を停止)
	void Deactivate(const int tab_handle);

	[[nodiscard]] std::shared_ptr<ITab> GetTab(const int tab_handle) const { return m_tabs.contains(tab_handle) ? m_tabs.at(tab_handle) : nullptr; }

private:
	TabDrawer();
	~TabDrawer() override;

private:
	std::unordered_map<int, std::shared_ptr<ITab>> m_tabs;
	std::vector<std::pair<int, int>> m_draw_priority;

	friend SingletonBase<TabDrawer>;
};
