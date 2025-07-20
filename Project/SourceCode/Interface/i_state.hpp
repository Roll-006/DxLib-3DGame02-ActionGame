#pragma once
#include <memory>
#include "../Concept/obj_concepts.hpp"

template<obj_concepts::ObjT ObjT>
class IState abstract
{
public:
	virtual ~IState() = default;

	virtual void Update		(ObjT* obj) abstract;
	virtual void LateUpdate	(ObjT* obj) abstract;
	virtual void Enter		(ObjT* obj) abstract;
	virtual void Exit		(ObjT* obj) abstract;

	/// @brief 停止させるステートを追加する
	/// @param state_handle ステートハンドル
	virtual void AddStopState(const int state_handle) abstract;

	/// @brief アクティブ化する
	virtual void Activate()   abstract;
	/// @brief 非アクティブ化する
	virtual void Deactivate() abstract;

	[[nodiscard]] virtual std::shared_ptr<IState<ObjT>> ChangeState(const ObjT* obj) abstract;
	[[nodiscard]] virtual int  GetStateKind()   const abstract;
	[[nodiscard]] virtual int  GetStateHandle() const abstract;
	[[nodiscard]] virtual bool IsActive()		const abstract;
	[[nodiscard]] virtual bool IsStop(const int state_handle) const abstract;
};
