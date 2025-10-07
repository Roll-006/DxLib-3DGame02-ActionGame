#pragma once
#include <memory>
#include "../Concept/obj_concepts.hpp"

template<obj_concepts::ObjT ObjT>
class IState abstract
{
public:
	virtual ~IState() = default;

	virtual void Update		(std::shared_ptr<ObjT>& obj) abstract;
	virtual void LateUpdate	(std::shared_ptr<ObjT>& obj) abstract;
	virtual void Enter		(std::shared_ptr<ObjT>& obj) abstract;
	virtual void Exit		(std::shared_ptr<ObjT>& obj) abstract;

	/// @brief 停止させるステートを追加する
	/// @param state_handle ステートハンドル
	virtual void AddStopState(const int state_handle) abstract;

	[[nodiscard]] virtual std::shared_ptr<IState<ObjT>> ChangeState(std::shared_ptr<ObjT>& obj) abstract;
	[[nodiscard]] virtual int  GetStateKind()   const abstract;
	[[nodiscard]] virtual int  GetStateHandle() const abstract;
	[[nodiscard]] virtual bool IsStop(const int state_handle) const abstract;
	[[nodiscard]] virtual bool IsStopAllState() const abstract;
};
