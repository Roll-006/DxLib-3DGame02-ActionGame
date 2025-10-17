#pragma once
#include <memory>
#include <typeindex>
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
		if (m_tabs.count(typeid(TabT))) { return; }

		m_tabs[typeid(TabT)] = tab;

		// 優先順位を追加・ソート
		m_draw_priority.emplace_back(typeid(TabT), tab->GetPriority());
		m_draw_priority = algorithm::Sort(m_draw_priority, SortKind::kAscending);
	}
	void RemoveTab(const std::type_index& type);

	void Draw() const;

	/// @brief オブジェクトをアクティブ化する
	void Activate  (const std::type_index& type);
	/// @brief オブジェクトを非アクティブ化する(削除せずに機能を停止)
	void Deactivate(const std::type_index& type);

	[[nodiscard]] std::shared_ptr<ITab> GetTab(const std::type_index& type) const { return m_tabs.count(type) ? m_tabs.at(type) : nullptr; }

private:
	TabDrawer();
	~TabDrawer() override;

private:
	std::unordered_map<std::type_index, std::shared_ptr<ITab>> m_tabs;
	std::vector<std::pair<std::type_index, int>> m_draw_priority;

	friend SingletonBase<TabDrawer>;
};
