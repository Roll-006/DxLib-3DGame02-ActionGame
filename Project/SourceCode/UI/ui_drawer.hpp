#pragma once
#include <memory>
#include <type_traits>
#include <unordered_map>

#include "../Algorithm/algorithm.hpp"
#include "../Base/singleton_base.hpp"
#include "../Interface/i_ui_creator.hpp"

namespace ui_concepts
{
	/// @brief UIクリエイター型である
	template<typename T>
	concept UICeratorT = std::is_base_of_v<IUICreator, T>;
}

class UIDrawer final : public SingletonBase<UIDrawer>
{
public:
	template<ui_concepts::UICeratorT UICreatorT>
	void AddUICreator(const std::shared_ptr<UICreatorT>& ui_creator)
	{
		if (m_ui_creators.contains(ui_creator->GetName())) { return; }

		m_ui_creators[ui_creator->GetName()] = ui_creator;

		// 優先順位を追加・ソート
		m_draw_priority.emplace_back(ui_creator->GetName(), ui_creator->GetPriority());
		m_draw_priority = algorithm::Sort(m_draw_priority, SortKind::kAscending);
	}
	void RemoveUICreator(const std::string& name);

	void Draw(const int main_screen_handle);

	/// @brief オブジェクトをアクティブ化する
	void Activate  (const std::string& name);
	/// @brief オブジェクトを非アクティブ化する(削除せずに機能を停止)
	void Deactivate(const std::string& name);

	[[nodiscard]] std::shared_ptr<IUICreator> GetUICreator(const std::string& name) const { return m_ui_creators.contains(name) ? m_ui_creators.at(name) : nullptr; }

private:
	UIDrawer();
	~UIDrawer() override;

private:
	std::unordered_map<std::string, std::shared_ptr<IUICreator>> m_ui_creators;
	std::vector<std::pair<std::string, int>> m_draw_priority;

	friend SingletonBase<UIDrawer>;
};
