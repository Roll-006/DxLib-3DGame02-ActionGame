#pragma once
#include "../Data/Name/game_obj_name.hpp"

#include "../Base/component_base.hpp"

#include "obj_base.hpp"

class GameObj : public ObjBase
{
public:
	GameObj(const std::string& name, const std::string& tag);
	~GameObj()override;

	void Init()			override;
	void Update()		override;
	void Draw()const	override;

	template<component_concepts::DerivedT CompT, typename... Args>
	void AttachComponent(Args&&... args)
	{
		// 上書き不可
		if (m_components.count(typeid(CompT))) { return; }

		// 参考URL : [ https://cpprefjp.github.io/reference/utility/forward.html ]
		m_components[typeid(CompT)] = std::make_shared<CompT>(std::forward<Args>(args)...);
	}
	template<component_concepts::DerivedT CompT>
	void DetachComponent()
	{
		m_components.erase(typeid(CompT));
	}

	template<component_concepts::DerivedT CompT>
	[[nodiscard]] std::shared_ptr<CompT> GetComponent()
	{
		if(m_components.count(typeid(CompT)))
		{
			return dynamic_pointer_cast<CompT>(m_components.at(typeid(CompT)));
		}
		return nullptr;
	}

private:
	std::unordered_map<std::type_index, std::shared_ptr<ComponentBase>> m_components;	// 型情報をKeyとして使用
};
