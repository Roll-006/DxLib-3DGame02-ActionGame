#pragma once
#include <vector>
#include "../Interface/i_state.hpp"
#include "../Handle/handle_creator.hpp"

template<obj_concepts::ObjT ObjT>
class WeaponActionStateBase abstract : public IState<ObjT>
{
public:
	WeaponActionStateBase(const int state_kind) : 
		m_state_kind	(state_kind), 
		m_state_handle	(HandleCreator::GetInstance()->CreateHandle())
	{ }

	virtual ~WeaponActionStateBase() = default;

	/// @brief 停止させるステートを追加する
	/// @param state_handle ステートハンドル
	void AddStopState(const int state_handle) override { m_stop_states.emplace_back(state_handle); }

	[[nodiscard]] int  GetStateKind()	const override { return m_state_kind; }
	[[nodiscard]] int  GetStateHandle() const override { return m_state_handle; }
	[[nodiscard]] bool IsStop(const int state_handle) const override { return std::find(m_stop_states.begin(), m_stop_states.end(), state_handle) != m_stop_states.end(); }

protected:
	std::vector<int> m_stop_states;

private:
	int	 m_state_kind;
	int  m_state_handle;
};
