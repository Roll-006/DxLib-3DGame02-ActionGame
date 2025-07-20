#pragma once
#include <vector>
#include "../Interface/i_state.hpp"
#include "../Handle/state_handler.hpp"

template<obj_concepts::ObjT ObjT>
class SpecialStateBase abstract : public IState<ObjT>
{
public:
	SpecialStateBase(const int state_kind) :
		m_state_kind	(state_kind),
		m_state_handle	(HandleCreator::GetInstance()->CreateHandle()),
		m_is_active		(true)
	{ }

	virtual ~SpecialStateBase() = default;


	/// @brief 停止させるステートを追加する
	/// @param state_handle ステートハンドル
	void AddStopState(const int state_handle) override { m_stop_states.emplace_back(state_handle); }

	/// @brief アクティブ化する
	void Activate()   override { m_is_active = true; }
	/// @brief 非アクティブ化する
	void Deactivate() override { m_is_active = false; }

	[[nodiscard]] int  GetStateKind()	const override { return m_state_kind; }
	[[nodiscard]] int  GetStateHandle() const override { return m_state_handle; }
	[[nodiscard]] bool IsActive()		const override { return m_is_active; }
	[[nodiscard]] bool IsStop(const int state_handle) const override { return std::find(m_stop_states.begin(), m_stop_states.end(), state_handle) != m_stop_states.end(); }

protected:
	std::vector<int> m_stop_states;

private:
	int	 m_state_kind;
	int  m_state_handle;
	bool m_is_active;
};
