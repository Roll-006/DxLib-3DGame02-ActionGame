#pragma once
#include <memory>
#include "../Concept/obj_concepts.hpp"

template<obj_concepts::ObjT ObjT>
class IState abstract
{
public:
	virtual ~IState() = default;

	virtual void Update(ObjT* obj) abstract;
	virtual void Enter (const ObjT* obj) abstract;

	/// @brief アクティブ化する
	virtual void Activate()   abstract;
	/// @brief 非アクティブ化する
	virtual void Deactivate() abstract;

	[[nodiscard]] virtual std::shared_ptr<IState<ObjT>> ChangeState(const ObjT* obj) abstract;
	[[nodiscard]] virtual bool IsActive() const abstract;
};
