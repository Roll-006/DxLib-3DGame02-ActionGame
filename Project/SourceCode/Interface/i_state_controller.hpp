#pragma once
#include <memory>
#include <vector>
#include "i_state.hpp"

class IStateController abstract
{
public:
	virtual ~IStateController() = default;

	virtual void Init()			abstract;
	virtual void Update()		abstract;
	virtual void LateUpdate()	abstract;

protected:
	virtual void CreateState()			abstract;
	virtual void AddStopStatePair()		abstract;
	virtual void AddCheckStopState()	abstract;

	/// @brief ステートを変更
	virtual void ChangeState() abstract;

	/// @brief 変更するステートを生成
	[[nodiscard]] virtual std::vector<int> CreateChangeState() abstract;

	/// @brief 未来のステート構成を生成
	[[nodiscard]] virtual std::vector<int> CreateFutureState(const std::vector<int>& next_state) abstract;

	/// @brief ステートの停止処理
	virtual void StopState(std::vector<int>& future_state, const std::shared_ptr<IState>& stop_state) abstract;
};
