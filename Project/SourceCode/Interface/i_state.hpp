#pragma once

class IState abstract
{
public:
	virtual ~IState() = default;

	virtual void Update()		abstract;
	virtual void LateUpdate()	abstract;
	virtual void Enter()		abstract;
	virtual void Exit()			abstract;

	/// @brief 停止させるステートを追加する
	/// @param state_handle ステートハンドル
	virtual void AddStopState(const int state_handle) abstract;

	[[nodiscard]] virtual int  GetNextStateKind()					  abstract;
	[[nodiscard]] virtual int  GetStateKind()					const abstract;
	[[nodiscard]] virtual int  GetStateHandle()					const abstract;
	[[nodiscard]] virtual bool IsStop(const int state_handle)	const abstract;
	[[nodiscard]] virtual bool IsStopAllState()					const abstract;
};
