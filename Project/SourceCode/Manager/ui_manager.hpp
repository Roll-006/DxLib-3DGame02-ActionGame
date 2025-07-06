#pragma once
#include <memory>
#include <type_traits>
#include <unordered_map>

#include "../Base/singleton_base.hpp"
#include "../Base/ui_creator_base.hpp"

namespace ui_creator_concepts
{
	/// @brief UIクリエイター型である
	template<typename T>
	concept UICeratorT = std::is_base_of_v<UICreatorBase, T>;
}

class UIManager final : public SingletonBase<UIManager>
{
public:
	template<ui_creator_concepts::UICeratorT UICeratorT>
	void AddUICreator(std::shared_ptr<UICeratorT> ui_creator)
	{
		if (!m_ui_creator.count(ui_creator->GetName()))
		{
			m_ui_creator[ui_creator->GetName()] = ui_creator;
		}
	}
	void Remove(const std::string& name);

	void Draw();

	/// @brief オブジェクトをアクティブ化する
	void Activate	(const std::string& name) { m_ui_creator.at(name)->Activate(); }
	/// @brief オブジェクトを非アクティブ化する(削除せずに機能を停止)
	void Deactivate	(const std::string& name) { m_ui_creator.at(name)->Deactivate(); }

private:
	UIManager();
	~UIManager() override;

private:
	std::unordered_map<std::string, std::shared_ptr<UICreatorBase>> m_ui_creator;

	friend SingletonBase<UIManager>;
};
